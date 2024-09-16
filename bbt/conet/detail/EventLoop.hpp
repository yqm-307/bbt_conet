#pragma once
#include <bbt/conet/detail/Define.hpp>

namespace bbt::network::conet::detail
{

class EventLoop:
    public interface::IEventLoop<std::shared_ptr<interface::IConnection>>
{
public:
    EventLoop(size_t copool_size, bool auto_dispatch);
    virtual ~EventLoop();

    /**
     * @brief 注册一个事件
     * ps：对于持久化事件使用需要谨慎，因为EventLoop内部使用了协程池，持久化事件触发会导致
     * 同时有多个协程持有持久化事件。对于需要严格执行次序的任务，请使用单次触发事件
     * 
     * @param udata 
     * @param events 
     * @param timeout 
     * @param callback 
     * @return EventId 
     */
    virtual EventId                         RegistEvent(std::shared_ptr<interface::IConnection> udata, int events, int timeout, const OnDispatchCallback& callback) override;

    /**
     * @brief 若构造EventLoop时设置了auto_dispatch为false，则需要主动调用该接口分发事件
     */
    virtual void                            Dispatch() override;

    virtual int                             UnRegistEvent(EventId id) override;

    virtual bool                            HasEvent(EventId id) override;

protected:
    /* 监听事件触发，做销毁操作 */
    void                                    _OnEvent(EventId id, bool can_continue);
private:
    std::shared_ptr<coroutine::pool::CoPool>m_co_pool{nullptr};
    std::unordered_map<EventId, IOTask*>    m_task_map;
    std::mutex                              m_task_map_mtx;
    const bool                              m_auto_dispatch{false};
};

}