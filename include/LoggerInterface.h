#pragma once

#include "SensorInterface.h"
#include "UUIDs.h"
#include "VariableInterface.h"

#include <ModularSensors.h>
#include <publishers/EnviroDIYPublisher.h>

namespace timu
{

    namespace detail
    {
        // Terminal baud rate
        const auto TERMINAL_BAUD = 57600UL;
        // Time zone. -4 = eastern
        const auto TIME_ZONE = -4;
    } // namespace detail

    /**
     * @brief Interface for logger-related functions
     */
    class LoggerInterface
    {

      public:
        /**
         * @brief Logger interface constructor
         */
        LoggerInterface()
            : sensorIface_{},
              variableIface_{sensorIface_},
              dataLogger_{station::STATION_ID, station::LOGGING_INTERVAL, &variableIface_.getList()},
              publisher_{dataLogger_, &sensorIface_.getModem().gsmClient, uuids::TOKEN, uuids::SAMPLING_FEATURE}
        {
        }

        /**
         * @brief Setup the logger
         */
        auto setup() -> void
        {
            startTerminalSerial();
            sensorIface_.startModemSerial();

            setupLogger();
            sensorIface_.setupModem();

            syncClock();
            createLogFile();
        }

        /**
         * @brief Get the battery voltage
         * @return float battery voltage
         */
        auto getBatteryVoltage() -> float
        {
            const auto& voltage = sensorIface_.getStats().sensorValues[0];

            // Update the reading if the last one was bad
            if (voltage == -9999.0F)
            {
                sensorIface_.getStats().update();
            }

            return voltage;
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
        timu::SensorInterface sensorIface_;
        // Variable interface
        timu::VariableInterface variableIface_;

        // The data logger
        Logger dataLogger_;
        // Data publisher
        EnviroDIYPublisher publisher_;

        /**
         * @brief Start the terminal serial
         */
        auto startTerminalSerial() -> void
        {
            Serial.begin(detail::TERMINAL_BAUD);
        }

        /**
         * @brief Setup the logger
         */
        auto setupLogger() -> void
        {
            Logger::setLoggerTimeZone(detail::TIME_ZONE);
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
            if (getBatteryVoltage() > 3.55 || !dataLogger_.isRTCSane())
            {
                dataLogger_.syncRTC();
            }
        }

        /**
         * @brief Create the log file if we have enough juice
         */
        auto createLogFile() -> void
        {
            if (getBatteryVoltage() > 3.4)
            {
                Serial.println(F("Setting up file on SD card"));
                dataLogger_.turnOnSDcard(true);  // true = wait for card to settle after power up
                dataLogger_.createLogFile(true); // true = write a new header
                dataLogger_.turnOffSDcard(true); // true = wait for internal housekeeping after write
            }
        }
    };

} // namespace timu