/*
 *  Description : Simple GPIO interface. m_file, always binds the file of
 *                value but other operations can also be done.
 *  License     : MIT License
 *  Created on  : 2025
 *  Author      : Heval Aktaş
 *  Email       : hevalakts@gmail.com
 */
#ifndef GPIO_H_
#define GPIO_H_

#include <string>
#include <fstream>

namespace bbb
{
    enum class direction
    {
        out,
        in
    };
    enum class value
    {
        low,
        high
    };

    class gpio
    {
    public:
        explicit gpio(uint16_t number);
        gpio(uint16_t number, bbb::direction dir);
        gpio(const gpio &other) = delete;
        gpio &operator=(const gpio &other) = delete;

        int set_direction(bbb::direction dir);
        std::string get_direction();

        int set_value(bbb::value val);
        int get_value();

        int set_active_low(bool act_low = true);
        int set_active_high();

        int gpio_unexport();

        ~gpio();

    private:
        int gpio_export();

        int open_file(const char *);
        int set(const char *file, int val);
        int set(const char *file, const char *val);
        std::string get(const char *file);

        constexpr static const char gpio_path[] = "/sys/class/gpio/";

        constexpr static const char active_low[] = "active_low";
        constexpr static const char direction[] = "direction";
        constexpr static const char value[] = "value";
        constexpr static const char export_p[] = "export";
        constexpr static const char unexport_p[] = "unexport";

        uint16_t m_number;
        std::string m_path;

        std::fstream m_file;
    };
}

#endif