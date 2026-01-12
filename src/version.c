#include "version.h"

#define __BDATE__      "2026-01-12 21:38:28"
#define __BVERSION__   "0.1.001"

const char * getVersion(void) {
    return __BVERSION__;
}

const char * getBuildDate(void) {
    return __BDATE__;
}
