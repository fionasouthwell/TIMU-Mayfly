float calculateAtlasSpCond(float waterTemp, float rawCond) {
    float spCond = -9999;  // Always safest to start with a bad value
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