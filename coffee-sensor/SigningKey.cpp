#include "SigningKey.h"

namespace SigningKey {

static const char _pubkey[] PROGMEM = R"EOF(
-----BEGIN PUBLIC KEY-----
MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAt8Wv0awmrQg4KP2k7RP3
cBdeBalJ0iKw8ISIk+06a8E1jYTkpQBMIfKrIHr5PA8I9nawTKTO7p2FoN5inDGx
WVl2QTDoH4oWQqbJpEOJAcPQI94V9/cdJR4fsWKZrRUkSp44v/oxD3pfxSGGftqy
vgm4xUEDpGOfgmYVnnzK+/25NrWn5W9omlrqZJN+IUtM3U3UffdVbftj8yDjwJ8K
hI9PjL6Aqqwp/7M6BfGg8J9otzDAfH/kK6n21/97unqnS6AUnlQcgbDiLkUpMTOY
FAAt5DjJXgaPJOAreUcGo7XDEDl5ckSY60/czodBlMyNlaH7p2m2vLjCq1Qgk43k
QQIDAQAB
-----END PUBLIC KEY-----
)EOF";

PGM_P pubkey = _pubkey;

}
