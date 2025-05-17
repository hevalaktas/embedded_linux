/*
 *  Description : Simple Client Socket Test
 *  License     : MIT License
 *  Created on  : 2025
 *  Author      : Heval Aktaş
 *  Email       : hevalakts@gmail.com
 */
#include "socket.hpp"

#include <iostream>
#include <array>
#include <string>
#include <thread>
#include <chrono>

static constexpr const int buffer_size = 1024;

using stype = bbb::socket<bbb::mode::client_t,
                          bbb::ip::ipv4,
                          bbb::con::tcp>;

int main(int argc, char *argv[])
{
    using namespace std::chrono;

    char buf[buffer_size + 1];

    if (argc != 3)
    {
        std::cerr << "Usage: ./client <hostname> <port>\n";
        return -1;
    }

    stype client{argv[1], argv[2]};

    for (;;)
    {
        std::cout << "client > " << std::flush;

        if (!std::cin.getline(buf, buffer_size))
            continue;

        ssize_t sent = client.send(client.fd(), buf, strlen(buf));
        if (sent == -1)
            std::cerr << "Send failed!\n";
        else
            std::cout << "Sent " << sent << " bytes to server\n";

        if (!strcmp(buf, "quit"))
            break;

        std::memset(buf, 0, buffer_size);
        std::this_thread::sleep_for(50ms);

        ssize_t len = client.receive(client.fd(), buf, buffer_size);
        if (len > 0)
        {
            std::cout << "Received from server " << ": " << buf << '\n';
        }

    }

}
