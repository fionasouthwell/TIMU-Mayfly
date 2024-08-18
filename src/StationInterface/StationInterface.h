#pragma once

#include "LoggerInterface/LoggerInterface.h"
#include "SensorInterface/SensorInterface.h"
#include "StationInterface/Config.h"
#include "VariableInterface/VariableInterface.h"

#include <ModularSensors.h>

namespace timu::station_interface
{

    class Station
    {

      public:
        /**
         * @brief Station constructor
         */
        Station()
            : sensorIface_{},
              variableIface_{sensorIface_},
              loggerIface_{sensorIface_, variableIface_}
        {
        }

        /**
         * @brief Setup the station
         */
        auto setup() -> void
        {
            setupLEDs();

            sensorIface_.startModemSerial();
            loggerIface_.startTerminalSerial();

            sensorIface_.setup();
            loggerIface_.setup();
        }

        /**
         * @brief Called in main, whenever the processor reawakes
         */
        auto log() -> void
        {
            const auto voltage = sensorIface_.getBatteryVoltage();

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
        // Interface for sensor-related functionality
        sensor_interface::SensorInterface sensorIface_;
        // Interface for variable-related functionality
        variable_interface::VariableInterface variableIface_;
        // Interface for logger-related functionality
        logger_interface::LoggerInterface loggerIface_;

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

} // namespace timu::station_interface