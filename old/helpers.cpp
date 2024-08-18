#include <Arduino.h>
#include <ModularSensors.h>
#include <modems/SIMComSIM7080.h>
#include <sensors/ProcessorStats.h>
#include <PINS.h>

void turn_on_shield(void){
    digitalWrite(PIN_REG_3V3, HIGH);
}

void configure_logger(Logger &dataLogger, SIMComSIM7080 &modem) {
    dataLogger.attachModem(modem);
    dataLogger.setLoggerPins(PIN_WAKE, PIN_SD_SS, PIN_SD_PWR, PIN_BUTTON_DEBUG, PIN_LED_GREEN);
    dataLogger.begin();
}

void configure_modem(SIMComSIM7080 &modem, int32_t modemBaud) {
    modem.setModemLED(PIN_MODEM_LED);
    modem.setModemWakeLevel(HIGH);   // ModuleFun Bee inverts the signal
    modem.setModemResetLevel(HIGH);  // ModuleFun Bee inverts the signal
    Serial.println(F("Waking modem and setting Cellular Carrier Options..."));
    modem.modemWake();                   // NOTE:  This will also set up the modem
    modem.gsmModem.setBaud(modemBaud);   // Make sure we're *NOT* auto-bauding!
    modem.gsmModem.setNetworkMode(38);   // set to LTE only
                                         // 2 Automatic
                                         // 13 GSM only
                                         // 38 LTE only
                                         // 51 GSM and LTE only
    modem.gsmModem.setPreferredMode(1);  // set to CAT-M
                                         // 1 CAT-M
                                         // 2 NB-IoT
                                         // 3 CAT-M and NB-IoT
}

void print_start_msg(const char *sketchName, const char *LoggerID){
        // Print a start-up note to the first serial port
    Serial.print(F("Now running "));
    Serial.print(sketchName);
    Serial.print(F(" on Logger "));
    Serial.println(LoggerID);
    Serial.println();

    Serial.print(F("Using ModularSensors Library version "));
    Serial.println(MODULAR_SENSORS_VERSION);
    Serial.print(F("TinyGSM Library version "));
    Serial.println(TINYGSM_VERSION);
    Serial.println();
}

void setup_leds(uint8_t numFlash, uint8_t rate) {
    pinMode(PIN_LED_GREEN, OUTPUT);
    digitalWrite({PIN_LED_GREEN}, LOW);
    pinMode(PIN_LED_RED, OUTPUT);
    digitalWrite(PIN_LED_RED, LOW);
    // flash em
    for (uint8_t i = 0; i < numFlash; i++) {
        digitalWrite(PIN_LED_GREEN, HIGH);
        digitalWrite(PIN_LED_RED, LOW);
        delay(rate);
        digitalWrite(PIN_LED_GREEN, LOW);
        digitalWrite(PIN_LED_RED, HIGH);
        delay(rate);
    }
    digitalWrite(PIN_LED_RED, LOW);
}

float getBatteryVoltage( ProcessorStats &mcuBoard) {
    if (mcuBoard.sensorValues[0] == -9999)
        mcuBoard.update();
    return mcuBoard.sensorValues[0];
}