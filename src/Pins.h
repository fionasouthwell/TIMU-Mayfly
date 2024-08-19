#pragma once

namespace timu
{
    namespace pins
    {
        inline const auto LED_GREEN = 8;
        inline const auto LED_RED = 9;
        inline const auto LED_MODEM = 9;

        inline const auto DEBUG_BUTTON = 21;

        inline const auto ALWAYS_POWERED = -1;
        inline const auto SENSOR_POWER = 22;
        inline const auto REG_3V3 = 22;

        inline const auto SD_POWER = -1;
        inline const auto SD_SS = 12;

        inline const auto MODEM_POWER = 18;
        inline const auto MODEM_STATUS = 19;
        inline const auto MODEM_SLEEP_RQ = 23;
        inline const auto WAKE = 31;
    } // namespace pins

} // namespace timu
