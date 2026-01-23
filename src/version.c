#include "version.h"

#define __BDATE__      "2026-01-23 08:34:18"
#define __BVERSION__   "1.0.007"

const char * getVersion(void) {
    return __BVERSION__;
}

const char * getBuildDate(void) {
    return __BDATE__;
}
