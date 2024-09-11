#include <bbt/coroutine/coroutine.hpp>
#include <bbt/conet/detail/Connection.hpp>
#include <bbt/conet/detail/Socket.hpp>

namespace bbt::network::conet::detail
{

void Connection::OnRecv(const char* byte, size_t len)
{

}

void Connection::Send(const char* byte, size_t len)
{

}

void Connection::OnSend(size_t len)
{

}

void Connection::OnClose()
{

}

void Connection::OnTimeout()
{

}

void Connection::Disconnect()
{

}

bool Connection::IsDisconnected()
{
    return true;
}

int Connection::GetFd()
{
    return m_socket->GetFd();
}

void Connection::Close()
{

}

}