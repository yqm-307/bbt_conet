#include <bbt/conet/detail/IOTask.hpp>

namespace bbt::network::conet::detail
{

IOTask::IOTask(const OnDispatchCallback& handle):
    m_handle(handle),
    m_id(GenId()),
    m_co_mtx(bbtco_make_comutex())
{
}

IOTask::~IOTask()
{

}

int64_t IOTask::GenId()
{
    static std::atomic_int64_t id{0};
    return ++id;
}

bool IOTask::Invoke(std::shared_ptr<interface::IConnection> conn, short event) {
    bool can_continue = false;
    std::unique_lock<bbt::co::sync::StdLockWapper> lock{m_co_mtx};
    Assert(m_status != TASK_RUNNING);
    if (m_status == TASK_DONE)
        return can_continue;


    if (m_status != TASK_CANCEL) {
        m_status = TASK_RUNNING;
        lock.unlock();
        can_continue = m_handle(conn, event);

        lock.lock();
        m_status = can_continue ? TASK_LISTENING : TASK_DONE;
    }

    return can_continue;
}

int64_t IOTask::GetId()
{
    return m_id;
}

int IOTask::Cancel()
{
    std::unique_lock<bbt::co::sync::StdLockWapper> lock{m_co_mtx};
    if (m_status != TASK_LISTENING) {
        return -1;
    }

    m_status = TASK_CANCEL;
    return 0;
}

}