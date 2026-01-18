#include "version.h"

#define __BDATE__      "2026-01-18 09:40:41"
#define __BVERSION__   "0.1.007"

const char * getVersion(void) {
    return __BVERSION__;
}

const char * getBuildDate(void) {
    return __BDATE__;
}
