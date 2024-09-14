#include <bbt/conet/conet.hpp>
#include "echoconnection.hpp"

class EchoClient:
    public bbt::conet::TcpClient
{
public:

    EchoClient(std::shared_ptr<bbt::conet::detail::EventLoop> eventloop):bbt::conet::TcpClient(eventloop) {}
    virtual ~EchoClient() {}

    virtual void OnConnect(int socket, const bbt::conet::IPAddress& addr)
    {
        auto conn = std::make_shared<EchoConn>(GetEventLoop(), socket, addr, 1000);
        bbtco [conn]{
            for (int i = 0; i < 10000; ++i) {
                conn->Send(bbt::buffer::Buffer{"hello world"});
            }
        };
    }

    virtual void OnError(const bbt::network::Errcode& err)
    {
        printf("[echo serv] %s\n", err.CWhat());
    }
};

int main()
{
    auto eventloop = std::make_shared<bbt::conet::detail::EventLoop>(100, true);

    EchoClient client{eventloop};

    client.CoConnect("127.0.0.1", 10101);

    while(true) {
        sleep(1);
    }
}