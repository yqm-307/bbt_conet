#pragma once
#include <bbt/network/interface/INetConnection.hpp>

namespace bbt::network::conet::interface
{

class IConnection
{
    virtual void OnRecv(const char* byte, size_t len) = 0;
    virtual void Send(const char* byte, size_t len) = 0;
    virtual void OnSend(size_t len) = 0;
    virtual void OnClose() = 0;
    virtual void OnTimeout() = 0;
    virtual void Disconnect() = 0;
    virtual bool IsDisconnected() = 0;
};

} // namespace bbt::network::conet::interface