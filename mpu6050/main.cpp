/*
 *  Description : Example usage of the mpu6050 sensor
 *  License     : MIT License
 *  Created on  : 2024
 *  Author      : Heval Aktaş
 *  Email       : hevalakts@gmail.com
 */

#include <iostream>
#include "mpu6050.h"
#include "kalman.hpp"


// i2c-2 pins for beaglebone black : SCL : P9_19 -> 95    SDA : P9_20 -> 94    VCC : P9_3    GND P9_1



int main()
{
    using namespace bbb;

    mpu6050<with_clb> mpu{0};
    kalman k_roll;
    kalman k_pitch;
    
    double acc_x, acc_y, acc_z;
    double gyr_x, gyr_y, gyr_z;
    double kalman_roll, kalman_pitch;

    std::ostream os{std::cout.rdbuf()};
    os.setf(std::ios::fixed, std::ios::floatfield);
    os.setf(std::ios::showpos | std::ios::showpoint);
    os.precision(2);

    os
        // << "acc_x" << "\t" << "acc_y" << "\t" << "acc_z" << "\t"
        // << "gyr_x" << "\t" << "gyr_y" << "\t" << "gyr_z" << "\t"
        << "roll" << "\t" << "pitch" << "\t"
        << "kalman_roll" << "\t" << "kalnman_pitch" 
        << "\n";


    // mpu.read_acc();
    // k_roll.init_angle(mpu.get_roll());
    // k_pitch.init_angle(mpu.get_pitch());

    while (true)
    {
        using namespace std::chrono;

        auto start = std::chrono::steady_clock::now();

        mpu.read_acc();
        mpu.read_gyr();

        acc_x = mpu.get_acc_val(axis::x);
        acc_y = mpu.get_acc_val(axis::y);
        acc_z = mpu.get_acc_val(axis::z);

        gyr_x = mpu.get_gyr_val(axis::x);
        gyr_y = mpu.get_gyr_val(axis::y);
        gyr_z = mpu.get_gyr_val(axis::z);

        std::this_thread::sleep_for(ms<uint16_t>{200});

        auto end = std::chrono::steady_clock::now();
        auto dt = duration<double>(end - start).count();

        kalman_roll = k_roll.angle(mpu.get_roll(), gyr_x, dt);
        kalman_pitch = k_pitch.angle(mpu.get_pitch(), gyr_y, dt);

        os
            // << "|" << acc_x << "|\t|" << acc_y << "|\t|" << acc_z << "|\t|"
            // << gyr_x << "|\t|" << gyr_y << "|\t|" << gyr_z << "|\t|"
            << mpu.get_roll() << "|\t|" << mpu.get_pitch()
            << "|\t|" << kalman_roll << "|\t\t|" << kalman_pitch 
            << "|\n";
    }
}
