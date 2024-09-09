#include <bbt/conet/detail/Connection.hpp>
#include <bbt/coroutine/coroutine.hpp>

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

}


}