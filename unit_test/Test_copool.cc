#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

#include <bbt/conet/detail/CoPool.hpp>

using namespace bbt::network::conet;

BOOST_AUTO_TEST_SUITE()

BOOST_AUTO_TEST_CASE(t_copool)
{
    g_scheduler->Start();
    const int ntask = 100000;
    bbt::thread::CountDownLatch l{ntask};

    bbtco_desc("main") [&](){
        detail::CoPool co{100};
        auto chan = bbt::coroutine::Chan<int, 65535>();
        for (int i = 0; i < ntask; ++i) {
            co.Submit([&](){
                chan << 1;
                l.Down();
            });
        }

        int a;
        for (int i = 0; i < ntask; ++i)
            chan >> a;
        
    };

    l.Wait();
    g_scheduler->Stop();
    BOOST_ASSERT(true);
}

BOOST_AUTO_TEST_SUITE_END()