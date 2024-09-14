#pragma once
#include <bbt/conet/conet.hpp>

class EchoConn:
    public bbt::conet::detail::Connection
{
public:
    EchoConn(std::shared_ptr<bbt::conet::detail::TIEventLoop> evloop,
             int fd,
             const bbt::conet::IPAddress& addr,
             int timeout
            ):
            bbt::conet::detail::Connection(evloop, fd, addr, timeout)
    {}

    virtual ~EchoConn() {}

    virtual void OnRecv(const char* byte, size_t len) override
    {
        printf("[echoconn][onrecv] %s\n", byte);
    }

    virtual void OnTimeout() override
    {
        printf("[echoconn][ontimeout] timeout!\n");
    }

    virtual void OnSend(size_t len) override
    {
        printf("[echoconn][onsend] send %d bytes\n", len);
    }

    virtual void OnClose() override
    {
        printf("[echoconn][onclose]\n");
    }

    virtual void OnError(const bbt::network::Errcode& err) override
    {
        printf("[echoconn][onerror] %s\n", err.CWhat());
    }
private:
};