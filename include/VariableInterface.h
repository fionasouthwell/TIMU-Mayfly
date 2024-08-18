#pragma once

#include "SensorInterface.h"
#include "UUIDs.h"

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
    class VariableInterface
    {
      public:
        explicit VariableInterface(timu::SensorInterface& sensorIface)
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

        auto getList() -> VariableArray& { return variableArray_; }

      private:
        ProcessorStats_SampleNumber statsSampleNo_{};
        ProcessorStats_FreeRam statsRam_{};

        ProcessorStats_Battery statsBattery_{};

        Modem_RSSI modemRSSI_;

        MaximDS3231_Temp rtcTemp_{};

        AtlasScientificRTD_Temp rtdTemp_{};

        AtlasScientificEC_Cond ecCond_{};
        AtlasScientificEC_Salinity ecSalinity_{};
        AtlasScientificEC_SpecificGravity ecGravity_{};
        AtlasScientificEC_TDS ecTDS_{};

        AtlasScientificpH_pH phPH_{};

        AtlasScientificDO_DOmgL doMgl_{};
        AtlasScientificDO_DOpct doPct_{};

        static const uint8_t SENSOR_COUNT = 13;
        Variable* list_[SENSOR_COUNT] = {&statsSampleNo_, &statsRam_, &statsBattery_, &modemRSSI_,  &rtcTemp_,
                                         &rtdTemp_,       &ecCond_,   &ecTDS_,        &ecSalinity_, &ecGravity_,
                                         &phPH_,          &doMgl_,    &doPct_};
        VariableArray variableArray_{SENSOR_COUNT, list_};
    };

} // namespace timu