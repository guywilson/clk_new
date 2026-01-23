#include "version.h"

#define __BDATE__      "2026-01-23 08:45:51"
#define __BVERSION__   "1.0.008"

const char * getVersion(void) {
    return __BVERSION__;
}

const char * getBuildDate(void) {
    return __BDATE__;
}
