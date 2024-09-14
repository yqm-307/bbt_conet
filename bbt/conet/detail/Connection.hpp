#pragma once
#include <bbt/conet/detail/Define.hpp>
#include <bbt/conet/detail/interface/IConnection.hpp>

namespace bbt::network::conet::detail
{

class Connection:
    public interface::IConnection,
    public std::enable_shared_from_this<Connection>
{
public:
    explicit Connection(std::shared_ptr<TIEventLoop> evloop, int fd, const IPAddress& addr, int timeout);
    virtual ~Connection();

    std::optional<Errcode>          Run();
    virtual std::optional<Errcode>  Send(const bbt::buffer::Buffer& buf) final;
    virtual void                    Close() override final;
    virtual bool                    IsClosed() const override final;
    virtual int                     GetFd() const override final;
    virtual const IPAddress&        GetPeerAddr() const final;
protected:
    virtual void                    OnRecv(const char* byte, size_t len) = 0;
    virtual void                    OnTimeout() = 0;
    virtual void                    OnSend(size_t len) = 0;
    virtual void                    OnClose() = 0;
    virtual void                    OnError(const Errcode& err) = 0;

private:
    virtual int                     Send(const char* byte, size_t len) override;
    int                             _OnSendEvent(std::shared_ptr<bbt::buffer::Buffer> buffer, short event);
    void                            _Co();
    int                             _AppendOutputBuffer(const char* data, size_t len);
    std::optional<Errcode>          _RegistASendEvent();
    std::shared_ptr<EventLoop>      _GetEventLoop();
private:
    std::weak_ptr<TIEventLoop>      m_event_loop;
    int                             m_socket{-1};
    IPAddress                       m_peer_addr;
    const int                       m_timeout{-1};  // 连接空闲关闭超时
    bbt::clock::Timestamp<>         m_last_active_time;
    ConnStatus                      m_run_status{CONN_DEFAULT};

    volatile EventId                         m_send_event{-1};
    volatile EventId                         m_timeout_event{-1};

    bbt::buffer::Buffer             m_output_buffer;
    std::atomic_bool                m_output_buffer_is_free{true};
    std::mutex                      m_output_buffer_mtx;
    const int                       m_input_buffer_len{4096};
    char*                           m_input_buffer{nullptr};


};

};