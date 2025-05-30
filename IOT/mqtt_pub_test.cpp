/*
 *  Description             : Simple MQTT Publish Test
 *  Physical environment    : MCP3208 SPI ADC, Light-Dependent Resistor (LDR), DS18B20 Temperature Sensor
 *  License                 : MIT License
 *  Created on              : 2025
 *  Author                  : Heval Aktaş
 *  Email                   : hevalakts@gmail.com
 */
#include <iostream>
#include <sstream>
#include <fstream>
#include <chrono>
#include <thread>
#include <atomic>
#include <csignal>

#include "spidevice.h"
#include "mqtt/async_client.h"

using namespace std::chrono;

constexpr const char DEF_SERVER_URI[] = "tcp://io.adafruit.com";
constexpr const char CLIENT_ID[] = "beaglebone_mqtt_test";

constexpr const char AIO_USERNAME[] = "heval21";
constexpr const char AIO_KEY[] = "*****YOUR_AIO_KEY*****";

constexpr const char TOPIC_LIGHT[] = "heval21/feeds/light";
constexpr const char TOPIC_TEMP[] = "heval21/feeds/temperature";

const int QOS = 1;
const int MAX_BUFFERED_MESSAGES = 1024;

std::atomic<bool> quit{false};
void ctrlc_handler(int) { quit = true; }

constexpr const char DS18B20[] = "/sys/bus/w1/devices/28-c2196014670d/temperature"; 
double get_temperature();

class light_from_MCP3208        
{
public:
    light_from_MCP3208()
    {
        spi.set_speed(1000000);
        spi.set_mode(0);

        send[0] = 0b00000111;
        send[1] = 0b11000000;
        send[2] = 0;
    }

    int level()
    {
        int adc_value = 0;
        double cur_voltage = 0;
        memset(receive, 0, sizeof(receive));

        spi.transfer(send, receive, 3);

        adc_value = combine_values(receive[1] & 0x0F, receive[2]);
        cur_voltage = adc_value * (3.30f / 4095);

        return (cur_voltage / 3.3) * 100;
    }

private:
    short combine_values(unsigned char upper, unsigned char lower)
    {
        return ((short)upper << 8) | ((short)lower);
    }

private:
    bbb::spi_device spi{0, 0};
    unsigned char send[3]{0};
    unsigned char receive[3]{0};
};

int main(int argc, char *argv[])
{
    using namespace std::literals;

    auto SERVER_URI = argc > 1 ? argv[1] : DEF_SERVER_URI;

    std::cout << "Initializing for server '"
              << SERVER_URI
              << "'...\n"
              << std::flush;

    auto cre_opts = mqtt::create_options_builder()
                        .server_uri(SERVER_URI)
                        .send_while_disconnected(true, true)
                        .max_buffered_messages(MAX_BUFFERED_MESSAGES)
                        .delete_oldest_messages()
                        .finalize();

    mqtt::async_client cli(cre_opts);

    cli.set_connected_handler([&cli](const std::string &) {
         std::cout << "***** Connected *****" << '\n'; 
    });

    cli.set_connection_lost_handler([&cli](const std::string &) {
         std::cout << "***** Connection Lost *****" << '\n'; 
    });

    auto will_msg = mqtt::message("heval21/feeds/status", "The publisher disconnected", 1, true);
    auto con_opts = mqtt::connect_options_builder()
                        .clean_session()
                        .keep_alive_interval(30s)
                        .automatic_reconnect(1s, 10s)
                        .will(will_msg)
                        .user_name(AIO_USERNAME)
                        .password(AIO_KEY)
                        .finalize();

    try
    {
        std::cout << "Starting connection...\n"
                  << std::flush;
        cli.connect(con_opts)->wait();

        auto top_light = mqtt::topic(cli, TOPIC_LIGHT, QOS);
        auto top_temp = mqtt::topic(cli, TOPIC_TEMP, QOS);
        std::cout << "Publishing data..." << std::flush;

        signal(SIGINT, ctrlc_handler);
        light_from_MCP3208 light;
        std::ostringstream temp;
        temp.precision(2);
        temp << std::fixed << get_temperature();

        top_light.publish(std::to_string(light.level()));
        top_temp.publish(temp.str());

        while (!quit)
        {
            std::this_thread::sleep_for(20s);

            temp << std::fixed << get_temperature();
            
            top_light.publish(std::to_string(light.level()));
            top_temp.publish(temp.str());
            
            temp.str("");
        }

        std::cout << "\nDisconnecting...\n" << std::flush;
        cli.disconnect()->wait();
        std::cout << "...OK\n" << std::flush;

    }
    catch (const std::exception &ex)
    {
        std::cerr << ex.what() << '\n';
    }

    return 0;
}

double get_temperature()
{
    int temp_degree;
    std::ifstream ifs{DS18B20};
    if (!ifs)
    {
        std::cerr << "Failed to open the file of temerature!" << std::endl;
        return 1;
    }
    ifs >> temp_degree;

    return temp_degree / 1000.0;
}