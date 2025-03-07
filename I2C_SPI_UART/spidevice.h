/*
 *  Description : Simple SPI interface
 *  License     : MIT License
 *  Created on  : 2025
 *  Author      : Heval Aktaş
 *  Email       : hevalakts@gmail.com
 */

#ifndef SPIDEVICE_H_
#define SPIDEVICE_H_

#include <string>
#include <stdint.h>
#include <linux/spi/spidev.h>

#define SPI_PATH "/dev/spidev"

namespace bbb
{

    class spi_device
    {
    public:
        spi_device(uint16_t bus, uint16_t dev);

        spi_device(spi_device &&);
        spi_device &operator=(spi_device &&other);

        int set_mode(uint8_t mode);
        int set_bits_per_word(uint8_t bits);
        int set_speed(uint32_t speed);

        int transfer(uint8_t tx[], uint8_t rx[], int length);

        uint8_t read_reg(uint8_t regaddr);
        int write(uint8_t value);
        int write(uint8_t value[], int lenght);
        int write_reg(uint8_t regaddr, uint8_t value);

        void spi_test(uint8_t rx[], size_t length);
        int get_lsb();

        void close(int fd);
        int open();

        ~spi_device();

    private:
        uint8_t mode;
        uint8_t bits;
        uint32_t speed;
        uint16_t delay;

        int fd;
        std::string device;
    };
}

#endif
