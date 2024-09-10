#pragma once
#include <bbt/conet/detail/Define.hpp>

namespace bbt::network::conet::detail
{

class IODispatcher:
    public interface::IDispatcher
{
public:
    IODispatcher(size_t copool_size);
    virtual ~IODispatcher();

    virtual ListenerId RegListener(interface::IConnection* udata, int events, int timeout, const OnDispatchCallback& callback) override;
    virtual void Dispatch() override;
    virtual int UnRegListener(ListenerId id) override;
    virtual bool HasListener(ListenerId id) = 0;

protected:
    /* 监听事件触发，做销毁操作 */
    void        _OnListen(ListenerId id);
private:
    coroutine::pool::CoPool* m_co_pool{nullptr};
    std::unordered_map<ListenerId, IOTask*> m_task_map;
    std::mutex                          m_task_map_mtx;
};

}