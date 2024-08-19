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
                        variable_interface::VariableInterface& variableInterface)
            : sensorIface_{sensorInterface},
              variableIface_{variableInterface},
              dataLogger_{station::STATION_ID, station::LOGGING_INTERVAL, &variableIface_.getList()},
              publisher_{dataLogger_, &sensorIface_.getModem().gsmClient, uuids::TOKEN, uuids::SAMPLING_FEATURE}
        {
        }

        /**
         * @brief Start the terminal serial
         */
        auto startTerminalSerial() -> void
        {
            Serial.begin(config::TERMINAL_BAUD);
        }

        /**
         * @brief Setup the logger
         */
        auto setup() -> void
        {
            setupLogger();
            syncClock();
            createLogFile();
        }

        /**
         * @brief Put the logger to sleep
         */
        auto sleep() -> void
        {
            Serial.println(F("Putting processor to sleep\n"));
            dataLogger_.systemSleep();
        }

        /**
         * @brief Log data to the SD card
         */
        auto log() -> void
        {
            dataLogger_.logData();
        }

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