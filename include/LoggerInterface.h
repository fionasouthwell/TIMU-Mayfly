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
        const auto TERMINAL_BAUD = 57600UL;
        const auto TIME_ZONE = -4;
    } // namespace detail

    class LoggerInterface
    {

      public:
        LoggerInterface()
            : sensorIface_{},
              variableIface_{sensorIface_},
              dataLogger_{station::STATION_ID, station::LOGGING_INTERVAL, &variableIface_.getList()},
              publisher_{dataLogger_, &sensorIface_.getModem().gsmClient, uuids::TOKEN, uuids::SAMPLING_FEATURE}
        {
        }

        auto setup() -> void
        {
            startTerminalSerial();
            sensorIface_.startModemSerial();

            setupLogger();
            sensorIface_.setupModem();

            syncClock();
            createLogFile();
        }

        auto getBatteryVoltage() -> float
        {
            const auto& voltage = sensorIface_.getStats().sensorValues[0];

            if (voltage == -9999.0F)
            {
                sensorIface_.getStats().update();
            }

            return voltage;
        }

        auto sleep() -> void
        {
            Serial.println(F("Putting processor to sleep\n"));
            dataLogger_.systemSleep();
        }

        auto log() -> void
        {
            dataLogger_.logData();
        }

        auto logAndPublish() -> void
        {
            dataLogger_.logDataAndPublish();
        }

      private:
        timu::SensorInterface sensorIface_;
        timu::VariableInterface variableIface_;

        Logger dataLogger_;
        EnviroDIYPublisher publisher_;

        auto startTerminalSerial() -> void { Serial.begin(detail::TERMINAL_BAUD); }

        auto setupLogger() -> void
        {
            Logger::setLoggerTimeZone(detail::TIME_ZONE);
            Logger::setRTCTimeZone(0);

            dataLogger_.attachModem(sensorIface_.getModem());
            dataLogger_.setLoggerPins(pins::WAKE, pins::SD_SS, pins::SD_POWER, pins::DEBUG_BUTTON, pins::LED_GREEN);
            dataLogger_.begin();
        }

        auto syncClock() -> void
        {
            if (getBatteryVoltage() > 3.55 || !dataLogger_.isRTCSane())
            {
                dataLogger_.syncRTC();
            }
        }

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