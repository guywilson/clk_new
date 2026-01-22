#include "version.h"

#define __BDATE__      "2026-01-22 18:25:19"
#define __BVERSION__   "1.0.002"

const char * getVersion(void) {
    return __BVERSION__;
}

const char * getBuildDate(void) {
    return __BDATE__;
}
