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
#include <PINS.h>
#include <TIMU-Mayfly.h>
#include <UUIDs.h>

// === DATA LOGGING OPTIONS ===================================================
// ============================================================================
const char *sketchName = "main.cpp";
// Logger ID, also becomes the prefix for the name of the data file on SD card
const char *LoggerID = "0001";
// How frequently (in minutes) to log data
const uint8_t loggingInterval = 5;
// Your logger's timezone.
const int8_t timeZone = -4; // Eastern Standard Time
// NOTE:  Daylight savings time will not be applied!  Please use standard time!

// === MODEM SETUP ============================================================
// ============================================================================
HardwareSerial &modemSerial = Serial1;
const int32_t modemBaud = 9600; //  SIM7080 does auto-bauding by default
const char *apn = "hologram";   // The APN for the gprs connection
SIMComSIM7080 modem(&modemSerial, PIN_MODEM_PWR, PIN_MODEM_STATUS, PIN_MODEM_SLEEP_RQ, apn);

// === SENSOR SETUP ===========================================================
// ============================================================================

// === Processor Sensor ===============
// ====================================

// Create the main processor chip "sensor" - for general metadata
ProcessorStats mcuBoard("v1.0");
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
MaximDS3231 ds3231(1);
Variable *rtcTemp = new MaximDS3231_Temp(&ds3231);

// === Atlas Scientific RTD ===========
// ====================================
AtlasScientificRTD atlasRTD(-1);
Variable *atlasTemp = new AtlasScientificRTD_Temp(&atlasRTD);

// === Atlas Scientific EC ============
// ====================================
AtlasScientificEC atlasEC(-1);
// Create four variable pointers for the EZO-ES
Variable *atlasCond = new AtlasScientificEC_Cond(&atlasEC);
Variable *atlasTDS = new AtlasScientificEC_TDS(&atlasEC);
Variable *atlasSal = new AtlasScientificEC_Salinity(&atlasEC);
Variable *atlasGrav = new AtlasScientificEC_SpecificGravity(&atlasEC);

// === AtlasScientific pH =============
// ====================================
AtlasScientificpH atlaspH(-1);
Variable *atlaspHpH = new AtlasScientificpH_pH(&atlaspH);

// === AtlasScientific DO =============
// ====================================
AtlasScientificDO atlasDO(-1);
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
    atlasTemp,
    atlasCond,
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

// Were using a modified version of Logger
// The default Logger automatically puts in the mayfly to sleep
// Logger changes this
Logger dataLogger(LoggerID, loggingInterval, &varArray);

// === DATA PUBLISHER SETUP ===================================================
// ============================================================================

// MMW identity tokens
// These definitely shouldn't be made public

// Create a data publisher for the Monitor My Watershed/EnviroDIY POST endpoint
#include <publishers/EnviroDIYPublisher.h>
EnviroDIYPublisher EnviroDIYPOST(dataLogger, &modem.gsmClient,
                                 registrationToken, samplingFeature);

// === MAIN SETUP =============================================================
// ============================================================================

void setup()
{
    // Start the serial connections
    modemSerial.begin(modemBaud);
    Wire.begin();

    // Set the timezones for the logger/data and the RTC
    Logger::setLoggerTimeZone(timeZone);
    Logger::setRTCTimeZone(0);

    print_start_msg(sketchName, LoggerID);
    setup_leds();
    turn_on_shield();

    // Attach the modem and information pins to the logger
    configure_logger(dataLogger, modem);

    // Set up the sensors, except at lowest battery level
    if (getBatteryVoltage(mcuBoard) > 3.4)
    {
        Serial.println(F("Setting up sensors..."));
        varArray.setupSensors();
    }

    configure_modem(modem, modemBaud);

    // Sync the clock if it isn't valid or we have battery to spare
    if (getBatteryVoltage(mcuBoard) > 3.55 || !dataLogger.isRTCSane())
    {
        // Synchronize the RTC with NIST
        // This will also set up the modem
        dataLogger.syncRTC();
    }

    // Create the log file, adding the default header to it
    // Do this last so we have the best chance of getting the time correct and all sensor names correct
    // Writing to the SD card can be power intensive, so if we're skipping the sensor setup we'll skip this too.
    if (getBatteryVoltage(mcuBoard) > 3.4)
    {
        Serial.println(F("Setting up file on SD card"));
        dataLogger.turnOnSDcard(true);  // true = wait for card to settle after power up
        dataLogger.createLogFile(true); // true = write a new header
        dataLogger.turnOffSDcard(true); // true = wait for internal housekeeping after write
    }

    Serial.println(F("Putting processor to sleep\n"));
    // turn_off_shield();
    dataLogger.systemSleep();
}

// === MAIN LOOP ==============================================================
// ============================================================================
void loop()
{

    if (getBatteryVoltage(mcuBoard) < 3.4)
    {
        dataLogger.systemSleep();
    }
    // At moderate voltage, log data but don't send it over the modem
    else if (getBatteryVoltage(mcuBoard) < 3.55)
    {
        dataLogger.logData();
    }
    // If the battery is good, send the data to the world
    else
    {
        dataLogger.logDataAndPublish();
    }
}