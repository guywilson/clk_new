#include "version.h"

#define __BDATE__      "2026-01-21 22:21:33"
#define __BVERSION__   "0.1.013"

const char * getVersion(void) {
    return __BVERSION__;
}

const char * getBuildDate(void) {
    return __BDATE__;
}
