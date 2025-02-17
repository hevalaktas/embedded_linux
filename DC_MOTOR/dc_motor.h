/*
 *  Description : Simple DC Motor interface
 *  License     : MIT License
 *  Created on  : 2025
 *  Author      : Heval Aktaş
 *  Email       : hevalakts@gmail.com
 */
#ifndef DC_MOTOR_
#define DC_MOTOR_

#include "gpio.h"
#include "pwm.h"

namespace bbb
{

    constexpr static const auto clockwise = bbb::direction::out;
    constexpr static const auto counter_clockwise = bbb::direction::in;
    constexpr static const auto on = bbb::value::high;
    constexpr static const auto off = bbb::value::low;

    class dc_motor
    {

        bbb::pwm m_speed;
        bbb::gpio m_rotate;
        bbb::gpio m_stby;

    public:
        dc_motor(uint16_t pwm, uint16_t gpio_rotate, uint16_t gpio_stby, uint32_t per = 1'000'000u);
        dc_motor(const dc_motor &) = delete;
        dc_motor &operator=(const dc_motor &) = delete;

        void power_on();
        void power_off();

        void period(uint32_t per);
        void direction(bbb::direction dir);
        void speed(double speed);
        void start();
        void stop();

        double get_speed();
        bbb::direction get_direction();

        ~dc_motor();
    };
}

#endif