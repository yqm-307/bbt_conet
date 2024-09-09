#pragma once
#include <bbt/base/net/IPAddress.hpp>
#include <bbt/base/buffer/Buffer.hpp>
#include <bbt/coroutine/coroutine.hpp>
#include <bbt/coroutine/utils/lockfree/concurrentqueue.h>

namespace bbt::network::conet
{

namespace
{

class Connection; 
class TcpClient;
class TcpServer;
class Socket;

}


typedef bbt::net::IPAddress IPAddress;

typedef std::function<void()> CoPoolWorkCallback;

}