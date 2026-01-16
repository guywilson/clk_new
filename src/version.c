#include "version.h"

#define __BDATE__      "2026-01-16 18:09:34"
#define __BVERSION__   "0.1.005"

const char * getVersion(void) {
    return __BVERSION__;
}

const char * getBuildDate(void) {
    return __BDATE__;
}
