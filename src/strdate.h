#include <iostream>
#include <string>
#include <time.h>
#include <sys/time.h>
#include <cstdint>

#include "logger.h"

using namespace std;

#ifndef __INCL_STRDATE
#define __INCL_STRDATE

#define TIME_STAMP_BUFFER_LEN               64
#define DATE_STAMP_BUFFER_LEN               32
#define DATE_STRING_LENGTH                  10

struct TimeComponents {
    string year;
    string month;
    string day;

    string hour;
    string minute;
    string second;

    string microsecond;
};

class StrDate {
    public:
        static string getTimestamp();
        static string getTimestamp(bool includeus);
        static string getTimestampToMicrosecond();
};

#endif
