#pragma once
#include <optional>
#include <bbt/base/net/IPAddress.hpp>
#include <bbt/base/buffer/Buffer.hpp>
#include <bbt/coroutine/coroutine.hpp>
#include <bbt/network/Errcode.hpp>
#include <bbt/coroutine/utils/lockfree/concurrentqueue.h>
#include <bbt/conet/detail/interface/IConnection.hpp>
#include <bbt/conet/detail/interface/IEventLoop.hpp>
#include <bbt/conet/detail/interface/IService.hpp>

namespace bbt::network::conet
{

namespace detail
{

class Connection; 
class TcpClient;
class TcpServer;
class EventLoop;
class Socket;
class IOTask;

typedef int64_t EventId;

enum TaskStatus
{
    TASK_LISTENING  = 1,
    TASK_RUNNING    = 2,
    TASK_CANCEL     = 3,
    TASK_DONE       = 4,
};

enum ConnStatus
{
    CONN_DEFAULT    = 1,
    CONN_CONNECTED  = 2,
    CONN_DISCONNECTED = 3,
};

typedef std::function<void(std::shared_ptr<interface::IConnection>, short)> OnDispatchCallback;

}


typedef bbt::net::IPAddress IPAddress;

typedef std::function<void()> CoPoolWorkCallback;

}