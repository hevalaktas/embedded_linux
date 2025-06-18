/*
 *  Description : Simple GPIO interface. m_file always binds the file of
 *                value but other operations can also be done.
 *  License     : MIT License
 *  Created on  : 2025
 *  Author      : Heval Aktaş
 *  Email       : hevalakts@gmail.com
 */
#include "gpio.h"
#include <iostream>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <thread>

bbb::gpio::gpio(uint16_t number) : m_number{number}, m_path{gpio_path}
{
    m_path += "gpio" + std::to_string(number) + "/";

    gpio_export();
    open_file((m_path + value).c_str());
}

bbb::gpio::gpio(uint16_t number, bbb::direction dir) : m_number{number}, m_path{gpio_path}
{
    m_path += "gpio" + std::to_string(number) + "/";

    gpio_export();
    set_direction(dir);
}

int bbb::gpio::open_file(const char *filename)
{
    m_file.open(filename);
    if (!m_file.is_open())
    {
        std::cerr << filename << " : the file cannot be opened \n";
        return -1;
    }
    return 0;
}

int bbb::gpio::set(const char *file, const char *val)
{
    m_file.close();

    if (open_file(file) == -1)
    {
        return -1;
    }
    m_file << val;
    m_file.close();

    return 0;
}

int bbb::gpio::set(const char *file, int val)
{
    m_file.close();

    if (open_file(file) == -1)
    {
        return -1;
    }
    m_file << val;
    m_file.close();

    return 0;
}

std::string bbb::gpio::get(const char *file)
{
    m_file.close();
    std::string info;

    open_file(file);
    getline(m_file, info);
    m_file.close();

    return info;
}

int bbb::gpio::set_direction(bbb::direction dir)
{
    if (dir == bbb::direction::in)
    {
        set((m_path + direction).c_str(), "in");
    }
    else
    {
        set((m_path + direction).c_str(), "out");
    }

    return open_file((m_path + value).c_str());
}

std::string bbb::gpio::get_direction()
{
    auto info = get((m_path + direction).c_str());

    open_file((m_path + value).c_str());

    return info;
}

int bbb::gpio::set_value(bbb::value val)
{
    m_file << static_cast<bool>(val);
    m_file.seekp(0);

    return 0;
}

int bbb::gpio::get_value()
{
    int value;
    m_file >> value;
    m_file.seekg(0);

    return value;
}

int bbb::gpio::set_active_low(bool act_low)
{
    if (set(active_low, act_low) == -1)
    {
        return -1;
    }
    return open_file((m_path + value).c_str());
}

int bbb::gpio::set_active_high()
{
    return set_active_low(false);
}

int bbb::gpio::gpio_export()
{
    int fd;
    if ((fd = ::open(m_path.c_str(), O_RDONLY)) == -1)
    {
        if (errno == ENOENT)
        {
            return set((std::string{gpio_path} + export_p).c_str(), m_number);
        }
    }
    std::this_thread::sleep_for(std::chrono::milliseconds{200});
    ::close(fd);

    return 0;
}

int bbb::gpio::gpio_unexport()
{
    return set((std::string{gpio_path} + unexport_p).c_str(), m_number);
}

bbb::gpio::~gpio() {}