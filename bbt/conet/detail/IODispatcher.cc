#include <bbt/conet/detail/IODispatcher.hpp>
#include <bbt/conet/detail/IOTask.hpp>

namespace bbt::network::conet::detail
{

IODispatcher::IODispatcher(size_t copool_size)
{

}

IODispatcher::~IODispatcher()
{
    
}

ListenerId IODispatcher::RegListener(interface::IConnection* udata, int events, int timeout, const OnDispatchCallback& callback)
{
    if (udata->IsDisconnected())
        return -1;

    auto* task = new IOTask{[=](interface::IConnection* conn, short event){
        callback(conn, event);
    }};

    std::lock_guard<std::mutex> lock{m_task_map_mtx};
    auto [_, succ] = m_task_map.insert(std::make_pair(task->GetId(), task));
    Assert(succ);

    ListenerId id = task->GetId();

    /**
     * 这个事件是一定会触发的（除非event设置的有问题）
     */
    __bbtco_event_regist_ex(
        udata->Socket(),
        events,
        timeout
    ) [=](int fd, short event){
        task->Invoke(udata, event);
        _OnListen(task->GetId());
    };

}

int IODispatcher::UnRegListener(ListenerId id)
{
    if (id <= 0)
        return -1;
    
    std::unique_lock<std::mutex> lock{m_task_map_mtx};
    auto it = m_task_map.find(id);
    if (it == m_task_map.end())
        return -1;
    
    if (it->second->Cancel());

    return 0;
}


void IODispatcher::_OnListen(ListenerId id)
{
    std::unique_lock<std::mutex> lock{m_task_map_mtx};
    auto it = m_task_map.find(id);
    auto* task = it->second;
    Assert(m_task_map.erase(id) > 0);

    delete task;
}


}