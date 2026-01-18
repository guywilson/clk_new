#include "version.h"

#define __BDATE__      "2026-01-18 11:37:48"
#define __BVERSION__   "0.1.008"

const char * getVersion(void) {
    return __BVERSION__;
}

const char * getBuildDate(void) {
    return __BDATE__;
}
