#ifndef COFFEE_SENSOR_UTIL_H
#define COFFEE_SENSOR_UTIL_H

#include <stdint.h>

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

namespace Util {

const char *wlStatusString(uint8_t status);

}

#endif
