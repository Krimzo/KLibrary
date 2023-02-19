#include "klib.h"


static void server()
{
    kl::socket server(kl::socket::self_ip, 1709);

    server.listen(1);

    if (const kl::socket client = server.accept(); client.send<kl::float3>({ 1.0f, 2.0f, 3.0f })) {
        kl::print("Data sent!");
    }
}

static void client()
{
    kl::socket client(kl::socket::self_ip, 1709);

    client.connect();

    kl::float3 result = {};
    client.receive(result);
    print("Received: ", result);
}

int main()
{
    std::thread(server).detach();
    kl::time::wait(0.25f);
    std::thread(client).detach();


    kl::get();
}
