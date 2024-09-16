#pragma once
#include <bbt/conet/detail/Define.hpp>
#include <bbt/conet/detail/interface/IConnection.hpp>

namespace bbt::network::conet::detail
{

class Connection:
    public interface::IConnection,
    public std::enable_shared_from_this<Connection>
{
public:
    explicit Connection(std::shared_ptr<TIEventLoop> evloop, int fd, const IPAddress& addr, int timeout);
    virtual ~Connection();

    /**
     * 启动连接接收、超时事件
     */
    std::optional<Errcode>          Run();

    /**
     * @brief 向对端发送数据
     * 
     * @param buf 
     * @return std::optional<Errcode> 
     */
    virtual std::optional<Errcode>  Send(const bbt::buffer::Buffer& buf) final;

    /**
     * @brief 关闭连接，当有输出缓存未发送成功会等待数据全部发送，但是无法继续发送数据
     * 
     */
    virtual void                    Close() override final;

    /**
     * @brief 立即关闭连接，未发送的数据会被丢弃
     * 
     */
    virtual void                    Shutdown() override final;

    /**
     * @brief 连接是否关闭
     * 
     * @return true 
     * @return false 
     */
    virtual bool                    IsClosed() const override final;

    /**
     * @brief 获取该连接的套接字
     * 
     * @return int 
     */
    virtual int                     GetFd() const override final;

    /**
     * @brief 获取对端地址
     * 
     * @return const IPAddress& 
     */
    virtual const IPAddress&        GetPeerAddr() const final;
protected:

    /**
     * @brief 数据接收回调，当接收到套接字数据时触发
     * 
     * @param byte 
     * @param len 
     */
    virtual void                    OnRecv(const char* byte, size_t len) = 0;

    /**
     * @brief 超时回调，当连接超时时触发
     * 
     */
    virtual void                    OnTimeout() = 0;

    /**
     * @brief 发送回调，发送数据成功后触发
     * 
     * @param len 发送字节数 
     */
    virtual void                    OnSend(size_t len) = 0;

    /**
     * @brief 套接字关闭回调，当套接字关闭时触发。
     * ps：Close时不会立即触发，需要等到输出缓存全部发送完毕触发；Shutdown可以立即触发
     * 
     */
    virtual void                    OnClose() = 0;

    /**
     * @brief 错误处理回调，当连接在收发过程中出现错误触发
     * 
     * @param err 
     */
    virtual void                    OnError(const Errcode& err) = 0;

private:
    virtual int                     Send(const char* byte, size_t len) override;
    int                             _OnSendEvent(std::shared_ptr<bbt::buffer::Buffer> buffer, short event);
    int                             _AppendOutputBuffer(const char* data, size_t len);
    std::optional<Errcode>          _RegistASendEvent();
    std::optional<Errcode>          _RegistAMainEvent();
    std::shared_ptr<EventLoop>      _GetEventLoop();
    void                            _Shutdown();
    bool                            _OnMainEvent(short event);

    /* 连接事件 */
    std::optional<Errcode>          _Recv();
private:
    std::weak_ptr<TIEventLoop>      m_event_loop;
    int                             m_socket{-1};
    IPAddress                       m_peer_addr;
    const int                       m_timeout{-1};  // 连接空闲关闭超时
    bbt::clock::Timestamp<>         m_last_active_time;
    ConnStatus                      m_run_status{CONN_DEFAULT};
    std::mutex                      m_mutex;    // 状态管理的锁

    volatile EventId                m_send_event{-1};
    EventId                         m_main_event{-1};

    bbt::buffer::Buffer             m_output_buffer;
    std::atomic_bool                m_send_event_is_in_progress{false};  // 是否正在进行发送事件
    std::mutex                      m_output_buffer_mtx;
    const int                       m_input_buffer_len{4096};
    char*                           m_input_buffer{nullptr};


};

};