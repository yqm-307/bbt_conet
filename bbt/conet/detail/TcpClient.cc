#include <bbt/conet/detail/TcpClient.hpp>

namespace bbt::network::conet::detail
{

std::shared_ptr<interface::IConnection> TcpClient::Connect(const std::string& ip, short port)
{
    CoConnect(ip, port);
}

void TcpClient::CoConnect(const std::string& ip, short port)
{
    bbtco [=](){
        _ConnectCo(ip, port);
    };
}

void TcpClient::_ConnectCo(const std::string& ip, short port)
{
    IPAddress addr{ip, port};
    int socket = ::socket(AF_INET, SOCK_STREAM, 0);
    if (socket < 0) {
        OnError(Errcode{"tcpclient socket() error! errno=" + std::to_string(errno)});
        return;
    }
    
    int err = ::connect(socket, addr.getsockaddr(), addr.getsocklen());
    if (err != 0) {
        if (errno == EINTR || errno == EINPROGRESS) {
            OnError(Errcode{"try again", ErrType::ERRTYPE_CONNECT_TRY_AGAIN});
        }

        if (errno == ECONNREFUSED)
            OnError(Errcode{"", ErrType::ERRTYPE_CONNECT_CONNREFUSED});
        
        ::close(socket);
        return;
    }

    OnConnect(socket, addr);
}

std::shared_ptr<TIEventLoop> TcpClient::GetEventLoop()
{
    return m_event_loop.lock();
}


}
