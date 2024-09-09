#include <bbt/conet/detail/CoPool.hpp>

namespace bbt::network::conet::detail
{

CoPool::CoPool(int max):
    m_max_co_num(max),
    m_latch(m_max_co_num),
    m_cond(bbtco_make_cocond(m_cond_mtx))
{
    Assert(m_max_co_num > 0);
    _Start();
}

CoPool::~CoPool()
{
}

int CoPool::Submit(const CoPoolWorkCallback& workfunc)
{
    auto* work = new Work(workfunc);
    AssertWithInfo(m_works_queue.enqueue(work), "oom!");
    m_cond->NotifyOne();
    return 0;    
}

int CoPool::Submit(CoPoolWorkCallback&& workfunc)
{
    auto* work = new Work(std::forward<CoPoolWorkCallback&&>(workfunc));
    AssertWithInfo(m_works_queue.enqueue(work), "oom!");
    m_cond->NotifyOne();
    return 0;
}

void CoPool::Release()
{
    m_is_running = false;
    m_latch.Wait();
}

void CoPool::_Start()
{
    for (int i = 0; i < m_max_co_num; ++i)
        bbtco_desc("do worker co") [=](){
            _CoRun();
        };
}

void CoPool::_CoRun()
{
    m_running_co_num++;

    while (m_is_running) {
        interface::IWork* work = nullptr;
        if (!m_works_queue.try_dequeue(work)) {
            m_cond->Wait();
            continue;
        }

        work->Invoke();
        delete work;
    }

    m_running_co_num--;
    m_latch.Down();
}


};