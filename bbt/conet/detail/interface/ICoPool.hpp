#pragma once
#include <bbt/conet/detail/Define.hpp>

namespace bbt::network::conet::interface
{

class IWork
{
public:
    virtual void Invoke() = 0;
};

class ICoPool
{
public:
    virtual int         Submit(const CoPoolWorkCallback& ) = 0;
    virtual int         Submit(CoPoolWorkCallback&& ) = 0;
    virtual void        Release() = 0;
};

}