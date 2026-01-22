#include "version.h"

#define __BDATE__      "2026-01-22 15:47:51"
#define __BVERSION__   "0.1.015"

const char * getVersion(void) {
    return __BVERSION__;
}

const char * getBuildDate(void) {
    return __BDATE__;
}
