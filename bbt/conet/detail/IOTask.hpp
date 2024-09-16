#pragma once
#include <bbt/conet/detail/Define.hpp>

namespace bbt::network::conet::detail
{

class IOTask
{
public:
    IOTask(const OnDispatchCallback& handle);
    virtual ~IOTask();

    /**
     * @brief 执行任务，根据返回值来判断此任务是否允许重复触发
     * 
     * 若返回false后，再次调用Invoke会导致未定义的行为
     * 
     * @param conn 
     * @param event 
     * @return true 
     * @return false 
     */
    bool                    Invoke(std::shared_ptr<interface::IConnection> conn, short event);

    /**
     * @brief 获取任务id
     * 
     */
    int64_t                 GetId();

    /**
     * @brief 取消该任务
     * 
     * @return int 若此任务没有正在等待中，则返回-1
     */
    int                     Cancel();
protected:
    static int64_t          GenId();
private:
    TaskStatus              m_status{TASK_LISTENING};
    OnDispatchCallback      m_handle{nullptr};
    int64_t                 m_id{0};
    bbt::coroutine::sync::CoMutex::SPtr
                            m_co_mtx{nullptr};
};

}