#pragma once

namespace station
{
    // Station ID
    const auto STATION_ID = "0001";
    // File name to write to SD card
    const auto FILE_NAME = "main.cpp";
    // How often to log data, minutes
    const auto LOGGING_INTERVAL = 5U;
} // namespace station

namespace uuids
{

    // MMW Registration Token
    const auto TOKEN = "c03aa02d-7a06-4c5b-9d97-1829b7ec0b92";
    // MMW Sampling Feature
    const auto SAMPLING_FEATURE = "797d8c01-904e-4468-a327-599806509fd6";
    // Sequence number (EnviroDIY_Mayfly_SampleNum)
    const auto SEQUENCE_NUMBER = "c2d77237-5f4a-4fc5-85e0-b65f0c36c689";
    // Free SRAM (EnviroDIY_Mayfly_FreeRAM)
    const auto FREE_SRAM = "9d4cb12c-083d-45da-b8f2-31f2c4b0f7eb";
    // Battery voltage (EnviroDIY_Mayfly_Batt)
    const auto BATTERY_VOLTAGE = "2f54c561-06c9-4794-be72-ecb3c0f33a89";
    // Real-Time Clock Temperature (Maxim_DS3231_Temp)
    const auto RTC_TEMPERATURE = "b43ff7e0-8831-4ecc-a4b8-c5cee1f6b266";
    // Received signal strength indication (Digi_Cellular_RSSI)
    const auto SIGNAL_STRENGTH = "2a20259b-9935-4e86-928b-259778377d75";
    // Temperature (Atlas_Temp)
    const auto TEMPERATURE = "b0d1cb75-3442-460a-8b9c-471fd657e53f";
    // Electrical conductivity (Atlas_Conductivity)
    const auto CONDUCTIVITY = "b6372e58-e280-4d6a-a15d-e4cd6215f2b7";
    // Solids, total dissolved (Atlas_TDS)
    const auto TOTAL_DISSOLVED_SOLIDS = "714bdd0d-02bf-4e62-a389-4be63cf7b7ec";
    // Salinity (Atlas_Salinity)
    const auto SALINITY = "930c90bb-5ffe-49ee-b50d-5246f5ebd52b";
    // Gage height (Atlas_SpecificGravity)
    const auto GAGE_HEIGHT = "7e44217d-527d-4020-a774-3115c2c27a0e";
    // pH (Atlas_pH)
    const auto WATER_PH = "9bd85870-801d-4b8a-8275-5c47cbe5d794";
    // Oxygen, dissolved (Atlas_DOconc)
    const auto DISSOLVED_OXYGEN = "be51f2d0-7406-43d0-8fd3-d6ca9cab55ec";
    // Oxygen, dissolved percent of saturation (Atlas_DOpct)
    const auto DISSOLVED_OXYGEN_PERCENT = "6bdb35da-26f5-4d22-a623-ff2e6fef32a8";
} // namespace uuids