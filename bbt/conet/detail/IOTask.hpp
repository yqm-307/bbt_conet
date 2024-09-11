#pragma once
#include <bbt/conet/detail/Define.hpp>

namespace bbt::network::conet::detail
{

class IOTask
{
public:
    IOTask(const OnDispatchCallback& handle);
    virtual ~IOTask();

    void Invoke(std::shared_ptr<interface::IConnection> conn, short event);

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