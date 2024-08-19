#pragma once

#include "SensorInterface/Types.h"

namespace timu::sensor_interface::config
{
   inline const auto NETWORK_MODE = types::NetworkMode::LTE_ONLY;
   inline const auto PREFERRED_MODE = types::PreferredMode::CAT_M;
   inline const auto MODEM_BAUD = 9600;
} // namespace timu::sensor_interface::config
