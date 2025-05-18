/*
 *  Description : Simple POSIX Socket Wrapper
 *                - Supports TCP over IPv4 and IPv6
 *                - Server and Client modes
 *                - Accepts connections, sends/receives data
 *  License     : MIT License
 *  Created on  : 2025
 *  Author      : Heval Aktaş
 *  Email       : hevalakts@gmail.com
 */
#ifndef SOCKET_HPP_
#define SOCKET_HPP_


#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>

#include <poll.h>

#include <iostream>
#include <type_traits>
#include <cstring>
#include <array>
#include <functional>
#include <mutex>

namespace bbb
{

    namespace ip /* domain types */
    {
        template <typename T, int d, int l>
        struct domain_traits
        {
            static constexpr int domain = d;
            static constexpr int addrlen = l;
            using sockaddr_type = T;
            using ip_addr_type = std::conditional_t<
                std::is_same_v<T, sockaddr_in>,
                uint32_t,
                in6_addr>;
        };

        using ipv4 = domain_traits<struct sockaddr_in, AF_INET, INET_ADDRSTRLEN>;
        using ipv6 = domain_traits<struct sockaddr_in6, AF_INET6, INET6_ADDRSTRLEN>;

    }

    namespace con /* Connection types / transport types */
    {

        using tcp = std::integral_constant<int, SOCK_STREAM>;
        using udp = std::integral_constant<int, SOCK_DGRAM>;

    }

    namespace mode /* modes */
    {

        template <std::size_t MaxClient>
        struct server_t
        {
            static constexpr std::size_t max_clients = MaxClient;
        };

        struct client_t {};

    }

    //-----------------------------------------------------------------------------------------------
    //-----------------------------------------------------------------------------------------------

    struct utils
    {
        utils() = default;
        utils(const utils &) = delete;
        utils &operator=(const utils &) = delete;

        static ssize_t send(int sock_fd, const char *data, ssize_t size)
        {
            ssize_t result = ::send(sock_fd, data, size, 0);
            if (result == -1)
                return errno == EAGAIN ? -1 : 0;
            if (result == 0)
                return 0;

            return result;
        }
        static ssize_t receive(int sock_fd, char *buffer, ssize_t size)
        {
            ssize_t len = ::recv(sock_fd, buffer, size, 0);
            if (len == -1)
                return errno == EAGAIN ? -1 : 0;
            if (len == 0)
                return 0;

            return len;
        }
    };

    template <typename Domain>
    class server_ops
    {
        using sockaddr_type = typename Domain::sockaddr_type;
        using ip_type = typename Domain::ip_addr_type;

    protected:
        sockaddr *make_addr(int port, ip_type ip)
        {
            addr = {};

            if constexpr (std::is_same_v<sockaddr_type, sockaddr_in>)
            {
                addr.sin_family = Domain::domain;
                addr.sin_port = htons(port);
                addr.sin_addr.s_addr = htonl(ip);
            }
            else
            {
                addr.sin6_family = Domain::domain;
                addr.sin6_port = htons(port);
                addr.sin6_addr = ip;
            }

            return reinterpret_cast<sockaddr *>(&addr);
        }

        sockaddr *make_empty_addr()
        {
            addr = {};
            return reinterpret_cast<sockaddr *>(&addr);
        }

        static auto any_addr()
        {
            if constexpr (std::is_same_v<sockaddr_type, sockaddr_in>)
                return INADDR_ANY;
            else
                return in6addr_any;
        }

        void fill_ip_port(std::pair<std::array<char, Domain::addrlen>, int> &out)
        {
            if constexpr (std::is_same_v<sockaddr_type, sockaddr_in>)
            {
                inet_ntop(Domain::domain, &addr.sin_addr, out.first.data(), out.first.size());
                out.second = static_cast<int>(ntohs(addr.sin_port));
            }
            else
            {
                inet_ntop(Domain::domain, &addr.sin6_addr, out.first.data(), out.first.size());
                out.second = static_cast<int>(ntohs(addr.sin6_port));
            }
        }

        socklen_t length() const noexcept { return sizeof(addr); }

        sockaddr_type addr{};
    };

    template <typename Mode, typename Domain = ip::ipv4, typename ConT = con::tcp>
    class socket : utils {};

    template <std::size_t N, typename Domain, typename ConT>
    class socket<mode::server_t<N>, Domain, ConT> : public utils, public server_ops<Domain>
    {
    public:
        using ops = server_ops<Domain>;
        using ip_type = typename Domain::ip_addr_type;
        using endpoint_type = std::pair<std::array<char, Domain::addrlen>, int>;
        using event_handler_t = std::function<int(std::size_t)>;

        explicit socket(int port, ip_type ip_addr = ops::any_addr())
        try
        {
            int sock = ::socket(Domain::domain, ConT{}, 0);
            if (sock == -1)
                throw "socket!";

            if (fcntl(sock, F_SETFL, fcntl(sock, F_GETFL) | O_NONBLOCK) == -1)
                throw "fcntl";

            if (::bind(sock, ops::make_addr(port, ip_addr), ops::length()) == -1)
                throw "bind!";

            if (::listen(sock, 32) == -1)
                throw "listen!";

            client_info[0].second = port;
            poll_fd[0].fd = sock;
            poll_fd[0].events = POLLIN;
            npfds = 1;

            std::cout << "Server : listening on port " << port << '\n';
        }
        catch (const char *ex)
        {
            std::cerr << ex << '\n';

            if (poll_fd[0].fd != -1)
            {
                ::close(poll_fd[0].fd);
            }
            std::exit(EXIT_FAILURE);
        }

