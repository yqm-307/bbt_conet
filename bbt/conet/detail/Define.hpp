#pragma once
#include <bbt/coroutine/coroutine.hpp>
#include <bbt/coroutine/utils/lockfree/concurrentqueue.h>

namespace bbt::network::conet
{

typedef std::function<void()> CoPoolWorkCallback;

}