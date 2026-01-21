#include "version.h"

#define __BDATE__      "2026-01-21 09:36:01"
#define __BVERSION__   "0.1.011"

const char * getVersion(void) {
    return __BVERSION__;
}

const char * getBuildDate(void) {
    return __BDATE__;
}
