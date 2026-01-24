#include <string>

#include "algorithm.h"

using namespace std;

#ifndef __INCL_KEY
#define __INCL_KEY

pair<uint8_t *, size_t> getKey(AlgorithmType & algorithm, bool generateKey, const string & keyFilename, size_t keyLength);

#endif
