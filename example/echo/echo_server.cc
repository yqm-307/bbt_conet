#include <bbt/conet/conet.hpp>
#include "echoconnection.hpp"

class EchoServer:
    public bbt::conet::TcpServer
{
public:

    EchoServer(std::shared_ptr<bbt::conet::detail::EventLoop> eventloop, const std::string& ip, short port):
        bbt::conet::TcpServer(eventloop, ip, port)
    {}

    virtual ~EchoServer() {}

    virtual void OnConnect(int socket, const bbt::conet::IPAddress& addr)
    {
        auto conn = std::make_shared<EchoConn>(GetEventLoop(), socket, addr, 1000);
        bbtco [conn]{
            for (int i = 0; i < 10000; ++i) {
                conn->Send(bbt::buffer::Buffer{"hello world"});
            }
        };
    }

    virtual void OnError(const bbt::network::Errcode& err) override
    {
        printf("[echo serv] %s\n", err.CWhat());
    }

    virtual void OnAccept(int socket, const bbt::conet::IPAddress& addr) override
    {
        auto conn = std::make_shared<EchoConn>(GetEventLoop(), socket, addr, 1000);
        conn->Run();
    }
};

int main()
{
    auto eventloop = std::make_shared<bbt::conet::detail::EventLoop>(100, true);

    EchoServer serv{eventloop, "", 10101};

    serv.CoStart();

    while (1) {
        sleep(1);
    }
}