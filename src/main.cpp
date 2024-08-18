#include <Arduino.h>
#include <BoardInterface.h>

timu::Station mayflyStation{};

void setup() { mayflyStation.setup(); }

void loop() { mayflyStation.work(); }