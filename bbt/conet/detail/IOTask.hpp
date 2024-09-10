#pragma once
#include <bbt/conet/detail/Define.hpp>

namespace bbt::network::conet::detail
{

class IOTask
{
public:
    IOTask(const OnDispatchCallback& handle);
    virtual ~IOTask();

    template<typename ...Args>
    void Invoke(Args ...args) {
        Assert(m_status != TASK_DONE && m_status != TASK_RUNNING);
        if (m_status != TASK_CANCEL) {
            m_status = TASK_RUNNING;
            m_handle(args...);
            m_status = TASK_DONE;
        }
    }

    int64_t GetId();
    int Cancel();
protected:
    static int64_t GenId();
private:
    TaskStatus              m_status{TASK_LISTENING};
    OnDispatchCallback      m_handle{nullptr};
    int64_t                 m_id{0};
};

}