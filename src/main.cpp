// === jax_logger.cpp =========================================================
// author: jake jones
// contact: jake@oakst.io
// ~ adapted from single_sensor by Sara Geleskie Damiano
// ============================================================================

// === ESSENTIAL INCLUDES =====================================================
// ============================================================================
#include <Arduino.h>
#include <EnableInterrupt.h>
#include <ModularSensors.h>
#include <UUIDs.h>

// === DATA LOGGING OPTIONS ===================================================
// ============================================================================
const char *sketchName = "main.cpp";
// Logger ID, also becomes the prefix for the name of the data file on SD card
const char *LoggerID = "0001";
// How frequently (in minutes) to log data
const uint8_t loggingInterval = 5;
// Your logger's timezone.
const int8_t timeZone = -4;  // Eastern Standard Time
// NOTE:  Daylight savings time will not be applied!  Please use standard time!
// Set the input and output pins for the logger
// NOTE:  Use -1 for pins that do not apply
const int32_t serialBaud = 115200;  // Baud rate for debugging
const int8_t greenLED = 8;          // Pin for the green LED
const int8_t redLED = 9;            // Pin for the red LED
const int8_t buttonPin = 21;        // Pin for debugging mode (ie, button pin)
const int8_t wakePin = 31;          // MCU interrupt/alarm pin to wake from sleep
// Set the wake pin to -1 if you do not want the main processor to sleep.
const int8_t sdCardPwrPin = -1;    // MCU SD card power pin
const int8_t sdCardSSPin = 12;     // SD card chip select/slave select pin
const int8_t sensorPowerPin = 22;  // MCU pin controlling main sensor power

// === MODEM SETUP ============================================================
// ============================================================================
#include <modems/SIMComSIM7080.h>

HardwareSerial &modemSerial = Serial1; 
const int32_t modemBaud = 9600;  //  SIM7080 does auto-bauding by default
const int8_t modemVccPin = 18;  
const int8_t modemStatusPin = 19; 
const int8_t modemSleepRqPin = 23;  // MCU pin for modem sleep/wake request
const int8_t modemLEDPin = redLED;  // MCU pin connected an LED to show modem status

const char *apn = "hologram";  // The APN for the gprs connection

// Create the modem object
SIMComSIM7080 modem7080(&modemSerial, modemVccPin, modemStatusPin, modemSleepRqPin, apn);
SIMComSIM7080 modem = modem7080;


// === SENSOR SETUP ===========================================================
// ============================================================================

// === Processor Sensor ===============
// ====================================
#include <sensors/ProcessorStats.h>
// Create the main processor chip "sensor" - for general metadata
const char *mcuBoardVersion = "v1.0";
ProcessorStats mcuBoard(mcuBoardVersion);
Variable *processorSampleNum = new ProcessorStats_SampleNumber(&mcuBoard);
Variable *processorFreeRAM = new ProcessorStats_FreeRam(&mcuBoard);
Variable *processorBattery = new ProcessorStats_Battery(&mcuBoard);

// === Modem Sensor ===================
// ====================================
Variable *modemRSSI = new Modem_RSSI(&modem);
Variable *modemSignalPct = new Modem_SignalPercent(&modem);
Variable *modemBatteryState = new Modem_BatteryState(&modem);
Variable *modemBatteryPct = new Modem_BatteryPercent(&modem);
Variable *modemBatteryVoltage = new Modem_BatteryVoltage(&modem);
Variable *modemTemperature = new Modem_Temp(&modem);

// === Maxim DS3231 RTC ===============
// ====================================
#include <sensors/MaximDS3231.h>
MaximDS3231 ds3231(1);
Variable *rtcTemp = new MaximDS3231_Temp(&ds3231);

// === Atlas Scientific RTD ===========
// ====================================
#include <sensors/AtlasScientificRTD.h>
const int8_t AtlasRTDPower = -1;  // Power pin (-1 if unconnected)
AtlasScientificRTD atlasRTD(AtlasRTDPower);
Variable *atlasTemp = new AtlasScientificRTD_Temp(&atlasRTD);

