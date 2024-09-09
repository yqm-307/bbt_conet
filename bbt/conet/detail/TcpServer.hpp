#pragma once
#include <bbt/conet/detail/Define.hpp>
#include <bbt/conet/detail/interface/IConnection.hpp>

namespace bbt::network::conet::detail
{

class TcpServer
{
public:
    explicit TcpServer(const std::string& ip, short port);
    virtual ~TcpServer();

    int     Start();
    void    Stop();
    std::shared_ptr<interface::IConnection> Accept();
protected:
    virtual std::shared_ptr<interface::IConnection> OnAccept(Socket* socket);
private:
    
};

}