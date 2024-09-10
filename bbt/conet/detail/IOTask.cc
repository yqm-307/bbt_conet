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

int64_t IOTask::GetId()
{
    return m_id;
}


}