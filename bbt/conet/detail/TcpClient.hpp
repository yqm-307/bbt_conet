#pragma once
#include <bbt/conet/detail/Define.hpp>
#include <bbt/conet/detail/interface/IConnection.hpp>

namespace bbt::network::conet::detail
{

class TcpClient
{
public:
    explicit TcpClient();
    virtual ~TcpClient();

    virtual std::shared_ptr<interface::IConnection> Connect(const std::string& ip, short port);
protected:
    virtual void OnConnect(Socket socket);
private:
};


}