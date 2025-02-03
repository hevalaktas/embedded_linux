/*
 *  License     : MIT License
 *  Created on  : 2024
 *  Author      : Heval Aktaş
 *  Email       : hevalakts@gmail.com
 */

#ifndef MPU6050_HPP_
#define MPU6050_HPP_

#include <thread>
#include <math.h>

namespace bbb
{
    template <typename T>
    using ms = std::chrono::duration<T, std::milli>;

    template <typename T>
    mpu6050<T>::mpu6050(int scale)
    {
        set_sensitivity(scale);
        init();

        if constexpr (std::is_same_v<T, with_clb>)
        {
            data_clb = {0, 0, 0};
            calibration_gyr(data_clb);
        }
    }

    template <typename T>
    void mpu6050<T>::set_sensitivity(int scale)
    {
        switch (scale)
        {
        default:
            acc_sens = 16384;
            gyr_sens = 131;
            fs_range = 0x00;
            break;
        case 1:
            acc_sens = 8192;
            gyr_sens = 65.5;
            fs_range = 0x08;
            break;
        case 2:
            acc_sens = 4096;
            gyr_sens = 32.8;
            fs_range = 0x10;
            break;
        case 3:
            acc_sens = 2048;
            gyr_sens = 16.4;
            fs_range = 0x18;
            break;
        }
    }

    template <typename T>
    void mpu6050<T>::init()
    {
        using namespace std::chrono;

        m_i2c.write_register(power, 0x00); // disable sleep mode
        std::this_thread::sleep_for(ms<uint16_t>{1});

        m_i2c.write_register(acc_config, fs_range); // Full Scale Range
        std::this_thread::sleep_for(ms<uint16_t>{1});

        m_i2c.write_register(gyr_config, fs_range); // Full Scale Range
        std::this_thread::sleep_for(ms<uint16_t>{1});
    }

    template <typename T>
    void mpu6050<T>::read_acc()
    {
        auto *acc_buffer = m_i2c.read_register(6, reg_acc[0]);

        acc_raw[0] = (int16_t)((acc_buffer[0] << 8) | acc_buffer[1]);
        acc_raw[1] = (int16_t)((acc_buffer[2] << 8) | acc_buffer[3]);
        acc_raw[2] = (int16_t)((acc_buffer[4] << 8) | acc_buffer[5]);
    }

    template <typename T>
    void mpu6050<T>::read_gyr()
    {
        auto *gyr_buffer = m_i2c.read_register(6, reg_gyr[0]);

        gyr_raw[0] = ((gyr_buffer[0] << 8) | gyr_buffer[1]);
        gyr_raw[1] = ((gyr_buffer[2] << 8) | gyr_buffer[3]);
        gyr_raw[2] = ((gyr_buffer[4] << 8) | gyr_buffer[5]);
    }

    template <typename T>
    short mpu6050<T>::get_acc_raw(axis a) const
    {
        if (a == axis::x)
            return acc_raw[0];
        else if (a == axis::y)
            return acc_raw[1];
        else
            return acc_raw[2];
    }

    template <typename T>
    short mpu6050<T>::get_gyr_raw(axis a) const
    {
        if (a == axis::x)
            return gyr_raw[0];
        else if (a == axis::y)
            return gyr_raw[1];
        else
            return gyr_raw[2];
    }

    template <typename T>
    double mpu6050<T>::get_acc_val(axis a) const
    {
        if (a == axis::x)
            return acc_raw[0] / acc_sens;
        else if (a == axis::y)
            return acc_raw[1] / acc_sens;
        else
            return acc_raw[2] / acc_sens;
    }

    template <typename T>
    double mpu6050<T>::get_gyr_val(axis a) const
    {
        if constexpr (std::is_same_v<T, with_clb>)
        {
            if (a == axis::x)
                return (gyr_raw[0] / gyr_sens) - data_clb[0];
            else if (a == axis::y)
                return (gyr_raw[1] / gyr_sens) - data_clb[1];
            else
                return (gyr_raw[2] / gyr_sens) - data_clb[2];
        }
        else
        {
            if (a == axis::x)
                return gyr_raw[0] / gyr_sens;
            else if (a == axis::y)
                return gyr_raw[1] / gyr_sens;
            else
                return gyr_raw[2] / gyr_sens;
        }
    }

    template <typename T>
    void mpu6050<T>::calibration_gyr(std::array<double, T::size> &data_clb)
    {

        for (int i{0}; i < 1000; i++)
        {
            read_gyr();

            data_clb[0] += gyr_raw[0] / gyr_sens;
            data_clb[1] += gyr_raw[1] / gyr_sens;
            data_clb[2] += gyr_raw[2] / gyr_sens;

            // std::this_thread::sleep_for(ms<uint16_t>{1});
        }

        data_clb[0] /= 1000;
        data_clb[1] /= 1000;
        data_clb[2] /= 1000;
    }

    template <typename T>
    double mpu6050<T>::get_roll() const
    {
        return atan2(get_acc_val(axis::y), get_acc_val(axis::z)) * (180 / M_PI);
    }

    template <typename T>
    double mpu6050<T>::get_pitch() const
    {
        return atan2(-get_acc_val(axis::x),
                     sqrt(get_acc_val(axis::y) * get_acc_val(axis::y) +
                          get_acc_val(axis::z) * get_acc_val(axis::z))) *
               (180 / M_PI);
    }

    template <typename T>
    mpu6050<T>::~mpu6050() {}
}

#endif
