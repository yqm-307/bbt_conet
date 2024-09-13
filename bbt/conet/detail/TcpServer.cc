#include <bbt/conet/detail/TcpServer.hpp>

namespace bbt::network::conet::detail
{

TcpServer::TcpServer(std::shared_ptr<TIEventLoop> loop, const std::string& ip, short port):
    m_event_loop(loop),
    m_listen_addr(ip, port)
{
}

TcpServer::~TcpServer()
{
}


std::optional<Errcode> TcpServer::Start()
{
    
}

void TcpServer::Stop(bool sync)
{
    if (sync)
        m_latch = new bbt::thread::CountDownLatch{1};

    m_is_running = false;

    if (sync)
        m_latch->Wait();
}

void TcpServer::_ListenCo()
{
    /**
     * 监听协程主函数
     */

    int listenfd = ::socket(AF_INET, SOCK_STREAM, -1);

    int err = ::listen(listenfd, 1024);
    if (err != 0) {
        OnError(Errcode{"tcp server listen failed! ip=" + m_listen_addr.GetIPPort() + " errno=" + std::to_string(errno), network::ErrType::ERRTYPE_ERROR});
        return;
    }

    while (m_is_running) {
        int client_socket = -1;
        sockaddr client_addr;
        socklen_t len = sizeof(client_addr);
        IPAddress addr;

        err =  ::accept(client_socket, (sockaddr*)&client_addr, &len);
        if (err != 0) {
            OnError(Errcode{"accpet error! errno=" + std::to_string(errno), network::ErrType::ERRTYPE_ERROR});
            continue;
        }

        OnAccept(client_socket, &client_addr);
    }

    if (m_latch) {
        m_latch->Down();
        delete m_latch;
    }
}




}