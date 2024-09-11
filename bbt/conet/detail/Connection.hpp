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
    explicit Connection(Socket* fd) {}
    virtual ~Connection() {}

    virtual void OnRecv(const char* byte, size_t len) override;
    virtual void Send(const char* byte, size_t len) override;
    virtual void OnSend(size_t len) override;
    virtual void OnClose() override;
    virtual void OnTimeout() override;
    virtual void Disconnect() override;
    virtual bool IsDisconnected() override;
    virtual int  GetFd() override;
    virtual void Close() override;
private:
    Socket*             m_socket{nullptr};
};

};