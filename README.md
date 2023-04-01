# TIMU-Mayfly

Code for the TIMU data loggings stations

## Manual Mode / Calibration

By default, the stations will log data every five minutes. To calibrate and test the sensors, the station needs to be put into a "manual mode". Manual mode wakes the station from sleep, stops the automatic logging, and allows you to sent I2C commands directly to the EZO circuits via serial.

To put the station into manual mode, press the tiny button behind the SD card labeled "D21 Button".  Open up a serial monitor at baud rate 57600. The serial monitor will greet you with a prompt and syntax help.

The syntax for sending an EZO circuit is as follows:

    [SENSOR] [COMMAND]
    
where `[SENSOR]` is either `DO`, `ORP`, `PH`, `EC`, or `RTD` and the command is the I2C command as found in the EZO datasheets.

For example, sending `EC R` takes a reading from the electrical conductivity sensor.

To exit manual mode, type `EXIT`. This re-enables automatic logging!

## Datasheets

 - [DO](https://files.atlas-scientific.com/DO_EZO_Datasheet.pdf)
 - [ORP](https://files.atlas-scientific.com/ORP_EZO_Datasheet.pdf)
 - [PH](https://files.atlas-scientific.com/pH_EZO_Datasheet.pdf)
 - [EC](https://files.atlas-scientific.com/EC_EZO_Datasheet.pdf)
 - [RTD](https://files.atlas-scientific.com/EZO_RTD_Datasheet.pdf)


