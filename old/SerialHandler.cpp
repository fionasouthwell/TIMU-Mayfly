#include <SerialHandler.h>

// process_buffer -> parse_sensor -> send_command

// process the rx_buffer
void SerialHandler::process_buffer(char (&rx_buffer)[32]) {
    // extract sensor and command tokens
    sensor = strtok(rx_buffer, " ");
    command = strtok(NULL, " ");
    // convert sensor string into address number
    address = parse_sensor();

    switch (address) {
        case EZO_HELP:
            print_help();
            break;
        case EZO_EXIT:
            Serial.println("Exiting manual mode!");
            exit_flag = 1;
            break;
        case EZO_NONE:
            print_not_recognized();
            break;
        default:
            // if we got here, address is valid
            send_command();
            break;
    }
    /*
    if (address == EZO_HELP) {
        print_help();
    } else if (address == EZO_EXIT) {
        Serial.println("Exiting manual mode!");
        exit_flag = 1;
    } else if (address) {
        send_command();
    } else {
        print_not_recognized();
    }*/
}

// convert sensor string into address number
ADDRESS SerialHandler::parse_sensor() {
    if (!strcmp(sensor, "do")) {
        return EZO_DO;
    }
    if (!strcmp(sensor, "orp")) {
        return EZO_ORP;
    }
    if (!strcmp(sensor, "ph")) {
        return EZO_PH;
    }
    if (!strcmp(sensor, "ec")) {
        return EZO_EC;
    }
    if (!strcmp(sensor, "rtd")) {
        return EZO_RTD;
    }
    if (!strcmp(sensor, "help")) {
        return EZO_HELP;
    }
    if (!strcmp(sensor, "exit")) {
        return EZO_EXIT;
    }
    // if we made it here, sensor not recognized
    return EZO_NONE;
}

// send the address and command to the I2C bus
uint8_t SerialHandler::send_command() {
    // clear the tx_buffer
    tx_buffer[0] = 0;

    Serial.print("Sending command \"");
    Serial.print(command);
    Serial.print("\" to the ");
    Serial.print(sensor);
    Serial.print(" sensor... \n");

    Wire.beginTransmission(address);
    Wire.write(command);
    Wire.endTransmission();

    // give the ezo time to process
    delay(900);

    Wire.requestFrom(address, BUFFER_SIZE, 1);

    // first byte read is ezo response code
    response_code = Wire.read();
    switch (response_code) {
        case 1:
            Serial.println("Request successful.");
            break;
        case 2:
            Serial.println("Syntax error.");
            Serial.println("Type HELP for syntax help");
            break;
        case 3:
            Serial.println("Still processing, not ready.");
            break;
        case 4:
            Serial.println("No data to send.");
            break;
        default:
            Serial.print("Sensor not available.");
            break;
    }

    // read bytes one at a time, placing them into the tx buffer
    tx_buffer_index = 0;
    while (Wire.available()) {
        tx_byte = Wire.read();
        tx_buffer[tx_buffer_index] = tx_byte;
        tx_buffer_index++;
        if (tx_byte == 0) {
            break;
        }
    }

    // print data from the tx buffer
    Serial.println(tx_buffer);
    Serial.println();
    return 1;
}

// error message when sensor is not recognized
void SerialHandler::print_not_recognized() {
    Serial.print("Sensor ");
    Serial.print(sensor);
    Serial.print(" not recognized \n");
    Serial.print("Type HELP for syntax help \n\n");
}

// help message
void SerialHandler::print_help(void) {
    Serial.println();
    Serial.println("======================");
    Serial.println("TIMU Mayfly Manual Mode");
    Serial.println("Type EXIT to return to regular logging mode!");
    Serial.println("Syntax is [SENSOR] [COMMAND]");
    Serial.println("Available sensors are DO, ORP, PH, COND, and RTD");
    Serial.println("Refer to the EZO datasheets for commands");
    Serial.println("Example: COND R takes a reading from the electrical conductivity sensor");
    Serial.println("======================");
    Serial.println();
}

// when ready to exit, reset exit flag
bool SerialHandler::ready_to_exit() {
    if (exit_flag) {
        exit_flag = 0;
        return 1;
    } else
        return 0;
}