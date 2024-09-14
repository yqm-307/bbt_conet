#include <bbt/coroutine/coroutine.hpp>
#include <bbt/conet/detail/Connection.hpp>
#include <bbt/conet/detail/EventLoop.hpp>


namespace bbt::network::conet::detail
{

Connection::Connection(std::shared_ptr<TIEventLoop> evloop, int fd, const IPAddress& addr, int timeout):
    m_socket(fd),
    m_event_loop(evloop),
    m_last_active_time(bbt::clock::now<>()),
    m_input_buffer(new char[m_input_buffer_len])
{
    AssertWithInfo(evloop != nullptr, "need a eventloop!");
}

Connection::~Connection()
{
    delete[] m_input_buffer;
}

std::optional<Errcode> Connection::Run()
{
    if (IsClosed())
        return Errcode{"connection is closed!"};

    auto pthis = shared_from_this();
    if (pthis == nullptr)
        return Errcode{"please use std::shared_ptr instead of raw pointer!"};

    auto eventloop = _GetEventLoop();
    if (eventloop == nullptr)
        return Errcode{"eventloop is released!"};

    /* 连接主循环会在连接关闭后退出 */
    bbtco [=](){ pthis->_Co(); };

    auto wkthis = weak_from_this();
    /* 超时事件会在连接关闭后释放掉 */
    m_timeout_event = eventloop->RegistEvent(shared_from_this(), bbtco_emev_timeout | bbtco_emev_persist | bbtco_emev_finalize, m_timeout, [wkthis](auto, short event){
        if (wkthis.expired())
            return;

        auto conn = wkthis.lock();
        if (bbt::clock::is_expired<bbt::clock::ms>(conn->m_last_active_time + bbt::clock::ms(conn->m_timeout)))
            conn->OnTimeout();
    });

    if (m_timeout_event <= 0)
        return Errcode{"eventloop regist connect timeout event failed!"};

    return std::nullopt;
}

void Connection::OnTimeout()
{

}

bool Connection::IsClosed() const
{
    return (m_run_status == CONN_DISCONNECTED);
}

int Connection::GetFd() const
{
    return m_socket;
}

const IPAddress& Connection::GetPeerAddr() const
{
    return m_peer_addr;
}

void Connection::Close()
{
    if (m_run_status == CONN_DISCONNECTED)
        return;
    
    m_run_status = CONN_DISCONNECTED;

    if (m_timeout_event > 0) {
        auto eventloop = _GetEventLoop();
        if (eventloop != nullptr)
            Assert(eventloop->UnRegistEvent(m_timeout_event) == 0);

        m_timeout_event = -1;
    }

    Assert(m_socket >= 0);
    ::close(m_socket);
    m_socket = -1;
    OnClose();
}

std::optional<Errcode> Connection::Send(const bbt::buffer::Buffer& buf)
{
    if (IsClosed()) {
        return Errcode{"connection is closed!"};
    }

    bool not_free = true;
    int append_len = _AppendOutputBuffer(buf.Peek(), buf.DataSize());
    if (!m_output_buffer_is_free.compare_exchange_strong(not_free, false)) {
        if (append_len != buf.DataSize())
            return Errcode{"output buffer not enough!"};
        return std::nullopt;
    }

    return _RegistASendEvent();
}

int Connection::_OnSendEvent(std::shared_ptr<bbt::buffer::Buffer> buffer, short event)
{
    size_t len = 0;

    if (IsClosed()) 
        return -1;

    if (event & bbtco_emev_timeout) {
    } else if(event & bbtco_emev_writeable) {
        len = ::send(m_socket, buffer->Peek(), buffer->DataSize(), MSG_NOSIGNAL);
    }

    OnSend(len);

    if (IsClosed() || m_output_buffer.DataSize() <= 0) {
        auto eventloop = _GetEventLoop();
        if (eventloop)
            eventloop->UnRegistEvent(m_send_event);
        
        m_send_event = -1;
        m_output_buffer_is_free.exchange(true);
    } else {
        m_send_event = -1;
        _RegistASendEvent();
    }

    return 0;
}


int Connection::Send(const char* byte, size_t len)
{
    return 0;
}

int Connection::_AppendOutputBuffer(const char* data, size_t len)
{
    std::lock_guard<std::mutex> _(m_output_buffer_mtx);
    auto before = m_output_buffer.DataSize();
    m_output_buffer.WriteString(data, len);
    auto after_size = m_output_buffer.DataSize();

    int change_num = after_size - before;

    return change_num > 0 ? change_num : 0;
}

std::optional<Errcode> Connection::_RegistASendEvent()
{
    AssertWithInfo(!m_output_buffer_is_free.load(), "output buffer must be free!");
    AssertWithInfo(m_send_event <= 0, "must no send event!");
    auto buffer_sptr = std::make_shared<bbt::buffer::Buffer>();
    {
        std::lock_guard<std::mutex> _(m_output_buffer_mtx);
        buffer_sptr->Swap(m_output_buffer);
    }

    auto wkthis = weak_from_this();
    auto eventloop = _GetEventLoop();
    if (eventloop == nullptr)
        return Errcode{"eventloop is released!"};
    
    m_send_event = eventloop->RegistEvent(shared_from_this(), bbtco_emev_writeable | bbtco_emev_finalize, -1,
    [wkthis, buffer_sptr](auto, short event){
        auto pthis = wkthis.lock();
        if (pthis == nullptr) return;
        if (event & bbtco_emev_writeable)
            pthis->_OnSendEvent(buffer_sptr, event);
    });

    return std::nullopt;
}

std::shared_ptr<EventLoop> Connection::_GetEventLoop()
{
    return std::dynamic_pointer_cast<EventLoop>(m_event_loop.lock());
}

void Connection::_Co()
{
    /**
     * 这个函数是协程Connection的协程主函数
     */

    while (!IsClosed()) {
        
        int len = ::read(m_socket, m_input_buffer, m_input_buffer_len);
        if (len < 0) {
            if (errno == EINTR || errno == EAGAIN) {
                OnError(Errcode{"please try again!", ERRTYPE_NETWORK_RECV_TRY_AGAIN});
            } else if (errno == ECONNREFUSED) {
                OnError(Errcode{"connect refused!", ERRTYPE_NETWORK_RECV_TRY_AGAIN});
            } else {
                OnError(Errcode{"other errno" + std::to_string(errno), ERRTYPE_NETWORK_RECV_OTHER_ERR});
            }
            continue;
        } else if (len == 0) {
            Close();
            continue;
        }

        OnRecv(m_input_buffer, len);
    }
}



}