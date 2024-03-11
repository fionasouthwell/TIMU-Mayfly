#ifndef HELPERS_H
#define HELPERS_H

#include <Arduino.h>
#include <ModularSensors.h>
#include <modems/SIMComSIM7080.h>
#include <sensors/ProcessorStats.h>
#include <PINS.h>

float getBatteryVoltage( ProcessorStats &mcuBoard);
void setup_leds(uint8_t numFlash = 4, uint8_t rate = 75);
void configure_modem(SIMComSIM7080 &modem, int32_t modemBaud);
void print_start_msg(const char *sketchName, const char *LoggerID);
void configure_logger(Logger &logger, SIMComSIM7080 &modem);
void turn_on_shield(void);
void turn_off_shield(void);

#endif