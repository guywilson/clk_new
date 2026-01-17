#include "version.h"

#define __BDATE__      "2026-01-17 21:26:47"
#define __BVERSION__   "0.1.006"

const char * getVersion(void) {
    return __BVERSION__;
}

const char * getBuildDate(void) {
    return __BDATE__;
}
