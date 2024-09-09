#pragma once
#include <bbt/conet/detail/Define.hpp>
#include <bbt/conet/detail/interface/ICoPool.hpp>

namespace bbt::network::conet::detail
{

class Work:
    public interface::IWork
{
public:
    Work(CoPoolWorkCallback&& workfunc):m_callback(std::move(workfunc)) {}
    Work(const CoPoolWorkCallback& workfunc):m_callback(workfunc) {}
    virtual ~Work() {}
    virtual void Invoke() override
    {
        m_callback();
    }
private:
    CoPoolWorkCallback m_callback{nullptr};
};

/**
 * @brief 复用Coroutine
 * 
 */
class CoPool:
    public interface::ICoPool,
    private bbt::templateutil::noncopyable
{
public:
    CoPool(int co_max);
    virtual ~CoPool();

    virtual int             Submit(const CoPoolWorkCallback& workfunc) override;
    virtual int             Submit(CoPoolWorkCallback&& workfunc) override;
    virtual void            Release() override;
protected:
    void                    _Start();
    void                    _CoRun();
private:
    volatile bool           m_is_running{true};
    const int               m_max_co_num{0};
    moodycamel::ConcurrentQueue<interface::IWork*> 
                            m_works_queue;
    std::atomic_int         m_running_co_num{};

    bbt::coroutine::sync::CoCond::SPtr
                            m_cond{nullptr};
    std::mutex              m_cond_mtx;

    bbt::thread::CountDownLatch m_latch;

};

}