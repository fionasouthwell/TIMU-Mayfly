#include <Arduino.h>
#include <StationInterface/StationInterface.h>

// Global Station object
timu::station_interface::Station mayflyStation{};

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