/*
 *  Description          : Simple MCP23017 interface
 *  License              : MIT License
 *  Created on           : 2025
 *  Author               : Heval Aktaş
 *  Email                : hevalakts@gmail.com
 */
#ifndef MCP23017_HPP_
#define MCP23017_HPP_

#include <stdint.h>
#include "i2cdevice.h"
#include "type_traits"

namespace bbb
{

    namespace mcp23017_control_reg
    {

        /*
            Note : Pins GPA7 and GPB7 are output only for MCP23017

            CONTROL REGISTER SUMMARY (IOCON.BANK = 0)

            set      : iodir, ipol, gpinten, defval, intcon, gppu, olat
            only get : gpio, intcap, intf

            https://ww1.microchip.com/downloads/aemDocuments/documents/APID/ProductDocuments/DataSheets/MCP23017-Data-Sheet-DS20001952.pdf
        */

        struct iodir // I/O Data Direction Register  1 -> in, 0 -> out
        {
            constexpr static const uint8_t a = 0x00; // Port A
            constexpr static const uint8_t b = 0x01; //	Port B
        };

        struct ipol // Input Polarity   1 = inversed, 0 = normal
        {
            constexpr static const uint8_t a = 0x02;
            constexpr static const uint8_t b = 0x03;
        };

        struct gpinten // GPIO Interrupt On Change  1 = enable, 0 = disable
        {
            constexpr static const uint8_t a = 0x04;
            constexpr static const uint8_t b = 0x05;
        };

        struct defval // Default Comparison Value (for Interrupt On Change)
        {
            constexpr static const uint8_t a = 0x06;
            constexpr static const uint8_t b = 0x07;
        };

        struct intcon // Interrupt Control Register  1 = on change, 0 = compare
        {
            constexpr static const uint8_t a = 0x08;
            constexpr static const uint8_t b = 0x09;
        };

        struct iocon // I/O Configuration Register.
        {
            constexpr static const uint8_t a = 0x0A;
            constexpr static const uint8_t b = 0x0B;
        };

        struct gppu // GPIO Pullup Register. for the input ports (100k Ohm value)   1 = Pull-up enabled   0 = Pull-up disabled
        {
            constexpr static const uint8_t a = 0x0C;
            constexpr static const uint8_t b = 0x0D;
        };

        struct intf // Interrupt Flag Register.  1 = Pin caused interrupt. 0 = Interrupt not pending
        {
            constexpr static const uint8_t a = 0x0E;
            constexpr static const uint8_t b = 0x0F;
        };

        struct intcap // Interrupt Capture Register.  1 = Logic-high  0 = Logic-low
        {
            // If you read data back from INTCAP or GPIO this will clear the interrupts.
            constexpr static const uint8_t a = 0x10;
            constexpr static const uint8_t b = 0x11;
        };

        struct gpio // GPIO
        {
            constexpr static const uint8_t a = 0x12;
            constexpr static const uint8_t b = 0x13;
        };

        struct olat // Output Latch Register
        {
            constexpr static const uint8_t a = 0x14;
            constexpr static const uint8_t b = 0x15;
        };

    }

    class mcp23017 : private i2c_device
    {
        /*
            Configuration Register :
            default 00111010  -> 0x3A
        */
        uint8_t m_config;

    public:
        explicit mcp23017(uint16_t dev, uint16_t addr = 0x20, uint8_t io_conf = 0x3A) : i2c_device{dev, addr},
                                                                                        m_config{io_conf}
        {
            configuration(m_config);
        }

        template <typename T>
        int set(uint16_t val) // General use for both port A and port B
        {
            using namespace mcp23017_control_reg;

            static_assert(!std::is_same_v<T, gpio> && !std::is_same_v<T, iocon> &&
                          !std::is_same_v<T, intf> && !std::is_same_v<T, intcap> &&
                          "Cannot set!");

            write_register(T::a, val & 0xFF);
            write_register(T::b, val >> 8);

            return {};
        }

        template <class T>
        int set_A(uint8_t val) // only for Port A
        {
            using namespace mcp23017_control_reg;

            static_assert(!std::is_same_v<T, gpio> && !std::is_same_v<T, iocon> &&
                          !std::is_same_v<T, intf> && !std::is_same_v<T, intcap> &&
                          "Cannot set!");

            return write_register(T::a, val);
        }

        template <class T>
        int set_B(uint8_t val) // only for Port B
        {
            using namespace mcp23017_control_reg;

            static_assert(!std::is_same_v<T, gpio> && !std::is_same_v<T, iocon> &&
                          !std::is_same_v<T, intf> && !std::is_same_v<T, intcap> &&
                          "Cannot set!");

            return write_register(T::b, val);
        }

        template <typename T>
        uint16_t get() // General use for both port A and port B
        {
            uint16_t data{0};

            data = read_register(T::b);
            data = (data << 8) | read_register(T::a);

            return data;
        }

        template <typename T>
        uint8_t get_A() // only for Port A
        {
            return read_register(T::a);
        }

        template <typename T>
        uint8_t get_B() // only for Port B
        {
            return read_register(T::b);
        }

        int configuration(uint8_t val)
        {
            using mcp23017_control_reg::iocon;

            write_register(iocon::a, val);
            write_register(iocon::b, val);

            return 0;
        }
    };
}

#endif