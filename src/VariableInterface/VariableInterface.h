#pragma once


#include "SensorInterface/SensorInterface.h"

#include <UUIDs.h>

#include <ModularSensors.h>
#include <modems/SIMComSIM7080.h>
#include <sensors/AtlasScientificDO.h>
#include <sensors/AtlasScientificEC.h>
#include <sensors/AtlasScientificRTD.h>
#include <sensors/AtlasScientificpH.h>
#include <sensors/MaximDS3231.h>
#include <sensors/ProcessorStats.h>

namespace timu::variable_interface
{
    /**
     * @brief Interface to the sensor variable
     */
    class VariableInterface
    {
      public:
        /**
         * @brief VariableInterface constructor
         * @note constructs the variables with the UUIDS provided in UUIDs.h
         * @param sensorIface Sensor Interface
         */
        explicit VariableInterface(timu::sensor_interface::SensorInterface& sensorIface)
            : statsSampleNo_{&sensorIface.getStats(), uuids::SEQUENCE_NUMBER},
              statsRam_{&sensorIface.getStats(), uuids::FREE_SRAM},
              statsBattery_{&sensorIface.getStats(), uuids::BATTERY_VOLTAGE},
              modemRSSI_{&sensorIface.getModem(), uuids::SIGNAL_STRENGTH},
              rtcTemp_{&sensorIface.getRTC(), uuids::RTC_TEMPERATURE},
              rtdTemp_{&sensorIface.getRTD(), uuids::TEMPERATURE},
              ecCond_{&sensorIface.getEC(), uuids::CONDUCTIVITY},
              ecSalinity_{&sensorIface.getEC(), uuids::SALINITY},
              ecGravity_{&sensorIface.getEC(), uuids::GAGE_HEIGHT},
              ecTDS_{&sensorIface.getEC(), uuids::TOTAL_DISSOLVED_SOLIDS},
              phPH_{&sensorIface.getPH(), uuids::WATER_PH},
              doMgl_{&sensorIface.getDO(), uuids::DISSOLVED_OXYGEN},
              doPct_{&sensorIface.getDO(), uuids::DISSOLVED_OXYGEN_PERCENT}
        {
        }

        /**
         * @brief Get the list of variables
         * @return VariableArray& reference to the variable list
         */
        auto getList() -> VariableArray&
        {
            return variableArray_;
        }

      private:
        // Sample Number
        ProcessorStats_SampleNumber statsSampleNo_{};
        // Free RAM
        ProcessorStats_FreeRam statsRam_{};
        // Battery voltage
        ProcessorStats_Battery statsBattery_{};
        // Modem Signal Strength
        Modem_RSSI modemRSSI_;
        // RTC Temperature
        MaximDS3231_Temp rtcTemp_{};
        // Atlas RTD Temperature
        AtlasScientificRTD_Temp rtdTemp_{};
        // Atlas EC Conductivity
        AtlasScientificEC_Cond ecCond_{};
        // Atlas EC Salinity
        AtlasScientificEC_Salinity ecSalinity_{};
        // Atlas EC Specific Gravity
        AtlasScientificEC_SpecificGravity ecGravity_{};
        // Atlas EC Total Dissolved Solids
        AtlasScientificEC_TDS ecTDS_{};
        // Atlas ph pH
        AtlasScientificpH_pH phPH_{};
        // Atlas DO Concentration
        AtlasScientificDO_DOmgL doMgl_{};
        // Atlas DO Percent
        AtlasScientificDO_DOpct doPct_{};

        // Number of sensors used
        static const uint8_t SENSOR_COUNT = 13;
        // List of the variables
        Variable* list_[SENSOR_COUNT] = {&statsSampleNo_, &statsRam_, &statsBattery_, &modemRSSI_,  &rtcTemp_,
                                         &rtdTemp_,       &ecCond_,   &ecTDS_,        &ecSalinity_, &ecGravity_,
                                         &phPH_,          &doMgl_,    &doPct_};
        // Fully-constructed variable array
        VariableArray variableArray_{SENSOR_COUNT, list_};
    };

} // namespace timu::variable_interface