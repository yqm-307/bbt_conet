#pragma once
#include <bbt/conet/detail/interface/IConnection.hpp>

namespace bbt::network::conet::interface
{

template <typename TUData>
class IEventLoop
{
public:

    /**
     * @brief 注册一个监听者，监听者会监听事件，当事件被dispatch时触发callback
     * 
     * @param udata 用户数据
     * @param events 
     * @param timeout 
     * @param callback 
     * @return TListenerId 返回的监听者id
     */
    virtual int64_t RegistEvent(TUData udata, int events, int timeout, const std::function<void(TUData, short)>& callback) = 0;

    /**
     * @brief 反注册一个监听者
     * 
     * @param id 
     * @return int 0表示成功，-1失败
     */
    virtual int UnRegistEvent(int64_t id) = 0;

    /**
     * @brief 监听者是否还存在
     * 
     * @param id 
     * @return true 
     * @return false 
     */
    virtual bool HasEvent(int64_t id) = 0;


    /**
     * @brief 分发事件
     * 
     * 若有活跃的事件，则调用Dispatch可以将活跃的事件分发给对应注册的监听者
     * 
     */
    virtual void Dispatch() = 0;
};

}