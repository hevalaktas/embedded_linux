/*
 *  Description : Simple Server Socket Test
 *  License     : MIT License
 *  Created on  : 2025
 *  Author      : Heval Aktaş
 *  Email       : hevalakts@gmail.com
 */
#include "socket.hpp"

#include <iostream>
#include <array>
#include <algorithm>
#include <thread>
#include <csignal>

static constexpr const int buffer_size = 1024;

template <int N>
using stype = bbb::socket<bbb::mode::server_t<N>,
                          bbb::ip::ipv4,
                          bbb::con::tcp>;

bool running = true;

int main(int argc, char *argv[])
{

    static int server_fd{-1};
    auto sig_handler = [](int)
    {
        running = false;
        std::cout << "\nSIGINT received. Shutting down...\n";

        if (server_fd >= 0)
            shutdown(server_fd, SHUT_RDWR);
    };

    std::signal(SIGINT, sig_handler);
    char buf[buffer_size + 1];
    int port;

    if (argc != 2)
    {
        std::cerr << "Usage: ./server <port>\n";
        return -1;
    }

    port = std::stoi(argv[1]);
    std::cout << port << '\n';

    stype<10> server{port};
    server_fd = server[0];

    /* Reverses received data and sends back */
    auto handler = [&server, &buf](std::size_t idx) -> int
    {
        int fd = server[idx];
        ssize_t len = server.receive(fd, buf, buffer_size);
        if (len > 0)
        {
            std::cout << "Received from "
                      << fd << " : "
                      << server.endpoint(idx)
                      << buf << '\n';

            buf[len] = '\0';
            if (!strcmp(buf, "quit"))
                return 0;

            std::reverse(buf, buf + len);
            server.send(fd, buf, len);
        }

        std::memset(buf, 0, len);

        return len;
    };

    try
    {
        while (running)
        {
            server.poll(-1, handler);
        }
    }
    catch (const char *ex)
    {
        std::cerr << ex << '\n';
    }

    std::cout << "closed!!! " << "\n";
}