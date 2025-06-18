/*
 *  Description : Simple PWM interface. It can be used only beaglebone
 *                black P9_14, P9_16, P9_21 and P9_22 pins for now.
 *  License     : MIT License
 *  Created on  : 2025
 *  Author      : Heval Aktaş
 *  Email       : hevalakts@gmail.com
 */
#ifndef PWM_H_
#define PWM_H_

#include <iostream>
#include <string>
#include <fstream>

namespace bbb
{

    class pwm
    {
        constexpr static const char *state_path = "/sys/devices/platform/ocp/ocp:P9_"; // used only for exporting

        std::string pwm_path{"/dev/bone/pwm/"};

        constexpr static const char period[] = "/period";
        constexpr static const char enable[] = "/enable";
        constexpr static const char polarity[] = "/polarity";
        constexpr static const char duty_cycle[] = "/duty_cycle";

        struct poles
        {
            constexpr static const char normal[]{"normal"};
            constexpr static const char inversed[]{"inversed"};
        };

        std::fstream m_file;

        int write(const char *filename, std::string value);
        std::string read(const char *filename);

    public:
        explicit pwm(uint16_t pin);

        pwm(const pwm &) = delete;
        pwm &operator=(const pwm &) = delete;

        int set_period(uint32_t per);
        int get_period();

        int set_duty_cycle(double per);
        int set_duty_cycle(uint32_t duty);
        int get_duty_cycle();

        int set_enable();
        int set_disable();

        int set_normal_polarity();
        int set_inversed_polarity();
        std::string get_polarity();
    };
}

#endif
