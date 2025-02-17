/*
 *  Description : Simple PWM interface
 *  License     : MIT License
 *  Created on  : 2025
 *  Author      : Heval Aktaş
 *  Email       : hevalakts@gmail.com
 */
#include "pwm.h"
#include <iostream>
#include <thread>

/*
  SG90 Micro Servo

  Rotational Range : 180°
  Pulse Cycle      : 20 ms
  Pulse Width      : 500 - 2400 µs

  Duty Cycle = (pulse_width / pulse_cycle) * 100  ==> [%2.5 - %12]

 */

int main()
{
    using namespace std::literals;

    bbb::pwm mypwm{16};

    mypwm.set_period(20'000'000u);
    mypwm.set_normal_polarity();
    mypwm.set_enable();
    int n{3};

    while (n--)
    {
        int cnt{0};
        double d_c{2.5};

        while (cnt < 7)
        {
            mypwm.set_duty_cycle(d_c);
            d_c += 1.583333;

            std::cout << " " << cnt++ << "  ";
            std::cout << "duty_cycle : "
                      << mypwm.get_duty_cycle()
                      << '\n';
            std::this_thread::sleep_for(500ms);
        }
        std::cout << '\n';
    }

    return 0;
}