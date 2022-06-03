#ifndef UUIDS_H
#define UUIDS_H

#if STATION == 1
const char *UUIDs[] =  // UUID array for device sensors
    {
        "c2d77237-5f4a-4fc5-85e0-b65f0c36c689",  // Sequence number (EnviroDIY_Mayfly_SampleNum)
        "9d4cb12c-083d-45da-b8f2-31f2c4b0f7eb",  // Free SRAM (EnviroDIY_Mayfly_FreeRAM)
        "2f54c561-06c9-4794-be72-ecb3c0f33a89",  // Battery voltage (EnviroDIY_Mayfly_Batt)
        "b43ff7e0-8831-4ecc-a4b8-c5cee1f6b266",  // Temperature (Maxim_DS3231_Temp)
        "2a20259b-9935-4e86-928b-259778377d75",  // Received signal strength indication (Digi_Cellular_RSSI)
        "b0d1cb75-3442-460a-8b9c-471fd657e53f",  // Temperature (Atlas_Temp)
        "b6372e58-e280-4d6a-a15d-e4cd6215f2b7",  // Electrical conductivity (Atlas_Conductivity)
        "714bdd0d-02bf-4e62-a389-4be63cf7b7ec",  // Solids, total dissolved (Atlas_TDS)
        "930c90bb-5ffe-49ee-b50d-5246f5ebd52b",  // Salinity (Atlas_Salinity)
        "7e44217d-527d-4020-a774-3115c2c27a0e",  // Gage height (Atlas_SpecificGravity)
        "9bd85870-801d-4b8a-8275-5c47cbe5d794",  // pH (Atlas_pH)
        "be51f2d0-7406-43d0-8fd3-d6ca9cab55ec",  // Oxygen, dissolved (Atlas_DOconc)
        "6bdb35da-26f5-4d22-a623-ff2e6fef32a8"   // Oxygen, dissolved percent of saturation (Atlas_DOpct)
};

const char *registrationToken =
    "c03aa02d-7a06-4c5b-9d97-1829b7ec0b92";  // Device registration token
const char *samplingFeature =
    "797d8c01-904e-4468-a327-599806509fd6";  // Sampling feature UUID

#endif

#if STATION == 2

const char *UUIDs[] =                                                      // UUID array for device sensors
{
    "34297ef4-c4d0-47c3-a928-0a0030652d1b",   // Sequence number (EnviroDIY_Mayfly_SampleNum)
    "d088dbac-07a2-4eee-9f76-935c0c1f87af",   // Free SRAM (EnviroDIY_Mayfly_FreeRAM)
    "6702d54f-32b5-445f-ac5d-000020501362",   // Battery voltage (EnviroDIY_Mayfly_Batt)
    "cc008c4e-3cae-49ae-9d0c-e395664bd528",   // Temperature (Maxim_DS3231_Temp)
    "205d2e97-aa67-4b13-8af9-0909bb00e833",   // Received signal strength indication (Digi_Cellular_RSSI)
    "bf62a74e-131e-4e3a-b754-d5eb6e9a6382",   // Temperature (Atlas_Temp)
    "0435cf43-66f2-47c3-929e-ffda88914170",   // Electrical conductivity (Atlas_Conductivity)
    "db9e85d1-d06d-4868-9fb2-17d11a3bdfb3",   // Solids, total dissolved (Atlas_TDS)
    "e8784069-90ac-4d5d-af34-fa73b5a3164e",   // Salinity (Atlas_Salinity)
    "282e67a3-0bb6-460a-bddc-56fceca22225",   // Gage height (Atlas_SpecificGravity)
    "68801968-326a-450f-b801-ac5f24769342",   // pH (Atlas_pH)
    "49f53bf1-51e6-4037-84b8-892b3a84f2fc",   // Oxygen, dissolved (Atlas_DOconc)
    "4f93ac76-0f12-48c8-b4d7-47bf033dbf9f"    // Oxygen, dissolved percent of saturation (Atlas_DOpct)
};
const char *registrationToken = "5c9fc489-8a2f-41f5-8254-816253de8d8a";   // Device registration token
const char *samplingFeature = "e352c3fd-4bb0-4864-9390-e33e960db357";     // Sampling feature UUID


#endif



#endif