// === Atlas Scientific EC ============
// ====================================
#include <sensors/AtlasScientificEC.h>
const int8_t AtlasECPower = -1;  // Power pin (-1 if unconnected)
AtlasScientificEC atlasEC(AtlasECPower);
// Create four variable pointers for the EZO-ES
Variable *atlasCond = new AtlasScientificEC_Cond(&atlasEC);
Variable *atlasTDS = new AtlasScientificEC_TDS(&atlasEC);
Variable *atlasSal = new AtlasScientificEC_Salinity(&atlasEC);
Variable *atlasGrav = new AtlasScientificEC_SpecificGravity(&atlasEC);

// Calculate the specific EC
float calculateAtlasSpCond(void) {
    float spCond = -9999;  // Always safest to start with a bad value
    float waterTemp = atlasTemp->getValue();
    float rawCond = atlasCond->getValue();
    // ^^ Linearized temperature correction coefficient per degrees Celsius.
    // The value of 0.019 comes from measurements reported here:
    // Hayashi M. Temperature-electrical conductivity relation of water for
    // environmental monitoring and geophysical data inversion. Environ Monit
    // Assess. 2004 Aug-Sep;96(1-3):119-28.
    // doi: 10.1023/b:emas.0000031719.83065.68. PMID: 15327152.
    if (waterTemp != -9999 && rawCond != -9999) {
        // make sure both inputs are good
        float temperatureCoef = 0.019;
        spCond = rawCond / (1 + temperatureCoef * (waterTemp - 25.0));
    }
    return spCond;
}

// Properties of the calculated variable
// The number of digits after the decimal place
const uint8_t atlasSpCondResolution = 0;
// This must be a value from http://vocabulary.odm2.org/variablename/
const char *atlasSpCondName = "specificConductance";
// This must be a value from http://vocabulary.odm2.org/units/
const char *atlasSpCondUnit = "microsiemenPerCentimeter";
// A short code for the variable
const char *atlasSpCondCode = "atlasSpCond";
// Finally, create the specific conductance variable and return a pointer to it
Variable *atlasSpCond =
    new Variable(calculateAtlasSpCond, atlasSpCondResolution, atlasSpCondName,
                 atlasSpCondUnit, atlasSpCondCode);

// === AtlasScientific pH =============
// ====================================
#include <sensors/AtlasScientificpH.h>
const int8_t AtlaspHPower = -1;  // Power pin (-1 if unconnected)
AtlasScientificpH atlaspH(AtlaspHPower);
Variable *atlaspHpH = new AtlasScientificpH_pH(&atlaspH);

// === AtlasScientific DO =============
// ====================================
#include <sensors/AtlasScientificDO.h>
const int8_t AtlasDOPower = -1;  // Power pin (-1 if unconnected)
AtlasScientificDO atlasDO(AtlasDOPower);
Variable *atlasDOconc = new AtlasScientificDO_DOmgL(&atlasDO);
Variable *atlasDOpct = new AtlasScientificDO_DOpct(&atlasDO);

// === LOGGER SETUP ===========================================================
// ============================================================================
// There's a few more sensors that can be logged by they aren't that useful
// The atlasSpCond is a 'calculated' reading and is not supported by MMW
Variable *variableList[] = {
    processorSampleNum,
    processorFreeRAM,
    processorBattery,
    rtcTemp,
    modemRSSI,
    // modemSignalPct,
    // modemBatteryState,
    // modemBatteryVoltage,
    // modemBatteryPct,
    // modemTemperature,
    atlasTemp,
    atlasCond,
    // atlasSpCond,
    atlasTDS,
    atlasSal,
    atlasGrav,
    atlaspHpH,
    atlasDOconc,
    atlasDOpct};

// UUIDs as taken from the MMW website. The order matters and must match the variableList!
// For security reasons these UUIDS *probably* shouldn't be made public

// Count up the number of pointers in the array
int variableCount = sizeof(variableList) / sizeof(variableList[0]);
VariableArray varArray(variableCount, variableList, UUIDs);
Logger dataLogger(LoggerID, loggingInterval, &varArray);

// === DATA PUBLISHER SETUP ===================================================
// ============================================================================

// MMW identity tokens
// These definitely shouldn't be made public

