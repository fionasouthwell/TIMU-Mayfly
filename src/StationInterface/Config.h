#pragma once

namespace timu::station_interface::config
{
    // Battery sleep threshold, volts. Station will go back to sleep if the battery level is less than this
    const auto SLEEP_THRESHOLD = 3.4F;
    // Battery log threshold, volts. Station will only log to file if the battery level is between the sleep
    // threshold and this
    const auto LOG_THRESHOLD = 3.55F;

    // Number of times to flash the LEDs on startup
    const auto FLASH_COUNT = 4U;
    // Flash rate
    const auto FLASH_RATE = 75U;

} // namespace timu::station_interface::config