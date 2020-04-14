#include "Util.h"

namespace Util {

const char *wlStatusString(uint8_t status) {
  switch (status) {
    case 255: return "WL_NO_SHIELD";
    case 0: return "WL_IDLE_STATUS";
    case 1: return "WL_NO_SSID_AVAIL";
    case 2: return "WL_SCAN_COMPLETED";
    case 3: return "WL_CONNECTED";
    case 4: return "WL_CONNECT_FAILED";
    case 5: return "WL_CONNECTION_LOST";
    case 6: return "WL_DISCONNECTED";
    default: return "UNKNOWN";
  }
}

}
