#pragma once

namespace bbt::network::conet::interface
{

class IConnection
{
public:
    virtual void OnRecv(const char* byte, size_t len) = 0;
    virtual int  Send(const char* byte, size_t len) = 0;
    virtual void OnSend(size_t len) = 0;
    virtual void OnClose() = 0;
    virtual void OnTimeout() = 0;
    virtual void Close() = 0;
    virtual void Shutdown() = 0;
    virtual bool IsClosed() const = 0;
    virtual int  GetFd() const = 0;
    virtual int64_t GetId() const = 0;
};

} // namespace bbt::network::conet::interface