/*
 *  License     : MIT License
 *  Created on  : 2024
 *  Author      : Heval Aktaş
 *  Email       : hevalakts@gmail.com
 */


#ifndef I2CDEVICE_H_
#define I2CDEVICE_H_

#include <stdint.h>



namespace bbb{

    class I2Cdevice{
    public:
        I2Cdevice(uint32_t bus, uint32_t devaddr);  

        I2Cdevice(const I2Cdevice&) = delete;
        I2Cdevice& operator=(const I2Cdevice&) = delete;

        int open();                             

        int write(uint8_t val);
        int write_register(uint8_t regaddr, uint8_t val);

        char read_register(uint8_t regaddr);
        char* read_register(uint8_t number, uint8_t fromaddr=0x00);

        void close();
        ~I2Cdevice();
    private:
        uint32_t bus;
        uint32_t device;
        
        int fd = -1;

        const char* bbb_12c_1 = "/dev/i2c-1";
        const char* bbb_12c_2 = "/dev/i2c-2";
    };

}







#endif