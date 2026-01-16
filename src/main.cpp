#include <iostream>
#include <string>

#include "cloakable.h"
#include "encryptable.h"
#include "decryptable.h"
#include "cmdarg.h"
#include "version.h"

using namespace std;

void addFileToImage(const string & dataFileName, const string & imageFileName, uint8_t * key, size_t keyLength) {
    AESEncryptableFile file;
    size_t initBufferSize = file.getInitialisationBlockBufferSize();
    uint8_t * initBuffer = file.getInitialisationBlockBuffer();

    file.fillInitialisationBlockBuffer(initBuffer);

    // Add initialisation block to image...
    // image.addBlock(initBuffer, initBufferSize);

    file.open(dataFileName);
    file.setKey(key, keyLength);

    uint8_t * buffer = file.getAllocatedBlock();

    while (file.hasMoreBlocks()) {
        size_t bytesRead = file.readBlock(buffer);
        // image.addBlock(buffer, bytesRead);
    }

    file.close();
}

void extractFileFromImage(const string & dataFileName, const string & imageFileName, uint8_t * key, size_t keyLength) {
    AESDecryptableFile file;
    size_t initBufferSize = file.getInitialisationBlockBufferSize();
    uint8_t * initBuffer = file.getInitialisationBlockBuffer();

    //image.extractBlock(initBuffer, initBufferSize);

    LengthBlock initBlock = file.extractInitialisationBlockFromBuffer(initBuffer);

    file.open(dataFileName);
    file.setKey(key, keyLength);

    uint8_t * buffer = file.getAllocatedBlock();
    size_t blockSize = file.getBlockSize();

    while (file.getBytesLeftToWrite() > 0) {
        //image.extractBlock(buffer, blockSize);
        file.writeBlock(buffer);
    }

    file.close();
}

int main(int argc, char ** argv) {
    return 0;
}
