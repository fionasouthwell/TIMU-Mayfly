#include "LoggerInterface/LoggerInterface.h"

namespace timu::logger_interface
{
    LoggerInterface::LoggerInterface(sensor_interface::SensorInterface& sensorInterface,
                                     variable_interface::VariableInterface& variableInterface)
        : sensorIface_{sensorInterface},
          variableIface_{variableInterface},
          dataLogger_{station::STATION_ID, station::LOGGING_INTERVAL, &variableIface_.getList()},
          publisher_{dataLogger_, &sensorIface_.getModem().gsmClient, uuids::TOKEN, uuids::SAMPLING_FEATURE}
    {
    }

    auto LoggerInterface::startTerminalSerial() -> void
    {
        Serial.begin(config::TERMINAL_BAUD);
    }

    auto LoggerInterface::setup() -> void
    {
        setupLogger();
        syncClock();
        createLogFile();
    }

    auto LoggerInterface::sleep() -> void
    {
        Serial.println(F("Putting processor to sleep\n"));
        dataLogger_.systemSleep();
    }

    auto LoggerInterface::log() -> void
    {
        dataLogger_.logData();
    }

            auto LoggerInterface::logAndPublish() -> void
        {
            dataLogger_.logDataAndPublish();
        }

} // namespace timu::logger_interface