#ifndef TIMU_MAYFLY_H
#define TIMU_MAYFLY_H

#include <Arduino.h>
#include <EnableInterrupt.h>
#include <ModularSensors.h>
#include <UUIDs.h>
#include <PINS.h>

#include <modems/SIMComSIM7080.h>
#include <sensors/ProcessorStats.h>
#include <sensors/MaximDS3231.h>
#include <sensors/AtlasScientificRTD.h>
#include <sensors/AtlasScientificEC.h>
#include <sensors/AtlasScientificpH.h>
#include <sensors/AtlasScientificDO.h>

float calculateAtlasSpCond(float waterTemp, float rawCond);
float getBatteryVoltage( ProcessorStats &mcuBoard);
void setup_leds(uint8_t numFlash = 4, uint8_t rate = 75);
void configure_modem(SIMComSIM7080 &modem, int32_t modemBaud);
void print_start_msg(const char *sketchName, const char *LoggerID);
void configure_logger(Logger &logger, SIMComSIM7080 &modem);
void turn_on_shield(void);
#endif