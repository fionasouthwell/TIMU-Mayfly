#include <Arduino.h>
#include <BoardInterface.h>

// Global Station object
timu::Station mayflyStation{};

void setup()
{
    // Setup the station
    mayflyStation.setup();
}

void loop()
{
    // Log data whenever the processor reawakes
    mayflyStation.log();
}