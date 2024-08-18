#pragma once

#include "LoggerInterface/Config.h"
#include "SensorInterface/SensorInterface.h"
#include "VariableInterface/VariableInterface.h"

#include <ModularSensors.h>
#include <publishers/EnviroDIYPublisher.h>

namespace timu::logger_interface
{
    /**
     * @brief Interface for logger-related functions
     */
    class LoggerInterface
    {

      public:
        /**
         * @brief Logger interface constructor
         */
        LoggerInterface(sensor_interface::SensorInterface& sensorInterface,
                        variable_interface::VariableInterface& variableInterface);

        /**
         * @brief Start the terminal serial
         */
        auto startTerminalSerial() -> void;

        /**
         * @brief Setup the logger
         */
        auto setup() -> void;

        /**
         * @brief Put the logger to sleep
         */
        auto sleep() -> void;

        /**
         * @brief Log data to the SD card
         */
        auto log() -> void;

        /**
         * @brief Log data to the SD card and publish to MMW
         */
        auto logAndPublish() -> void
        {
            dataLogger_.logDataAndPublish();
        }

      private:
        // Sensor interface
        sensor_interface::SensorInterface& sensorIface_;
        // Variable interface
        variable_interface::VariableInterface& variableIface_;

        // The data logger
        Logger dataLogger_;
        // Data publisher
        EnviroDIYPublisher publisher_;

        /**
         * @brief Setup the logger
         */
        auto setupLogger() -> void
        {
            Logger::setLoggerTimeZone(config::TIME_ZONE);
            Logger::setRTCTimeZone(0);

            dataLogger_.attachModem(sensorIface_.getModem());
            dataLogger_.setLoggerPins(pins::WAKE, pins::SD_SS, pins::SD_POWER, pins::DEBUG_BUTTON, pins::LED_GREEN);
            dataLogger_.begin();
        }

        /**
         * @brief Sync the clock if we have enough juice
         */
        auto syncClock() -> void
        {
            if (sensorIface_.getBatteryVoltage() > 3.55 || !dataLogger_.isRTCSane())
            {
                dataLogger_.syncRTC();
            }
        }

        /**
         * @brief Create the log file if we have enough juice
         */
        auto createLogFile() -> void
        {
            if (sensorIface_.getBatteryVoltage() > 3.4)
            {
                Serial.println(F("Setting up file on SD card"));
                dataLogger_.turnOnSDcard(true);  // true = wait for card to settle after power up
                dataLogger_.createLogFile(true); // true = write a new header
                dataLogger_.turnOffSDcard(true); // true = wait for internal housekeeping after write
            }
        }
    };

} // namespace timu::logger_interface