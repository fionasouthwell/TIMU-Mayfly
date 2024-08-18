#pragma once

#if STATION == 1
const char *UUIDs[] =  // UUID array for device sensors
    {
 o
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
