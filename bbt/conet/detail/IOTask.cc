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
    m_co_mtx->Lock();
    Assert(m_status != TASK_RUNNING);
    if (m_status == TASK_DONE)
        return can_continue;


    if (m_status != TASK_CANCEL) {
        m_status = TASK_RUNNING;
        can_continue = m_handle(conn, event);
        m_status = can_continue ? TASK_LISTENING : TASK_DONE;
    }
    m_co_mtx->UnLock();
    return can_continue;
}

int64_t IOTask::GetId()
{
    return m_id;
}

int IOTask::Cancel()
{
    m_co_mtx->Lock();
    if (m_status != TASK_LISTENING) {
        m_co_mtx->UnLock();
        return -1;
    }

    m_status = TASK_CANCEL;
    m_co_mtx->UnLock();
    return 0;
}

}