#include <string>

#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "binary.h"
#include "logger.h"
#include "clk_error.h"

using namespace std;

#ifndef __INCL_CLOAKABLE_FILE
#define __INCL_CLOAKABLE_FILE

class CloakableInputFile : public BinaryFile {
    private:
        size_t fileLength;

        void getFileLength();

    public:
        void open(const string & filename) override;

        virtual size_t readBlock(uint8_t * buffer, size_t blockSize);

        size_t size() {
            return fileLength;
        }
};

class CloakableOutputFile : public BinaryFile {
    public:
        void open(const string & filename) override;

        virtual size_t writeBlock(uint8_t * buffer, size_t blockSize);
};

#endif
