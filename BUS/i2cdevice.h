/*
 *  License     : MIT License
 *  Created on  : 2024
 *  Author      : Heval Aktaş
 *  Email       : hevalakts@gmail.com
 */

#ifndef I2CDEVICE_H_
#define I2CDEVICE_H_

#include <stdint.h>

namespace bbb
{

    class i2c_device
    {
    public:
        i2c_device(uint32_t bus, uint32_t devaddr);

        i2c_device(const i2c_device &) = delete;
        i2c_device &operator=(const i2c_device &) = delete;

        int open();

        int write(uint8_t val);
        int write_register(uint16_t regaddr, uint8_t val);

        char read_register(uint16_t regaddr);
        char *read_register(uint16_t number, uint8_t fromaddr = 0x00);

        void close();
        ~i2c_device();

    private:
        uint32_t bus;
        uint32_t device;
        char data[6]{0};

        int fd = -1;

        const char *bbb_12c_1 = "/dev/i2c-1";
        const char *bbb_12c_2 = "/dev/i2c-2";
    };

}

#endif