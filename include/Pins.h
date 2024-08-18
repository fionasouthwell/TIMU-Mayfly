#pragma once

namespace timu
{

    namespace pins
    {

        const auto LED_GREEN = 8;
        const auto LED_RED = 9;
        const auto LED_MODEM = 9;

        const auto DEBUG_BUTTON = 21;

        const auto ALWAYS_POWERED = -1;
        const auto SENSOR_POWER = 22;
        const auto REG_3V3 = 22;

        const auto SD_POWER = -1;
        const auto SD_SS = 12;

        const auto MODEM_POWER = 18;
        const auto MODEM_STATUS = 19;
        const auto MODEM_SLEEP_RQ = 23;
        const auto WAKE = 31;


    } // namespace pins

} // namespace timu

// #define PIN_LED_GREEN 8
// #define PIN_LED_RED 9
// #define PIN_BUTTON_DEBUG 21
// #define PIN_WAKE 31
// #define PIN_SD_PWR -1
// #define PIN_SD_SS 12
// #define PIN_SENSOR_POWER 22
// #define PIN_REG_3V3 22

// #define PIN_MODEM_PWR 18
// #define PIN_MODEM_STATUS 19
// #define PIN_MODEM_SLEEP_RQ 23
// #define PIN_MODEM_LED 9