#include <string>

#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "binary.h"
#include "cloakable.h"
#include "logger.h"
#include "clk_error.h"

using namespace std;

void CloakableInputFile::calculateFileLength() {
    if (fptr != NULL) {
        long currentPos = ftell(fptr);
        fseek(fptr, 0, SEEK_END);
        fileLength = (size_t)ftell(fptr);
        fseek(fptr, currentPos, SEEK_SET);
    }
}

void CloakableInputFile::open(const string & filename) {
    fptr = fopen(filename.c_str(), "rb");

    if (fptr == NULL) {
        throw clk_error(
                clk_error::buildMsg(
                    "Failed to open input file '%s' : '%s'", 
                    filename.c_str(), 
                    strerror(errno)),
                __FILE__,
                __LINE__);
    }

    calculateFileLength();

    this->fileName = filename;
}

size_t CloakableInputFile::readBlock(uint8_t * buffer, size_t blockSize) {
    size_t bytesRead = fread(buffer, sizeof(uint8_t), blockSize, fptr);

    if (bytesRead < blockSize) {
        if (!feof(fptr)) {
            int error = ferror(fptr);

            if (error) {
                throw clk_error(
                        clk_error::buildMsg(
                            "Failed to read %u bytes from '%s'", 
                            blockSize, 
                            fileName.c_str()),
                        __FILE__,
                        __LINE__);
            }
        }
    }

    return bytesRead;
}


void CloakableOutputFile::open(const string & filename) {
    fptr = fopen(filename.c_str(), "wb");

    if (fptr == NULL) {
        throw clk_error(
                clk_error::buildMsg(
                    "Failed to open output file '%s' : '%s'", 
                    filename.c_str(), 
                    strerror(errno)),
                __FILE__,
                __LINE__);
    }

    this->fileName = filename;
}

size_t CloakableOutputFile::writeBlock(uint8_t * buffer, size_t blockSize) {
    size_t bytesWritten = fwrite(buffer, sizeof(uint8_t), blockSize, fptr);

    if (bytesWritten < blockSize) {
        int error = ferror(fptr);

        if (error) {
            throw clk_error(
                    clk_error::buildMsg(
                        "Failed to write %u bytes to '%s'", 
                        blockSize, 
                        fileName.c_str()),
                    __FILE__,
                    __LINE__);
        }
    }

    return bytesWritten;
}
