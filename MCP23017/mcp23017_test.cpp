/*
 *  Description          : MCP23017 test
 *  License              : MIT License
 *  Created on           : 2025
 *  Author               : Heval Aktaş
 *  Email                : hevalakts@gmail.com
 */
#include "mcp23017.hpp"

#include <iostream>
#include <bitset>
#include <thread>
#include <chrono>

constexpr uint8_t pin(short which_pin)
{
    return 1u << which_pin;
}

// template <std::size_t N>
// uint8_t IO = 1u << N;    // alternative

void print_reg(uint8_t val, const char *name)
{
    std::cout
        << name
        << (int)val
        << " "
        << std::bitset<8>{val} << '\n';
}

const auto hold_on_a_moment = [](unsigned int x)
{
    std::this_thread::sleep_for(std::chrono::milliseconds{x});
};

int main()
{
    using namespace bbb;
    using namespace mcp23017_control_reg;

    mcp23017 mcp{2, 0x20};

    mcp.set_B<iodir>(0x00);

    /*

        Test 1 : GPB4 and GPB7 out-high 0b1001'0000

    */
    mcp.set_B<olat>(pin(4) | pin(7));

    hold_on_a_moment(1000);

    auto gpiob = mcp.get_B<gpio>();
    print_reg(gpiob, "gpio b : ");
    hold_on_a_moment(1000);

    /*

        Test 2 : push button, GPA5 in, pull-up 00100000

    */
    mcp.set_A<iodir>(pin(5)); // 00100000
    mcp.set_A<gppu>(pin(5));

    auto gpioa = mcp.get_A<gpio>();
    print_reg(gpioa, "gpio a : ");

    while (true)
    {
        if (auto buton = mcp.get_A<gpio>(); ~(buton >> 5) & 1)
        {
            std::cout << "button (pin 5) : "
                      << std::bitset<8>{buton}
                      << " was pressed.\n";
            break;
        }
    }

    /*

        test 3 : Interrupt Configuration  (INTA)

    */
    mcp.set_A<gpinten>(pin(5));
    mcp.set_A<defval>(pin(5));
    mcp.set_A<intcon>(pin(5));

    auto intfa = mcp.get_A<intf>();
    print_reg(intfa, "intf a : ");

    hold_on_a_moment(2000);

    auto intcapa = mcp.get_A<intcap>();
    print_reg(intcapa, "intcap a : ");

    mcp.set_B<olat>(0x00);

    return 0;
}
