#pragma once

#include <Arduino.h>

namespace timu::sensor_interface::types
{

    /**
     * @brief Modem Network Mode
     */
    enum class NetworkMode : uint8_t
    {
        AUTOMATIC = 2,
        GSM_ONLY = 13,
        LTE_ONLY = 38,
        GSM_AND_LTE = 51
    };

    /**
     * @brief Modem Preferred Mode
     */
    enum class PreferredMode : uint8_t
    {
        CAT_M = 1,
        NB_IOT = 2,
        CAT_M_AND_NB_IOT = 3
    };

} // namespace timu::sensor_interface::types