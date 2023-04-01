#include <Arduino.h>
#include <Wire.h>


void print_help(void);
uint8_t parse_sensor(char *sensor);
uint8_t send_command(char *sensor, uint8_t address, char *command);
void print_not_recognized(char *sensor);

uint8_t handle_serial(char (&rx_buffer)[32]);