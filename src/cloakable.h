#include <string>

#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "binary.h"
#include "algorithm.h"
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

#define CLOAKED_LENGTH_BLOCK_SIZE               sizeof(LengthBlock)

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
        virtual size_t getInitialisationBlockBufferSize() {
            return CLOAKED_LENGTH_BLOCK_SIZE;
        }

        virtual uint8_t * getInitialisationBlockBuffer() {
            size_t initialisationBlockSize = getInitialisationBlockBufferSize();
            uint8_t * initialisationBlockBuffer = (uint8_t *)malloc(initialisationBlockSize);

            if (initialisationBlockBuffer == NULL) {
                throw clk_error(
                        clk_error::buildMsg(
                            "Failed to allocate %u bytes for the initialisation block", 
                            initialisationBlockSize), 
                        __FILE__, 
                        __LINE__);
            }

            return initialisationBlockBuffer;
        }

        virtual inline size_t getBlockSize() {
            return DEFAULT_BLOCK_SIZE;
        }

        virtual uint8_t * getAllocatedBlock() {
            size_t blockSize = getBlockSize();

            uint8_t * block = (uint8_t *)malloc(blockSize);

            if (block == NULL) {
                throw clk_error(
                    clk_error::buildMsg(
                        "Failed to allocate %u bytes for block",
                        blockSize), 
                    __FILE__, 
                    __LINE__);
            }

            return block;
        }
};

class CloakableInputFile : public CloakableFile {
    private:
        void calculateFileLength();

    protected:
        virtual void addAdditionalInitialisationBlock(uint8_t * initialisationBlockBuffer) {}

    public:
        void open(const string & filename) override;

        virtual size_t readBlock(uint8_t * buffer);

        virtual inline bool hasMoreBlocks() {
            return ((getBlockSize() * getBlockNum()) < size() ? true : false);            
        }

        virtual void fillInitialisationBlockBuffer(uint8_t * initialisationBlockBuffer) {
            LengthBlock block = {(cloaked_len_t)size(), 0};

            memcpy(initialisationBlockBuffer, &block, getInitialisationBlockBufferSize());

            addAdditionalInitialisationBlock(initialisationBlockBuffer);
        }
};

class CloakableOutputFile : public CloakableFile {
    protected:
        size_t actualFileLength;
        size_t encryptedFileLength;
        size_t bytesLeftToWrite;

        virtual void extractAdditionalInitialisationBlock(uint8_t * initialisationBlockBuffer) {}

    public:
        void open(const string & filename) override;

        virtual size_t writeBlock(uint8_t * buffer, size_t bytesToWrite);

        virtual LengthBlock extractInitialisationBlockFromBuffer(uint8_t * initialisationBlockBuffer) {
            LengthBlock block;
            memcpy(&block, initialisationBlockBuffer, CLOAKED_LENGTH_BLOCK_SIZE);

            actualFileLength = block.originalFileLength;
            encryptedFileLength = block.originalFileLength + block.encryptedLengthIncrease;
            bytesLeftToWrite = actualFileLength;

            extractAdditionalInitialisationBlock(initialisationBlockBuffer);

            return block;
        }
};

#endif
