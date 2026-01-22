#include "version.h"

#define __BDATE__      "2026-01-22 17:02:51"
#define __BVERSION__   "0.1.017"

const char * getVersion(void) {
    return __BVERSION__;
}

const char * getBuildDate(void) {
    return __BDATE__;
}
