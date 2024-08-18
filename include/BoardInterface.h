#pragma once

#include <ModularSensors.h>

#include "LoggerInterface.h"

namespace timu
{
    namespace config
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

    } // namespace config

    class Station
    {

      public:
        /**
         * @brief Station constructor
         */
        Station()
            : loggerIface_{}
        {
        }

        /**
         * @brief Setup the station
         */
        auto setup() -> void
        {
            setupLEDs();
            loggerIface_.setup();
        }

        /**
         * @brief Called in main, whenever the processor reawakes
         */
        auto work() -> void
        {
            const auto voltage = loggerIface_.getBatteryVoltage();

            // If battery is low, just sleep
            if (voltage < config::SLEEP_THRESHOLD)
            {
                loggerIface_.sleep();
                return;
            }

            // At moderate voltage, log to file
            if (voltage < config::LOG_THRESHOLD)
            {
                loggerIface_.log();
                return;
            }

            // Log and publish if we have enough juice
            loggerIface_.logAndPublish();
        }

      private:
        // Interface for logger-related functionality
        LoggerInterface loggerIface_;

        // Setup and turn on the LEDs
        auto setupLEDs() -> void
        {
            // setup LED pin modes
            pinMode(pins::LED_GREEN, OUTPUT);
            digitalWrite({pins::LED_GREEN}, LOW);
            pinMode(pins::LED_RED, OUTPUT);
            digitalWrite(pins::LED_RED, LOW);

            // flash em
            for (auto i = 0U; i < config::FLASH_COUNT; i++)
            {
                digitalWrite(pins::LED_GREEN, HIGH);
                digitalWrite(pins::LED_RED, LOW);
                delay(config::FLASH_RATE);
                digitalWrite(pins::LED_GREEN, LOW);
                digitalWrite(pins::LED_RED, HIGH);
                delay(config::FLASH_RATE);
            }
            digitalWrite(pins::LED_RED, LOW);
        }
    };

} // namespace timu