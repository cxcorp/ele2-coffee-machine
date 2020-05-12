#ifndef COFFEE_SENSOR_VERSION_H
#define COFFEE_SENSOR_VERSION_H

// don't override _FW_VERSION if we want to at some point inject it programmatically via build toolchain
#ifndef _FW_VERSION
#define _FW_VERSION "0.8.1"
#endif

#define _FW_TIMESTAMP __DATE__ " " __TIME__

const char *FW_VERSION = _FW_VERSION;
const char *FW_TIMESTAMP = _FW_TIMESTAMP;

#endif
