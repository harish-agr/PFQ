#include <stdexcept>

#include <pfq.hpp>
using namespace net;

char packet[] = "\x01\x02\x04\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x08\x00"
                "\x45\x02\x03\x06\x05\x06\x03\x08\x03\x0a\x01\x0c"
                "\x45\x02\x04\x04\x05\x06\x05\x08\x02\x0a\x03\x0c";

int
main(int argc, char *argv[])
{
    if (argc < 2)
        throw std::runtime_error(std::string("usage: ").append(argv[0]).append(" dev"));

    const char *dev = argv[1];

    pfq q(128);

    q.bind(argv[1], pfq::any_queue);

    q.timestamp_enable(true);

    q.enable();

    q.test_();

    auto idx = net::nametoindex(dev);

    q.start_tx(0, idx, -1);


    for(int n = 0; n < 100000; ++n)
    {
        if (!q.sendto(net::const_buffer(packet, sizeof(packet)), idx))
            std::cout << "queue full!" << std::endl;

        q.wakeup_tx();
    }

    q.stop_tx();

    return 0;
}

