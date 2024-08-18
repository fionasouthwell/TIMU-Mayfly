#pragma once

#include "Pins.h"

#include "SensorInterface/Types.h"
#include "SensorInterface/Config.h"

#include <ModularSensors.h>
#include <modems/SIMComSIM7080.h>
#include <sensors/AtlasScientificDO.h>
#include <sensors/AtlasScientificEC.h>
#include <sensors/AtlasScientificRTD.h>
#include <sensors/AtlasScientificpH.h>
#include <sensors/MaximDS3231.h>
#include <sensors/ProcessorStats.h>

namespace timu::sensor_interface
{

    class SensorInterface
    {
      public:
        /**
         * @brief Sensor Interface constructor
         */
        SensorInterface()
            : atlasDO_{pins::ALWAYS_POWERED},
              atlasEC_{pins::ALWAYS_POWERED},
              atlasPH_{pins::ALWAYS_POWERED},
              atlasRTD_{pins::ALWAYS_POWERED},
              modem_{&Serial1, pins::MODEM_POWER, pins::MODEM_STATUS, pins::MODEM_SLEEP_RQ, "hologram"},
              stats_{"v1.0"},
              rtc_{}
        {
        }

        auto setup() -> void
        {
            setupModem();
        }

        /**
         * @brief Start the modem serial
         */
        void startModemSerial()
        {
            Serial1.begin(config::MODEM_BAUD);
        }

        /**
         * @brief Get the battery voltage
         * @return float battery voltage
         */
        auto getBatteryVoltage() -> float
        {
            const auto& voltage = stats_.sensorValues[0];

            // Update the reading if the last one was bad
            if (voltage == -9999.0F)
            {
                stats_.update();
            }

            return voltage;
        }

        /**
         * @brief Get a reference to the AtlasDO sensor
         * @return AtlasScientificDO&
         */
        auto getDO() -> AtlasScientificDO&
        {
            return atlasDO_;
        }

        /**
         * @brief Get a reference to the AtlasEC sensor
         * @return AtlasScientificEC&
         */
        auto getEC() -> AtlasScientificEC&
        {
            return atlasEC_;
        }

        /**
         * @brief Get a reference to the Atlas pH sensor
         * @return AtlasScientificpH&
         */
        auto getPH() -> AtlasScientificpH&
        {
            return atlasPH_;
        }

        /**
         * @brief Get a reference to the Atlas RTD sensor
         *
         * @return AtlasScientificRTD&
         */
        auto getRTD() -> AtlasScientificRTD&
        {
            return atlasRTD_;
        }

        /**
         * @brief Get a reference to the Modem sensor
         * @return SIMComSIM7080&
         */
        auto getModem() -> SIMComSIM7080&
        {
            return modem_;
        }

        /**
         * @brief Get a reference to the Processor sensor
         * @return ProcessorStats&
         */
        auto getStats() -> ProcessorStats&
        {
            return stats_;
        }

        /**
         * @brief Get a reference to the Real-Time Clock sensor
         * @return MaximDS3231&
         */
        auto getRTC() -> MaximDS3231&
        {
            return rtc_;
        }

      private:
        // Atlas DO sensor
        AtlasScientificDO atlasDO_;
        // Atlas EC sensor
        AtlasScientificEC atlasEC_;
        // Atlas pH sensor
        AtlasScientificpH atlasPH_;
        // Atlas RTD sensor
        AtlasScientificRTD atlasRTD_;
        // Modem sensor
        SIMComSIM7080 modem_;
        // Processor sensor
        ProcessorStats stats_;
        // Real-Time Clock sensor
        MaximDS3231 rtc_;

        /**
         * @brief Setup the modem
         */
        void setupModem()
        {
            modem_.setModemLED(pins::LED_MODEM);
            modem_.setModemWakeLevel(HIGH);
            modem_.setModemResetLevel(HIGH);

            Serial.println(F("Waking modem and setting Cellular Carrier Options..."));
            modem_.modemWake();
            modem_.gsmModem.setBaud(config::MODEM_BAUD);

            const auto networkMode = static_cast<uint8_t>(config::NETWORK_MODE);
            modem_.gsmModem.setNetworkMode(networkMode);

            const auto preferredMode = static_cast<uint8_t>(config::PREFERRED_MODE);
            modem_.gsmModem.setPreferredMode(preferredMode);
        }
    };

} // namespace timu::sensor_interface