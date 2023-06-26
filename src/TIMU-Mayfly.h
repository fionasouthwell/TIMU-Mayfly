#ifndef TIMU_MAYFLY_H
#define TIMU_MAYFLY_H

#include <Arduino.h>
#include <EnableInterrupt.h>
#include <ModularSensors.h>
#include <UUIDs.h>
#include <PINS.h>
#include <helpers.h>

#include <modems/SIMComSIM7080.h>
#include <sensors/ProcessorStats.h>
#include <sensors/MaximDS3231.h>
#include <sensors/AtlasScientificRTD.h>
#include <sensors/AtlasScientificEC.h>
#include <sensors/AtlasScientificpH.h>
#include <sensors/AtlasScientificDO.h>

float calculateAtlasSpCond(float waterTemp, float rawCond);

#endif