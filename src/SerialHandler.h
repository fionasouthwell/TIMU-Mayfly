// SerialHandler.h
// @bruthaearl (jake jones)
// Send manual I2C commands to the EZOs via serial

#include <Arduino.h>
#include <Wire.h>

#define BUFFER_SIZE 32

typedef enum ADDRESS{
    EZO_NONE = 0,
    EZO_HELP = 1,
    EZO_EXIT = 2,
    EZO_DO = 97,
    EZO_ORP = 98,
    EZO_PH = 99,
    EZO_COND = 100,
    EZO_RTD = 102
} ADDRESS;

// serial handler for the mayfly
class SerialHandler {
   public:
    SerialHandler() {}

    void process_buffer(char (&rx_buffer)[BUFFER_SIZE]);
    bool ready_to_exit();
    void print_help(void);

   private:
    uint8_t response_code = 0; // response code sent back from EZO
    uint8_t tx_buffer_index = 0;
    bool exit_flag = 0; // set when ready to exit
    char tx_buffer[BUFFER_SIZE]; // to be sent back over serial to pc
    char tx_byte; 
    char *sensor;  // sensor string
    char *command; // command string
    ADDRESS address; // sensor address number
    uint8_t send_command();
    ADDRESS parse_sensor();
    void print_not_recognized();
};