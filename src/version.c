#include "version.h"

#define __BDATE__      "2026-01-14 21:30:31"
#define __BVERSION__   "0.1.002"

const char * getVersion(void) {
    return __BVERSION__;
}

const char * getBuildDate(void) {
    return __BDATE__;
}
