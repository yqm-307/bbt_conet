#pragma once
#include <bbt/conet/detail/Define.hpp>

namespace bbt::network::conet::detail
{

class TcpServer
{
public:
    explicit TcpServer(std::shared_ptr<TIEventLoop> loop, const std::string& ip, short port);
    virtual ~TcpServer();

    std::optional<Errcode>                  Start();
    void                                    Stop(bool sync = false);

    virtual void                            OnError(const Errcode& err) = 0;
protected:
    virtual std::shared_ptr<interface::IConnection> OnAccept(int socket, const sockaddr* addr) = 0;
    virtual void _ListenCo();
private:
    std::weak_ptr<TIEventLoop>      m_event_loop;
    const IPAddress                 m_listen_addr;
    volatile bool                   m_is_running{true};
    bbt::thread::CountDownLatch*    m_latch;

};

}