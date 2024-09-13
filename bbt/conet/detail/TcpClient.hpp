#pragma once
#include <bbt/conet/detail/Define.hpp>
#include <bbt/conet/detail/interface/IConnection.hpp>

namespace bbt::network::conet::detail
{

class TcpClient
{
public:
    explicit TcpClient(std::shared_ptr<TIEventLoop> eventloop):m_event_loop(eventloop) {}
    virtual ~TcpClient() {}

    virtual std::shared_ptr<interface::IConnection> Connect(const std::string& ip, short port);
protected:
    virtual void OnConnect(int socket, const sockaddr* server_addr) = 0;
private:
    std::weak_ptr<TIEventLoop>          m_event_loop;
};


}