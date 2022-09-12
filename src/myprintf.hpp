#pragma once

#include <Arduino.h>

#define printf(...) snprintf(my_printf_buffer, 80, __VA_ARGS__); Serial.print(my_printf_buffer);

extern char my_printf_buffer[80];
