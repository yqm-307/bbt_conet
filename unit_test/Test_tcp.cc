#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

#include <bbt/conet/conet.hpp>

std::shared_ptr<bbt::conet::TIEventLoop> eventloop;

class TestConn:
    public bbt::conet::Connection
{
public:
    TestConn(std::shared_ptr<bbt::conet::TIEventLoop> evloop, int fd, const bbt::conet::IPAddress& addr, int timeout):
        Connection(evloop, fd, addr, timeout)
    {
    }

    virtual void                    OnRecv(const char* byte, size_t len) override { m_total_recv += len; }
    virtual void                    OnTimeout() override {}
    virtual void                    OnSend(size_t len) override { m_total_send += len; }
    virtual void                    OnClose() override { BOOST_TEST_MESSAGE("closed" << GetPeerAddr().GetIPPort()); }
    virtual void                    OnError(const bbt::network::Errcode& err) override { BOOST_ERROR(err.CWhat()); }
private:
    size_t          m_total_recv{0};
    size_t          m_total_send{0};
};

class TestServer:
    public bbt::conet::TcpServer
{
public:
    TestServer(std::shared_ptr<bbt::conet::TIEventLoop> loop, const std::string& ip, short port):
        TcpServer(loop, ip, port)
    {
    }

    virtual void OnError(const bbt::network::Errcode& err) override
    {
        BOOST_ERROR(err.CWhat());
    }

    virtual void OnAccept(int socket, const bbt::conet::IPAddress& addr) override
    {
        auto conn = std::make_shared<TestConn>(GetEventLoop(), socket, addr, 2000);
        conn->Run();
    }
private:
};

class TestClient:
    public bbt::conet::TcpClient
{
public:
    TestClient(std::shared_ptr<bbt::conet::TIEventLoop> eventloop):
        TcpClient(eventloop)
    {
    }

    virtual ConnectResult OnConnect(int socket, const bbt::conet::IPAddress& addr) override
    {
        auto conn = std::make_shared<TestConn>(GetEventLoop(), socket, addr, 2000);
        conn->Run();
        return {std::nullopt, conn};
    }


    virtual void OnError(const bbt::network::Errcode& err) override
    {
        BOOST_ERROR(err.CWhat());
    }
};

BOOST_AUTO_TEST_SUITE()

BOOST_AUTO_TEST_CASE(t_begin)
{
    eventloop = std::make_shared<bbt::conet::TIEventLoop>(1000, true);
}

BOOST_AUTO_TEST_CASE(t_c2s_connect)
{
    TestServer server{eventloop, "127.0.0.1", 9527};
    TestClient client{eventloop};

    bbtco_desc("main") [&](){
        server.CoStart();

        /* 确保server启动 */
        bbtco_sleep(100);
        auto [err, conn] = client.Connect("127.0.0.1", 9527);
        conn->Close();

        BOOST_ASSERT(conn->IsClosed());
    };


    sleep(1);
}

BOOST_AUTO_TEST_CASE(t_end)
{
    eventloop = nullptr;
}

BOOST_AUTO_TEST_SUITE_END()