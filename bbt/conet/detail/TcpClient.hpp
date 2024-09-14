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

    virtual std::shared_ptr<interface::IConnection> Connect(const std::string& ip, short port) final;
    virtual void                                    CoConnect(const std::string& ip, short port);
protected:
    virtual void                                    OnConnect(int socket, const IPAddress& addr) = 0;
    virtual void                                    OnError(const Errcode& err) = 0;
    virtual std::shared_ptr<TIEventLoop>            GetEventLoop() final;
private:
    void _ConnectCo(const std::string& ip, short port);
private:
    std::weak_ptr<TIEventLoop>          m_event_loop;
};


}