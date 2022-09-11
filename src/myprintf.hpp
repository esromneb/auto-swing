#pragma once

#define printf(...) snprintf(buf, 64, __VA_ARGS__); Serial.print(buf);
