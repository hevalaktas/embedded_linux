/*
 *  Description          : Simple DC Motor test
 *  Physical environment : DRV8833 Dual H-Bridge Motor Driver and DC Motor
 *  License              : MIT License
 *  Created on           : 2025
 *  Author               : Heval Aktaş
 *  Email                : hevalakts@gmail.com
 */
#include "dc_motor.h"

#include <iostream>
#include <thread>
#include <iomanip>

/*
    Beaglebone black    -->    DRV8833

    gpio60      P9_12   -->    stby
    gpio116     P9_41   -->    AIN2
    PWM         P9_16   -->    AIN1
*/

int main()
{

    using namespace std::literals;
    using namespace std::this_thread;

    bbb::dc_motor dc_m{16, 116, 60};

    dc_m.speed(75.);
    dc_m.start();

    std::cout << std::fixed
              << std::setprecision(1)
              << "duty cycle : "
              << dc_m.get_speed() << '\n';

    sleep_for(2000ms);

    dc_m.stop();
    sleep_for(250ms);

    dc_m.direction(bbb::counter_clockwise);
    dc_m.speed(25.);
    dc_m.start();
    std::cout << "duty cycle : " << dc_m.get_speed() << '\n';
    sleep_for(2000ms);

    return 0;
}
