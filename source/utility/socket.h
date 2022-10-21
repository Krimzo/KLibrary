#pragma once

#include "window/system_handler.h"
#include "math/math.h"


namespace kl {
    class socket
    {
    public:
        inline static const std::string self_ip = "127.0.0.1";
        static void init_utility();
        static void free_utility();
        
    private:
        sockaddr_in address_ = {};
        uint64_t socket_ = {};

    public:
        socket();
        socket(const std::string& address, uint32_t port);
        explicit socket(uint64_t socket);
        socket(const socket&) = delete;
        socket(const socket&&) = delete;
        ~socket();

        void operator=(const socket&) = delete;
        void operator=(const socket&&) = delete;

        explicit operator uint64_t() const;

        void close() const;

        [[nodiscard]] std::string get_address() const;

        void set_address(const std::string& address);

        [[nodiscard]] uint32_t get_port() const;

        void set_port(uint32_t port);

        void listen(uint32_t queue_size);

        [[nodiscard]] socket accept();

        void connect();

        int send(const void* data, uint32_t byte_size) const;

        int receive(void* buff, uint32_t byte_size) const;

        template <typename T>
        int send(const T& obj) const
        {
            return send(&obj, sizeof(T));
        }

        template <typename T>
        int receive(T& obj) const
        {
            return receive(&obj, sizeof(T));
        }
    };
}
