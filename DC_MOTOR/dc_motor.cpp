/*
 *  Description : Simple DC Motor interface
 *  License     : MIT License
 *  Created on  : 2025
 *  Author      : Heval Aktaş
 *  Email       : hevalakts@gmail.com
 */
#include "dc_motor.h"

namespace bbb
{

    dc_motor::dc_motor(uint16_t pwm, uint16_t gpio_rotate, uint16_t gpio_stby, uint32_t per) : m_speed{pwm},
                                                                                               m_rotate{gpio_rotate},
                                                                                               m_stby{gpio_stby}
    {
        power_on();
        direction(clockwise);
        period(per);
    }

    void dc_motor::power_on()
    {
        m_stby.set_direction(bbb::direction::out);
        m_stby.set_value(on);
    }

    void dc_motor::power_off()
    {
        m_stby.set_value(off);
    }

    void dc_motor::direction(bbb::direction dir)
    {
        m_rotate.set_direction(dir);
        m_rotate.set_value(on);
    }

    direction dc_motor::get_direction()
    {
        if(m_rotate.get_direction() == "out")
        {
            return bbb::direction::out;
        }
        else
        {
            return bbb::direction::in;
        }
    }

    void dc_motor::start()
    {
        m_speed.set_enable();
        m_rotate.set_value(bbb::value::high);
    }

    void dc_motor::stop()
    {
        m_speed.set_disable();
        m_rotate.set_value(bbb::value::low);
    }

    void dc_motor::period(uint32_t per)
    {
        m_speed.set_period(per);
    }

    void dc_motor::speed(double speed)
    {
        m_speed.set_duty_cycle(speed);
    }

    double dc_motor::get_speed()
    {
        return 100 * m_speed.get_duty_cycle() / (double)m_speed.get_period();
    }

    dc_motor::~dc_motor()
    {
        stop();
        power_off();
    }
}