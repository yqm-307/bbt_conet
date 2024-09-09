#pragma once
#include <bbt/network/interface/INetConnection.hpp>

namespace bbt::network::conet::interface
{

class IService:
    public bbt::network::interface::INetCallback
{

};

};