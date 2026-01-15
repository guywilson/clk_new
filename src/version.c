#include "version.h"

#define __BDATE__      "2026-01-15 22:10:36"
#define __BVERSION__   "0.1.004"

const char * getVersion(void) {
    return __BVERSION__;
}

const char * getBuildDate(void) {
    return __BDATE__;
}
