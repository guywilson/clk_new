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

typedef uint32_t cloaked_len_t;

#pragma pack(push, 1)
struct LengthBlock {
    /*
    ** The original length of the file to be extracted
    */
    cloaked_len_t originalFileLength;

    /*
    ** The number of bytes bigger the encrypted length vs original,
    ** if originalFileLength is an exact multiple of the blocksize, 
    ** this will be 0 (zero)...
    */
    uint8_t encryptedLengthIncrease;
};
#pragma pack(pop)

#define CLOAKED_LENGTH_FIELD_SIZE               sizeof(LengthBlock)

class CloakableFile : public BinaryFile {
    protected:
        int blockNum = 0;

        int getBlockNum() {
            return blockNum;
        }

        bool isFirstBlock() {
            return blockNum == 0 ? true : false;
        }

    public:
        size_t getLengthBufferSize() {
            return CLOAKED_LENGTH_FIELD_SIZE;
        }
};

class CloakableInputFile : public CloakableFile {
    private:
        void calculateFileLength();

    public:
        void open(const string & filename) override;

        virtual size_t readBlock(uint8_t * buffer, size_t blockSize);

        void fillFileLengthBuffer(uint8_t * buffer, LengthBlock & length) {
            memcpy(buffer, &length, CLOAKED_LENGTH_FIELD_SIZE);
        }
};

class CloakableOutputFile : public CloakableFile {
    public:
        void open(const string & filename) override;

        virtual size_t writeBlock(uint8_t * buffer, size_t blockSize);

        LengthBlock extractFileLengthFromBuffer(uint8_t * buffer) {
            LengthBlock length;

            memcpy(&length, buffer, CLOAKED_LENGTH_FIELD_SIZE);

            return length;
        }
};

#endif
