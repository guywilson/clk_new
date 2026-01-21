#include <iostream>
#include <string>

#include "filefactory.h"
#include "cloakable.h"
#include "encryptable.h"
#include "decryptable.h"
#include "pnghost.h"
#include "pngrw.h"
#include "cloak.h"
#include "logger.h"
#include "cmdarg.h"
#include "version.h"

using namespace std;

uint8_t testKey[64] = {
    0xCC, 0x86, 0xB1, 0xC1, 0xEE, 0x9D, 0x66, 0xB8, 
	0x82, 0xF3, 0x49, 0xCF, 0x4E, 0x70, 0x43, 0xEA, 
    0xF2, 0xDA, 0xDF, 0xA5, 0x61, 0x44, 0x7D, 0x23, 
	0x76, 0xA0, 0x8C, 0xA3, 0x80, 0x21, 0x4F, 0xD6, 
    0x55, 0xF5, 0x40, 0x0E, 0x5D, 0x3F, 0xC5, 0x0C, 
	0xC5, 0x7B, 0x7F, 0xA0, 0x41, 0x0B, 0x2C, 0x33, 
    0x55, 0x47, 0xD0, 0x04, 0x72, 0x08, 0xF4, 0xCC,
	0xFA, 0xEB, 0xE2, 0x86, 0xBD, 0xB1, 0xB2, 0x2B 
};

void addFileToImage(const string & dataFileName, const string & imageFileName, uint8_t * key, size_t keyLength) {
    auto file = EncryptableFileFactory::create(dataFileName, AlgorithmType::aes_encryption);

    auto of = CloakableFileFactory::createOutputFile("/Users/guy/test.enc");

    size_t initBufferSize = file->getInitialisationBlockBufferSize();
    uint8_t * initBuffer = file->getInitialisationBlockBuffer();

    file->fillInitialisationBlockBuffer(initBuffer);

    // Add initialisation block to image...
    // image.addBlock(initBuffer, initBufferSize);

    of->writeBlock(initBuffer, initBufferSize);

    file->setKey(key, keyLength);

    size_t blockSize = file->getBlockSize();
    uint8_t * buffer = file->getAllocatedBlock();

    while (file->hasMoreBlocks()) {
        size_t bytesRead = file->readBlock(buffer);
        // image.addBlock(buffer, bytesRead);

        of->writeBlock(buffer, blockSize);
    }
}

void extractFileFromImage(const string & dataFileName, const string & imageFileName, uint8_t * key, size_t keyLength) {
    auto file = DecryptableFileFactory::create(dataFileName, AlgorithmType::aes_encryption);

    size_t initBufferSize = file->getInitialisationBlockBufferSize();
    uint8_t * initBuffer = file->getInitialisationBlockBuffer();

    auto inputFile = CloakableFileFactory::createInputFile("/Users/guy/test.enc");
    inputFile->readBlock(initBuffer, initBufferSize);
    inputFile->resetBlockCounter();

    //image.extractBlock(initBuffer, initBufferSize);

    LengthBlock initBlock = file->extractInitialisationBlockFromBuffer(initBuffer);

    file->setKey(key, keyLength);

    uint8_t * buffer = file->getAllocatedBlock();
    size_t blockSize = file->getBlockSize();

    while (file->getBytesLeftToWrite() > 0) {
        inputFile->readBlock(buffer, blockSize);

        //image.extractBlock(buffer, blockSize);
        file->writeBlock(buffer);
    }
}

void read_write_image(const string & inputFile, const string & outputFile) {
    PNGReader reader;
    PNGWriter writer;

    reader.open(inputFile);

    writer.assignImageDetails(reader.getPNGDetails());
    writer.open(outputFile);

    writer.close();
    reader.close();
}

int main(int argc, char ** argv) {
    int defaultLogLevel = LOG_LEVEL_ALL;

    Logger & log = Logger::getInstance();
    log.init("clk.log", defaultLogLevel);

    // addFileToImage("/Users/guy/test.dat", "test.png", testKey, 16);
    // extractFileFromImage("/Users/guy/out.dat", "test.png", testKey, 16);

    read_write_image("/Users/guy/flowers.png", "/Users/guy/out.png");

    log.close();

    return 0;
}
