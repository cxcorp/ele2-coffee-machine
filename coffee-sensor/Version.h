#ifndef COFFEE_SENSOR_VERSION_H
#define COFFEE_SENSOR_VERSION_H

// don't override FW_VERSION if we want to at some point inject it programmatically via build toolchain
#ifndef FW_VERSION
#define FW_VERSION "0.8.0"
#endif

#define FW_TIMESTAMP __DATE__ " " __TIME__

#endif
