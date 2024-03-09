#include <ModularSensors.h>
#include <TIMU-Logger.h>
#include <helpers.h>

void TIMU_Logger::logData(void) {
    // Reset the watchdog
    watchDogTimer.resetWatchDog();

    // Assuming we were woken up by the clock, check if the current time is an
    // even interval of the logging interval
    if (checkInterval()) {
        // Flag to notify that we're in already awake and logging a point
        TIMU_Logger::isLoggingNow = true;
        // Reset the watchdog
        watchDogTimer.resetWatchDog();

        // Power on shield before taking the readings
        turn_on_shield();
        delay(1000);

        // Print a line to show new reading
        PRINTOUT(F("------------------------------------------"));
        // Turn on the LED to show we're taking a reading
        alertOn();
        // Power up the SD Card
        // TODO(SRGDamia1):  Decide how much delay is needed between turning on
        // the card and writing to it.  Could we turn it on just before writing?
        turnOnSDcard(false);

        // Do a complete sensor update
        MS_DBG(F("    Running a complete sensor update..."));
        watchDogTimer.resetWatchDog();
        _internalArray->completeUpdate();
        watchDogTimer.resetWatchDog();

        // Create a csv data record and save it to the log file
        logToSD();
        // Cut power from the SD card, waiting for housekeeping
        turnOffSDcard(true);

        // Turn off the LED
        alertOff();
        // Print a line to show reading ended
        PRINTOUT(F("------------------------------------------\n"));

        // Unset flag
        TIMU_Logger::isLoggingNow = false;
    }

    // Check if it was instead the testing interrupt that woke us up
    if (TIMU_Logger::startTesting) testingMode();

    // Power off shiled before sleeping
    turn_off_shield();
    systemSleep();
}

void TIMU_Logger::logDataAndPublish(void) {
    // Reset the watchdog
    watchDogTimer.resetWatchDog();

    // Assuming we were woken up by the clock, check if the current time is an
    // even interval of the logging interval
    if (checkInterval()) {
        // Flag to notify that we're in already awake and logging a point
        TIMU_Logger::isLoggingNow = true;
        // Reset the watchdog
        watchDogTimer.resetWatchDog();

        // Turn on shield before taking a reading
        turn_on_shield();
        delay(1000);

        // Print a line to show new reading
        PRINTOUT(F("------------------------------------------"));
        // Turn on the LED to show we're taking a reading
        alertOn();
        // Power up the SD Card
        // TODO(SRGDamia1):  Decide how much delay is needed between turning on
        // the card and writing to it.  Could we turn it on just before writing?
        turnOnSDcard(false);

        // Do a complete update on the variable array.
        // This this includes powering all of the sensors, getting updated
        // values, and turing them back off.
        // NOTE:  The wake function for each sensor should force sensor setup to
        // run if the sensor was not previously set up.
        MS_DBG(F("Running a complete sensor update..."));
        watchDogTimer.resetWatchDog();
        _internalArray->completeUpdate();
        watchDogTimer.resetWatchDog();

// Print out the sensor data
#if defined(STANDARD_SERIAL_OUTPUT)
        MS_DBG('\n');
        _internalArray->printSensorData(&STANDARD_SERIAL_OUTPUT);
        MS_DBG('\n');
#endif

        // Create a csv data record and save it to the log file
        logToSD();

        if (_logModem != nullptr) {
            MS_DBG(F("Waking up"), _logModem->getModemName(), F("..."));
            if (_logModem->modemWake()) {
                // Connect to the network
                watchDogTimer.resetWatchDog();
                MS_DBG(F("Connecting to the Internet..."));
                if (_logModem->connectInternet()) {
                    // Publish data to remotes
                    watchDogTimer.resetWatchDog();
                    publishDataToRemotes();
                    watchDogTimer.resetWatchDog();

                    if ((TIMU_Logger::markedLocalEpochTime != 0 &&
                         TIMU_Logger::markedLocalEpochTime % 86400 == 43200) ||
                        !isRTCSane(TIMU_Logger::markedLocalEpochTime)) {
                        // Sync the clock at noon
                        MS_DBG(F("Running a daily clock sync..."));
                        setRTClock(_logModem->getNISTTime());
                        watchDogTimer.resetWatchDog();
                    }

                    // Update the modem metadata
                    MS_DBG(F("Updating modem metadata..."));
                    _logModem->updateModemMetadata();

                    // Disconnect from the network
                    MS_DBG(F("Disconnecting from the Internet..."));
                    _logModem->disconnectInternet();
                } else {
                    MS_DBG(F("Could not connect to the internet!"));
                    watchDogTimer.resetWatchDog();
                }
            }
            // Turn the modem off
            _logModem->modemSleepPowerDown();
        }

        // Cut power from the SD card - without additional housekeeping wait
        // TODO(SRGDamia1):  Do some sort of verification that minimum 1 sec has
        // passed for internal SD card housekeeping before cutting power -
        // although it seems very unlikely based on my testing that less than
        // one second would be taken up in publishing data to remotes.
        turnOffSDcard(false);

        // Turn off the LED
        alertOff();
        // Print a line to show reading ended
        PRINTOUT(F("------------------------------------------\n"));

        // Unset flag
        TIMU_Logger::isLoggingNow = false;
    }

    // Check if it was instead the testing interrupt that woke us up
    if (TIMU_Logger::startTesting) testingMode();

    // turn off shield before sleeping
    turn_off_shield();

    // Call the processor sleep
    systemSleep();
}