        void poll(int timeout, event_handler_t &&event_handler)
        {
            if (::poll(poll_fd.data(), npfds, timeout) == -1)
            {
                if (errno == EINTR)
                {
                    return;
                }
                throw "poll!";
            }

            int count = npfds;

            for (int i{0}; i < count; ++i)
            {
                if (revents(i) & POLLIN)
                {
                    if (is_server_fd(i))
                    {
                        std::cout << "Client connected: fd : " << accept() << '\n';
                    }
                    else
                    {
                        int ret = event_handler(i);
                        if (ret == -1)
                        {
                            std::cout << "no data : " << poll_fd[i].fd << '\n';
                        }
                        else if (ret == 0)
                        {
                            std::cout << "disconnect : " << poll_fd[i].fd << '\n';
                            disconnect(i);
                        }
                    }
                }
            }
        }

        int accept()
        {
            if (npfds >= N + 1)
            {
                std::cerr << "Max client limit reached!\n";
                return -1;
            }

            socklen_t len = ops::length();
            int client_sock = 0;
            if ((client_sock = ::accept(poll_fd[0].fd, ops::make_empty_addr(), &len)) == -1)
                throw "accept!";

            if (fcntl(client_sock, F_SETFL, fcntl(client_sock, F_GETFL) | O_NONBLOCK) == -1)
                throw "fcntl(client_sock)!";

            {
                std::lock_guard<std::mutex> lock(mtx);

                ops::fill_ip_port(client_info[npfds]);

                poll_fd[npfds].fd = client_sock;
                poll_fd[npfds].events = POLLIN;
                ++npfds;
            }

            return client_sock;
        }

        int operator[](std::size_t idx) const noexcept
        {
            std::lock_guard<std::mutex> lock(mtx);

            if (idx >= npfds)
                return -1;

            return poll_fd[idx].fd;
        }

        size_t size() const
        {
            std::lock_guard<std::mutex> lock(mtx);

            return npfds - 1;
        }

        const endpoint_type &endpoint(std::size_t idx) const
        {
            std::lock_guard<std::mutex> lock(mtx);

            if (idx >= npfds)
                throw "Invalid client index";

            return client_info[idx];
        }

        ~socket()
        {
            for (std::size_t i = 0; i < npfds; ++i)
                close(poll_fd[i].fd);
        }

    private:
        short int revents(int idx) const
        {
            return poll_fd[idx].revents;
        }

        bool is_server_fd(size_t idx)
        {
            return poll_fd[idx].fd == poll_fd[0].fd;
        }

        void close(int fd)
        {
            ::shutdown(fd, SHUT_RDWR);
            ::close(fd);
        }

        void disconnect(int idx)
        {
            std::lock_guard<std::mutex> lock(mtx);

            close(poll_fd[idx].fd);
            client_info[idx] = client_info[npfds - 1];
            poll_fd[idx] = poll_fd[npfds - 1];
            --npfds;
        }

    private:
        mutable std::mutex mtx;

        std::size_t npfds{0}; // number of poll file descriptors
        std::array<struct pollfd, N + 1> poll_fd{{{-1}}};
        std::array<endpoint_type, N + 1> client_info{};
    };

    //-----------------------------------------------------------------------------------------------
    //-----------------------------------------------------------------------------------------------

    template <typename Domain, typename ConT>
    class socket<mode::client_t, Domain, ConT> : public utils
    {
    public:
        socket(const char *host, const char *port)
        try
        {
            sock = ::socket(Domain::domain, ConT::value, 0);
            if (sock == -1)
            {
                throw "socket!";
            }

            connect(host, port, Domain::domain, ConT::value);
        }
        catch (const char *ex)
        {
            std::cerr << ex << '\n';
            if (sock != -1)
            {
                ::close(sock);
            }
            std::exit(EXIT_FAILURE);
        }

        int fd() const noexcept
        {
            return sock;
        }

        ~socket()
        {
            if (sock != -1)
                close(sock);
        }

    private:
        void connect(const char *host, const char *port, int domain, int type)
        {

            struct addrinfo hints{0, domain, type};
            struct addrinfo *res = nullptr;
            struct addrinfo *ri;
            int gai_result;

            if ((gai_result = getaddrinfo(host, port, &hints, &res)) != 0)
            {
                std::cerr << gai_strerror(gai_result) << '\n';
                throw "getaddrinfo!";
            }

            for (ri = res; ri != nullptr; ri = ri->ai_next)
            {
                if (::connect(sock, ri->ai_addr, ri->ai_addrlen) != -1)
                    break;
            }

            if (ri == nullptr)
            {
                std::cerr << strerror(errno) << '\n';
                freeaddrinfo(res);
                throw "connection failed!";
            }

            if (fcntl(sock, F_SETFL, fcntl(sock, F_GETFL) | O_NONBLOCK) == -1)
            {
                throw "fcntl(client socket)!";
            }

            freeaddrinfo(res);
        }

        void close(int fd)
        {
            ::shutdown(fd, SHUT_RDWR);
            ::close(fd);
        }

    private:
        int sock = -1;
    };
}

template <typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &ep)
{
    os << "[";
    for (const auto &c : ep.first)
        os << c;
    os << " : " << ep.second << "]\n";

    return os;
}

#endif