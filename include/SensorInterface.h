#pragma once

#include "Pins.h"

#include <ModularSensors.h>
#include <modems/SIMComSIM7080.h>
#include <sensors/AtlasScientificDO.h>
#include <sensors/AtlasScientificEC.h>
#include <sensors/AtlasScientificRTD.h>
#include <sensors/AtlasScientificpH.h>
#include <sensors/MaximDS3231.h>
#include <sensors/ProcessorStats.h>

namespace timu
{

    namespace helpers
    {
        enum class NetworkMode : uint8_t
        {
            AUTOMATIC = 2,
            GSM_ONLY = 13,
            LTE_ONLY = 38,
            GSM_AND_LTE = 51
        };

        enum class PreferredMode : uint8_t
        {
            CAT_M = 1,
            NB_IOT = 2,
            CAT_M_AND_NB_IOT = 3
        };
    }

    namespace detail
    {
        const auto NETWORK_MODE = helpers::NetworkMode::LTE_ONLY;
        const auto PREFERRED_MODE = helpers::PreferredMode::CAT_M;
        const auto MODEM_BAUD = 9600;
    } // namespace detail

    class SensorInterface
    {
      public:
        SensorInterface() {}

        auto getDO() -> AtlasScientificDO& { return atlasDO_; }

        auto getEC() -> AtlasScientificEC& { return atlasEC_; }

        auto getPH() -> AtlasScientificpH& { return atlasPH_; }

        auto getRTD() -> AtlasScientificRTD& { return atlasRTD_; }

        auto getModem() -> SIMComSIM7080& { return modem_; }

        auto getStats() -> ProcessorStats& { return stats_; }

        auto getRTC() -> MaximDS3231& { return rtc_; }

        void setupModem()
        {
            modem_.setModemLED(pins::LED_MODEM);
            modem_.setModemWakeLevel(HIGH);
            modem_.setModemResetLevel(HIGH);

            Serial.println(F("Waking modem and setting Cellular Carrier Options..."));
            modem_.modemWake();                          // NOTE:  This will also set up the modem
            modem_.gsmModem.setBaud(detail::MODEM_BAUD); // Make sure we're *NOT* auto-bauding!

            const auto networkMode = static_cast<uint8_t>(detail::NETWORK_MODE);
            modem_.gsmModem.setNetworkMode(networkMode); 

            const auto preferredMode = static_cast<uint8_t>(detail::PREFERRED_MODE);
            modem_.gsmModem.setPreferredMode(preferredMode);        
        }

        void startModemSerial()
        {
            Serial1.begin(detail::MODEM_BAUD);
        }

      private:
        AtlasScientificDO atlasDO_{pins::ALWAYS_POWERED};
        AtlasScientificEC atlasEC_{pins::ALWAYS_POWERED};
        AtlasScientificpH atlasPH_{pins::ALWAYS_POWERED};
        AtlasScientificRTD atlasRTD_{pins::ALWAYS_POWERED};
        SIMComSIM7080 modem_{&Serial1, pins::MODEM_POWER, pins::MODEM_STATUS, pins::MODEM_SLEEP_RQ, "hologram"};
        ProcessorStats stats_{"v1.0"};
        MaximDS3231 rtc_{};
    };

} // namespace timu