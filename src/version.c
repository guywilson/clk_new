#include "version.h"

#define __BDATE__      "2026-01-23 13:16:22"
#define __BVERSION__   "1.0.011"

const char * getVersion(void) {
    return __BVERSION__;
}

const char * getBuildDate(void) {
    return __BDATE__;
}
