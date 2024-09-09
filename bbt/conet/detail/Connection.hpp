#pragma once
#include <bbt/conet/detail/Define.hpp>
#include <bbt/conet/detail/interface/IConnection.hpp>

namespace bbt::network::conet::detail
{

class Connection:
    public interface::IConnection
{
public:
    explicit Connection(Socket socket);
    virtual ~Connection();

    virtual void OnRecv(const char* byte, size_t len) override;
    virtual void Send(const char* byte, size_t len) override;
    virtual void OnSend(size_t len) override;
    virtual void OnClose() override;
    virtual void OnTimeout() override;
    virtual void Disconnect() override;
    virtual bool IsDisconnected() override;
private:
    int         m_socket{-1};
};

};