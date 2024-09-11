#include <bbt/conet/detail/IOTask.hpp>

namespace bbt::network::conet::detail
{

IOTask::IOTask(const OnDispatchCallback& handle):
    m_handle(handle),
    m_id(GenId())
{
}

IOTask::~IOTask()
{

}

int64_t IOTask::GenId()
{
    static std::atomic_int64_t id{0};
    return id++;
}

void IOTask::Invoke(std::shared_ptr<interface::IConnection> conn, short event) {
    Assert(m_status != TASK_DONE && m_status != TASK_RUNNING);
    if (m_status != TASK_CANCEL) {
        m_status = TASK_RUNNING;
        m_handle(conn, event);
        m_status = TASK_DONE;
    }
}

int64_t IOTask::GetId()
{
    return m_id;
}

int IOTask::Cancel()
{

    if (m_status != TASK_LISTENING) {
        return -1;
    }

    m_status = TASK_CANCEL;
    return 0;
}

}