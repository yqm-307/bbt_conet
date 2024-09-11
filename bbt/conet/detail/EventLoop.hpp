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

    virtual EventId                         RegistEvent(std::shared_ptr<interface::IConnection> udata, int events, int timeout, const OnDispatchCallback& callback) override;
    virtual void                            Dispatch() override;
    virtual int                             UnRegistEvent(EventId id) override;
    virtual bool                            HasEvent(EventId id) override;

protected:
    /* 监听事件触发，做销毁操作 */
    void                                    _OnEvent(EventId id);
private:
    coroutine::pool::CoPool*                m_co_pool{nullptr};
    std::unordered_map<EventId, IOTask*> m_task_map;
    std::mutex                              m_task_map_mtx;
    const bool                              m_auto_dispatch{false};
};

}