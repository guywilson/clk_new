#include "version.h"

#define __BDATE__      "2026-01-20 22:34:58"
#define __BVERSION__   "0.1.010"

const char * getVersion(void) {
    return __BVERSION__;
}

const char * getBuildDate(void) {
    return __BDATE__;
}
