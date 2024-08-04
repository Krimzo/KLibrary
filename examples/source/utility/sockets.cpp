#include "examples.h"


void server_tcp();
void client_tcp();

void server_udp();
void client_udp();

int examples::sockets_main(const int argc, const char** argv)
{
    std::vector<std::thread> threads{};
    if (true) {
        kl::print("Testing TCP");
        threads.emplace_back(server_tcp);
        threads.emplace_back(client_tcp);
    }
    else {
        kl::print("Testing UDP");
        threads.emplace_back(server_udp);
        threads.emplace_back(client_udp);
    }
    for (auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
    return 0;
}

void server_tcp()
{
    kl::Socket server{ false };
    server.set_port(1709);
    server.bind();
    server.listen(1);

    kl::Socket client{ false };
    server.accept(&client);

    client.send<kl::Float3>({ 1.0f, 2.0f, 3.0f });
    kl::print("TCP data sent!");
}

void client_tcp()
{
    kl::time::wait(0.25f);

    kl::Socket client{ false };
    client.set_port(1709);
    client.set_address("127.0.0.1");
    client.connect();

    kl::Float3 result{};
    client.receive(&result);
    kl::print("Received: ", result);
}

void server_udp()
{
    kl::Socket server{ true };
    server.set_port(1709);
    server.bind();

    int recieve_data{};
    kl::Address address{};
    server.receive_from(&recieve_data, &address);

    kl::Float3 send_data{ 4.0f, 2.0f, 0.0f };
    server.send_to(send_data, address);
    kl::print("UDP data sent!");
}

void client_udp()
{
    kl::time::wait(0.25f);
    kl::Socket client{ true };

    kl::Address host_address{};
    host_address.set_port(1709);
    host_address.set_address("127.0.0.1");

    int send_data = 1;
    client.send_to(send_data, host_address);

    kl::Float3 recieve_data{};
    client.receive(&recieve_data);
    kl::print("Received: ", recieve_data);
}
