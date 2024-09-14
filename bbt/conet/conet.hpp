#pragma once
#include <bbt/conet/detail/EventLoop.hpp>
#include <bbt/conet/detail/Connection.hpp>
#include <bbt/conet/detail/TcpClient.hpp>
#include <bbt/conet/detail/TcpServer.hpp>

namespace bbt::conet
{
using namespace bbt::network::conet;

typedef bbt::network::conet::detail::TcpClient TcpClient;
typedef bbt::network::conet::detail::TcpServer TcpServer;

}