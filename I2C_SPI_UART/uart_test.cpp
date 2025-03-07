/*
 *  Description          : Desktop serial server exampke (UART)
 *  License              : MIT License
 *  Created on           : 2025
 *  Author               : Heval Aktaş
 *  Email                : hevalakts@gmail.com
 */
#include <iostream>
#include <cstring>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <map>
#include <fstream>


void set_terminal(int fd, struct termios &opt);
int print(int fd, const char *message);
int gpio_control(const char *filename, const char *value);
int execute_cmd(int fd, const char *command);

const std::string led_path{"/sys/class/gpio/gpio60/"};
const std::map<std::string, int> cmd_list{{"led on", 1},
                                          {"led off", 2},
                                          {"quit", 3}};

int main(int argc, char *argv[])        // argv[1] -> /dev/ttyS*
{
    int fd{-1};
    int cnt{0};
    uint8_t ch;
    char cmd_buffer[255]{0};

    struct termios opt;

    if (argc != 2) 
    {
        std::cerr << "wrong number of arguments!\n";
        return 1;
    }

    if ((fd = open(argv[1], O_RDWR | O_NOCTTY | O_NDELAY)) < 0)
    {
        std::cerr << "cannot open!\n";
    }

    set_terminal(fd, opt);
    gpio_control("direction", "out");
    print(fd, "\n\rUART Server Running\n\rUART > ");

    while (true)
    {
        if (read(STDIN_FILENO, &ch, 1) > 0)
        {
            write(fd, &ch, 1);
        }

        if (read(fd, &ch, 1) > 0)
        {
            write(STDOUT_FILENO, &ch, 1);

            cmd_buffer[cnt++] = ch;

            if (cnt >= 255)
                cnt = 0;

            if (ch == '\n')
            {
                cmd_buffer[cnt - 1] = '\0';
                auto q = execute_cmd(fd, cmd_buffer);
                cnt = 0;

                if (q == -1)
                    break;
                print(fd, "\rUART > ");
            }
        }
    }

    close(fd);
    return 0;
}

int print(int fd, const char *message)
{
    size_t size = strlen(message);
    if (write(fd, message, size) < 0)
    {
        std::cerr << "Failed to write\n";
        return -1;
    }

    return 0;
}

void set_terminal(int fd, struct termios &opt)
{
    tcgetattr(fd, &opt);

    opt.c_iflag = IGNPAR | ICRNL;
    opt.c_cflag = B115200 | CS8 | CREAD | CLOCAL;
    tcflush(fd, TCIOFLUSH);
    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);

    tcsetattr(fd, TCSANOW, &opt);
}

int execute_cmd(int fd, const char *command)
{
    std::ofstream ofs;
    int val = 0;
    if (auto iter = cmd_list.find(command); iter != cmd_list.end())
    {
        val = iter->second;
    }
    else
    {
        print(fd, "\rUnknown command!\n");
    }

    switch (val)
    {
    case 1:
        return gpio_control("value", "1");
    case 2:
        return gpio_control("value", "0");
    case 3:
        print(fd, "\rgoodbye!\n\r");
        return -1; // exit
    default:
        return 0;
    }
}

int gpio_control(const char *filename, const char *value)
{
    std::ofstream ofs{led_path + filename};
    if (!ofs)
    {
        std::cerr << "Failed to open gpio file!" << '\n';
    }
    ofs << value;

    return 0;
}
