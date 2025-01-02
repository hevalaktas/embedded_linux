/*
 *  License     : MIT License
 *  Created on  : 2024
 *  Author      : Heval Aktaş
 *  Email       : hevalakts@gmail.com
 */

#include "I2Cdevice.h"
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>


namespace bbb
{
    /*The constructor opens a file descriptor to the I2C device*/
    I2Cdevice::I2Cdevice(uint32_t bus, uint32_t devaddr) : bus{bus}, device{devaddr}
    {
        open();
    }

    /*Open a connection to an I2C device.*/
    int I2Cdevice::open()
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
    int I2Cdevice::write(uint8_t val)
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
    int I2Cdevice::write_register(uint8_t regaddr, uint8_t val)
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
    char I2Cdevice::read_register(uint8_t regaddr)
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

    /*Read a number of registers from a single device.*/
    char *I2Cdevice::read_register(uint8_t number, uint8_t fromaddr)
    {
        write(fromaddr);
        auto *data = new char[number];

        if (::read(fd, data, number) != static_cast<int>(number))
        {
            std::cerr << "failed to read the buffer.\n";
            return nullptr;
        }
        return data;
    }

    void I2Cdevice::close()
    { 
        ::close(fd);
        fd = -1;
    }

    I2Cdevice::~I2Cdevice()
    { 
        if (fd != -1)
            close();
    }


}