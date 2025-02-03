/*
 *  Description : Simple SPI interface
 *  License     : MIT License
 *  Created on  : 2025
 *  Author      : Heval Aktaş
 *  Email       : hevalakts@gmail.com
 */

#include "spidevice.h"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <cstring>

#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

namespace bbb
{

    spi_device::spi_device(uint16_t bus, uint16_t dev) : fd{-1}, mode{0}, bits{8}, speed{500'000}, delay{0}
    {
        std::ostringstream oss;
        oss << SPI_PATH << bus << "." << dev;
        device = oss.str();

        open();
    }

    spi_device::spi_device(spi_device &&other) : fd{other.fd}, mode{other.mode},
                                                 bits{other.bits}, speed{other.speed},
                                                 delay{other.delay},
                                                 device{std::move(other.device)}
    {
        other.fd = -1;
    }

    spi_device &spi_device::operator=(spi_device &&other)
    {
        if (this == &other)
            return *this;

        mode = other.mode;
        bits = other.bits;
        speed = other.speed;
        delay = other.delay;
        device = std::move(other.device);

        fd = other.fd;
        other.fd = -1;

        return *this;
    }

    int spi_device::open()
    {
        if ((fd = ::open(device.c_str(), O_RDWR)) < 0)
        {
            std::cerr << "Failed to open SPI device.\n";
            return 1;
        }

        if (set_mode(mode) == -1)
            return -1;
        if (set_speed(speed) == -1)
            return -1;
        if (set_bits_per_word(bits) == -1)
            return -1;

        return 0;
    }

    int spi_device::transfer(uint8_t tx[], uint8_t rx[], int length)
    {
        spi_ioc_transfer tr;
        tr.tx_buf = reinterpret_cast<uint64_t>(tx);
        tr.rx_buf = reinterpret_cast<uint64_t>(rx);
        tr.len = length;

        tr.speed_hz = speed;
        tr.bits_per_word = bits;
        tr.delay_usecs = delay;

        tr.tx_nbits = 0;
        tr.rx_nbits = 0;
        tr.cs_change = 0;
        tr.pad = 0;

        int ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
        if (ret < 0)
        {
            std::cerr << "Can't send SPI message\n";
            return -1;
        }

        return ret;
    }

    void spi_device::spi_test(uint8_t rx[], size_t length)
    {

        std::cout
            << std::dec
            << "device  : " << device << '\n'
            << "mode  : " << static_cast<int>(mode) << '\n'
            << "bits  : " << static_cast<int>(bits) << '\n'
            << "speed : " << static_cast<int>(speed) << '\n';

        for (size_t i = 0; i < length; i++)
        {
            if (!(i % 6))
                std::cout << "\n";

            std::cout << std::hex
                      << std::uppercase
                      << std::setw(2)
                      << std::setfill('0')
                      << static_cast<int>(rx[i])
                      << " ";
        }

        std::cout << '\n';
    }

    uint8_t spi_device::read_reg(uint8_t regaddr)
    {
        uint8_t tx_buf[2];
        uint8_t rx_buf[2];
        tx_buf[0] = 0x80 | regaddr;
        tx_buf[1] = 0;
        memset(rx_buf, 0, sizeof(rx_buf));

        transfer(tx_buf, rx_buf, 2);
        return rx_buf[1];
    }

    int spi_device::write(uint8_t value)
    {
        transfer(&value, nullptr, 1);
        return 0;
    }

    int spi_device::write(uint8_t value[], int lenght)
    {
        transfer(value, nullptr, lenght);
        return 0;
    }

    int spi_device::write_reg(uint8_t regaddr, uint8_t value)
    {
        uint8_t tx_buf[2] = {regaddr, value};
        uint8_t rx_buf[2];
        memset(rx_buf, 0, sizeof(rx_buf));

        transfer(tx_buf, rx_buf, 2);
        return 0;
    }

    int spi_device::set_speed(uint32_t hz)
    {
        speed = hz;

        if (ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) == -1)
        {
            std::cerr << "SPI : Can't set max speed HZ";
            return -1;
        }
        if (ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed) == -1)
        {
            std::cerr << "SPI : Can't get max speed HZ.";
            return -1;
        }

        return 0;
    }

    int spi_device::set_mode(uint8_t m)
    {
        mode = m;

        if (ioctl(fd, SPI_IOC_WR_MODE, &mode) == -1)
        {
            std::cerr << "SPI : Can't set SPI mode.";
            return -1;
        }
        if (ioctl(fd, SPI_IOC_RD_MODE, &mode) == -1)
        {
            std::cerr << "SPI : Can't get SPI mode.";
            return -1;
        }

        return 0;
    }

    int spi_device::set_bits_per_word(uint8_t bit)
    {
        bits = bit;

        if (ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits) == -1)
        {
            std::cerr << "SPI : Can't set bits per word.";
            return -1;
        }
        if (ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits) == -1)
        {
            std::cerr << "SPI : Can't get bits per word.";
            return -1;
        }

        return 0;
    }

    void spi_device::close(int fd)
    {
        ::close(fd);
        fd = -1;
    }

    spi_device::~spi_device()
    {
        if (fd != -1)
        {
            close(fd);
        }
    }
}
