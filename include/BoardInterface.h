#pragma once

#include <ModularSensors.h>

#include "LoggerInterface.h"

namespace timu
{
    namespace config
    {
        const auto FLASH_COUNT = 4U;
        const auto FLASH_RATE = 75U;

        const auto SLEEP_THRESHOLD = 3.4F;
        const auto LOG_THRESHOLD = 3.55F;

    } // namespace config

    class Station
    {

      public:
        Station()
            : loggerIface_{}
        {
        }

        auto setup() -> void
        {
            setupLEDs();
            loggerIface_.setup();
        }

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
        LoggerInterface loggerIface_;

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