/*
 *  Description : Simple PWM interface. It can be used only beaglebone
 *                black P9_14, P9_16, P9_21 and P9_22 pins for now.
 *  License     : MIT License
 *  Created on  : 2025
 *  Author      : Heval Aktaş
 *  Email       : hevalakts@gmail.com
 */
#include "pwm.h"

namespace bbb
{

    pwm::pwm(uint16_t pin)
    {

        if (14 == pin || pin == 16)
        {
            pwm_path += '1';
            pwm_path += '/';
            pwm_path += pin == 14 ? 'a' : 'b';
        }
        else if (22 == pin || pin == 21)
        {
            pwm_path += '0';
            pwm_path += '/';
            pwm_path += pin == 22 ? 'a' : 'b';
        }
        else
        {
            throw std::runtime_error{"invalid pin number for pwm"};
        }

        std::string file_path{state_path};
        file_path += std::to_string(pin);
        file_path += "_pinmux/state";

        m_file.open(file_path);
        if (!m_file)
        {
            std::cerr << "Pin " << pin << " cannot be exported!" << '\n';
        }
        m_file << "pwm";
        m_file.close();
    }

    int pwm::write(const char *filename, std::string value)
    {
        m_file.open(pwm_path + filename);
        if (!m_file.is_open())
        {
            std::cerr << "PWM : Can't set " << filename << '\n';
        }
        m_file << value;
        m_file.close();

        return 0;
    }

    std::string pwm::read(const char *filename)
    {
        m_file.open(pwm_path + filename);
        if (!m_file.is_open())
        {
            std::cerr << "PWM : Can't get " << filename << '\n';
        }

        std::string info;
        getline(m_file, info);
        m_file.close();

        return info;
    }

    int pwm::set_period(uint32_t per)
    {
        return write(period, std::to_string(per));
    }

    int pwm::get_period()
    {
        return atoi(read(period).c_str());
    }

    int pwm::set_duty_cycle(uint32_t duty)
    {
        return write(duty_cycle, std::to_string(duty));
    }

    int pwm::set_duty_cycle(double per)
    {
        if (per < 0 || 100 < per)
            return -1;
        uint32_t duty = get_period() * (per / 100.);

        return set_duty_cycle(duty);
    }

    int pwm::set_enable()
    {
        return write(enable, "1");
    }

    int pwm::get_duty_cycle()
    {
        return atoi(read(duty_cycle).c_str());
    }

    int pwm::set_disable()
    {
        return write(enable, "0");
    }

    int pwm::set_normal_polarity()
    {
        return write(polarity, pwm::poles::normal);
    }

    int pwm::set_inversed_polarity()
    {
        return write(polarity, pwm::poles::inversed);
    }

    std::string pwm::get_polarity()
    {
        return read(polarity);
    }
}
