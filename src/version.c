#include "version.h"

#define __BDATE__      "2026-01-24 11:05:54"
#define __BVERSION__   "1.0.014"

const char * getVersion(void) {
    return __BVERSION__;
}

const char * getBuildDate(void) {
    return __BDATE__;
}