// Create a data publisher for the Monitor My Watershed/EnviroDIY POST endpoint
#include <publishers/EnviroDIYPublisher.h>
EnviroDIYPublisher EnviroDIYPOST(dataLogger, &modem.gsmClient,
                                 registrationToken, samplingFeature);

// === OTHER FUNCTIONS ========================================================
// ============================================================================

// Flashes the LED's on the primary board
void greenredflash(uint8_t numFlash = 4, uint8_t rate = 75) {
    for (uint8_t i = 0; i < numFlash; i++) {
        digitalWrite(greenLED, HIGH);
        digitalWrite(redLED, LOW);
        delay(rate);
        digitalWrite(greenLED, LOW);
        digitalWrite(redLED, HIGH);
        delay(rate);
    }
    digitalWrite(redLED, LOW);
}

// Returns the battery voltage
float getBatteryVoltage() {
    if (mcuBoard.sensorValues[0] == -9999)
        mcuBoard.update();
    return mcuBoard.sensorValues[0];
}

// === MAIN SETUP =============================================================
// ============================================================================
void setup() {

    digitalWrite(22, HIGH);
    // Start the primary serial connection
    Serial.begin(serialBaud);

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

    // Start the serial connection with the modem
    modemSerial.begin(modemBaud);

    // Set up pins for the LED's
    pinMode(greenLED, OUTPUT);
    digitalWrite(greenLED, LOW);
    pinMode(redLED, OUTPUT);
    digitalWrite(redLED, LOW);
    // Blink the LEDs to show the board is on and starting up
    greenredflash();

    // Set the timezones for the logger/data and the RTC
    Logger::setLoggerTimeZone(timeZone);
    // It is STRONGLY RECOMMENDED that you set the RTC to be in UTC (UTC+0)
    Logger::setRTCTimeZone(0);

    // Attach the modem and information pins to the logger
    dataLogger.attachModem(modem);
    modem.setModemLED(modemLEDPin);
    // modem.modemWake();
    dataLogger.setLoggerPins(wakePin, sdCardSSPin, sdCardPwrPin, buttonPin,
                             greenLED);

    // Begin the variable array[s], logger[s], and publisher[s]
    dataLogger.begin();

    // Set up the sensors, except at lowest battery level
    if (getBatteryVoltage() > 3.4) {
        Serial.println(F("Setting up sensors..."));
        varArray.setupSensors();
    }

    /** Start [setup_sim7080] */
    modem.setModemWakeLevel(HIGH);   // ModuleFun Bee inverts the signal
    modem.setModemResetLevel(HIGH);  // ModuleFun Bee inverts the signal
    Serial.println(F("Waking modem and setting Cellular Carrier Options..."));
    modem.modemWake();  // NOTE:  This will also set up the modem
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
    /** End [setup_sim7080] */

    // Sync the clock if it isn't valid or we have battery to spare
    if (getBatteryVoltage() > 3.55 || !dataLogger.isRTCSane()) {
        // Synchronize the RTC with NIST
        // This will also set up the modem
        dataLogger.syncRTC();
    }

    // Create the log file, adding the default header to it
    // Do this last so we have the best chance of getting the time correct and
    // all sensor names correct
    // Writing to the SD card can be power intensive, so if we're skipping
    // the sensor setup we'll skip this too.
    if (getBatteryVoltage() > 3.4) {
        Serial.println(F("Setting up file on SD card"));
        dataLogger.turnOnSDcard(
            true);                       // true = wait for card to settle after power up
        dataLogger.createLogFile(true);  // true = write a new header
        dataLogger.turnOffSDcard(
            true);  // true = wait for internal housekeeping after write
    }

    // Call the processor sleep
    Serial.println(F("Putting processor to sleep\n"));
    dataLogger.systemSleep();
}

// === MAIN LOOP ==============================================================
// ============================================================================
void loop() {
    if (getBatteryVoltage() < 3.4) {
        dataLogger.systemSleep();
    }
    // At moderate voltage, log data but don't send it over the modem
    else if (getBatteryVoltage() < 3.55) {
        dataLogger.logData();
    }
    // If the battery is good, send the data to the world
    else {
        dataLogger.logDataAndPublish();
    }
}