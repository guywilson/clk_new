#include "version.h"

#define __BDATE__      "2026-01-22 21:52:47"
#define __BVERSION__   "1.0.003"

const char * getVersion(void) {
    return __BVERSION__;
}

const char * getBuildDate(void) {
    return __BDATE__;
}
