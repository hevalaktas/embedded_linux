/*
 *  License     : MIT License
 *  Created on  : 2024
 *  Author      : Heval Aktaş
 *  Email       : hevalakts@gmail.com
 */


#ifndef MPU6050_H_
#define MPU6050_H_

#include <stdint.h>
#include <array>
#include "i2cdevice.h"



namespace bbb
{

    enum class axis { x, y, z };

    struct  with_clb{
        const static uint8_t size {3};  // This is for calibration data. There are only two tags.
    };

    struct without_clb{
        const static uint8_t size {0};
    };


    template <typename T = without_clb>
    class mpu6050
    {

    public:
        explicit mpu6050(int scale = 0);

        // mpu6050(const mpu6050 &) = delete;               // implicitly declared because of i2c_device 
        // mpu6050 &operator=(const mpu6050 &) = delete;

        void read_acc();
        void read_gyr();

        short get_acc_raw(axis)const;
        short get_gyr_raw(axis)const;

        double get_acc_val(axis)const;
        double get_gyr_val(axis)const;

        double get_roll()const;               
        double get_pitch()const;

        ~mpu6050();

    private:
        const uint8_t power{0x6B};
        const uint8_t acc_config{0x1C};
        const uint8_t gyr_config{0x1B};

        const uint32_t slave_addr{0x68};

        const uint8_t reg_acc[6] = {0x3B, 0x3C, 0x3D, 0x3E, 0x3F, 0x40}; // The addresses of accelerometer x, y, z high and low values.
        const uint8_t reg_gyr[6] = {0x43, 0x44, 0x45, 0x46, 0x47, 0x48}; // The addresses of gyroscope x, y, z high and low values.

        double acc_sens;
        double gyr_sens;
        uint8_t fs_range;

        i2c_device m_i2c{2, slave_addr};

        void set_sensitivity(int sens);
        void init();
        void calibration_gyr(std::array<double, T::size> &data_clb);

        short acc_raw[3]{0};
        short gyr_raw[3]{0};
        std::array<double, T::size> data_clb;
    };
}

#include "mpu6050.hpp"


#endif