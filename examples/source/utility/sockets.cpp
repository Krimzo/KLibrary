#include "examples.h"


static void server()
{
    kl::socket server(1709);
    server.listen(1);

    kl::socket client = server.accept();
    if (client.send<kl::float3>({ 1.0f, 2.0f, 3.0f })) {
        kl::print("Data sent!");
    }
}

static void client()
{
    kl::time::wait(0.25f);

    kl::socket client(1709);
    client.connect();

    kl::float3 result = {};
    client.receive(result);
    print("Received: ", result);
}

int examples::sockets_main()
{
    auto server_thread = std::thread(server);
    auto client_thread = std::thread(client);
    server_thread.join();
    client_thread.join();
    return 0;
}
