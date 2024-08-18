#pragma once

#include "SensorInterface/Types.h"

namespace timu::sensor_interface::config
{
    const auto NETWORK_MODE = types::NetworkMode::LTE_ONLY;
    const auto PREFERRED_MODE = types::PreferredMode::CAT_M;
    const auto MODEM_BAUD = 9600;
} // namespace timu::sensor_interface::config
