/*
 *  License     : MIT License
 *  Created on  : 2024
 *  Author      : Heval Aktaş
 *  Email       : hevalakts@gmail.com
 */

#include "i2cdevice.h"
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

namespace bbb
{
    /*The constructor opens a file descriptor to the I2C device*/
    i2c_device::i2c_device(uint32_t bus, uint32_t devaddr) : bus{bus}, device{devaddr}
    {
        open();
    }

    /*Open a connection to an I2C device.*/
    int i2c_device::open()
    {
        const char *device_name = bus == 1 ? bbb_12c_1 : bbb_12c_2;

        if ((fd = ::open(device_name, O_RDWR)) < 0)
        {
            std::cerr << "Failed to open I2C device file.\n";
            return 1;
        }

        if (ioctl(fd, I2C_SLAVE, device) < 0)
        {
            std::cerr << "Failed to set I2C slave address.\n";
            return 1;
        }

        return 0;
    }

    /* Write a single value to the I2C device.*/
    int i2c_device::write(uint8_t val)
    {
        uint8_t buffer[1];
        buffer[0] = val;

        if (::write(fd, buffer, 1) != 1)
        {
            std::cerr << "failed to write I2C device file.\n";
            return 1;
        }
        return 0;
    }

    /* Write a single byte value to a single register.*/
    int i2c_device::write_register(uint16_t regaddr, uint8_t val)
    {
        uint8_t buffer[2];
        buffer[0] = regaddr;
        buffer[1] = val;

        if (::write(fd, buffer, 2) != 2)
        {
            std::cerr << "failed to write I2C device file.\n";
            return 1;
        }
        return 0;
    }

    /* Read a single byte register value from the address.*/
    char i2c_device::read_register(uint16_t regaddr)
    {
        write(regaddr);
        char buffer[1];

        if (::read(fd, buffer, 1) != 1)
        {
            std::cerr << "failed to read I2C device file.\n";
            return 1;
        }
        return buffer[0];
    }

    /*Read a number of registers from a single device but the maximum data size is 6.*/
    char *i2c_device::read_register(uint16_t number, uint8_t fromaddr)
    {
        write(fromaddr);

        if (::read(fd, data, number) != static_cast<int>(number))
        {
            std::cerr << "failed to read the buffer.\n";
            return nullptr;
        }
        return data;
    }

    void i2c_device::close()
    {
        ::close(fd);
        fd = -1;
    }

    i2c_device::~i2c_device()
    {
        if (fd != -1)
            close();
    }

}