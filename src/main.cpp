#include <iostream>
#include <string>
#include <stdio.h>

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
#include "xdump.h"

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

    PNGReader * reader = new PNGReader();
    reader->open(imageFileName);

    size_t initBufferSize = file->getInitialisationBlockBufferSize();
    uint8_t * initBuffer = file->getInitialisationBlockBuffer();

    file->fillInitialisationBlockBuffer(initBuffer);

    PNGHost host;

    // Add initialisation block to image...
    host.setCloakSecurityLevel(CloakSecurity::security_high);
    host.addBlock(reader, initBuffer, initBufferSize);

    file->setKey(key, keyLength);

    size_t blockSize = file->getBlockSize();
    uint8_t * buffer = file->getAllocatedBlock();

    while (file->hasMoreBlocks()) {
        size_t bytesRead = file->readBlock(buffer);
        host.addBlock(reader, buffer, blockSize);
    }

    PNGWriter writer;
    writer.assignImageDetails(reader->getPNGDetails());

    reader->close();

    writer.open(imageFileName);
    writer.close();

    delete reader;
}

void extractFileFromImage(const string & dataFileName, const string & imageFileName, uint8_t * key, size_t keyLength) {
    auto file = DecryptableFileFactory::create(dataFileName, AlgorithmType::aes_encryption);

    PNGReader * reader = new PNGReader();
    reader->open(imageFileName);

    size_t initBufferSize = file->getInitialisationBlockBufferSize();
    uint8_t * initBuffer = file->getInitialisationBlockBuffer();

    PNGHost host;
    host.setCloakSecurityLevel(CloakSecurity::security_high);
    host.extractBlock(reader, initBuffer, initBufferSize);

    LengthBlock initBlock = file->extractInitialisationBlockFromBuffer(initBuffer);

    file->setKey(key, keyLength);

    uint8_t * buffer = file->getAllocatedBlock();
    size_t blockSize = file->getBlockSize();

    while (file->getBytesLeftToWrite() > 0) {
        host.extractBlock(reader, buffer, blockSize);
        file->writeBlock(buffer);
    }

    delete reader;

    file->close();
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

void test_merge_extract() {
    uint8_t imgBytes[32] = {
        0xEC, 0xA7, 0x50, 0xE9, 0xA3, 0x4E, 0xE4, 0x9F, 
        0x4F, 0xE4, 0x9C, 0x4D, 0xE4, 0x9C, 0x4D, 0xE5,
        0x9C, 0x4F, 0xE3, 0x9A, 0x4B, 0xE3, 0x99, 0x49, 
        0xE3, 0x99, 0x48, 0xE4, 0x9A, 0x4B, 0xDE, 0x94};

    uint8_t dataBytes[4] = {0x37, 0x65, 0x42, 0x17};

    /*
    ** 00000000	ECA7 50E9 A34E E49F  |..P..N..|
    ** 00000000	EDA7 51E8 A34F E49E  |..Q..O..|
    **
    **           1 1  1 0  1 1  0 0
    */
    hexDump(imgBytes, 8);

    CloakAlgorithm::mergeBlock(imgBytes, dataBytes, 4, CloakSecurity::security_high);

    hexDump(imgBytes, 8);

    CloakAlgorithm::extractBlock(imgBytes, dataBytes, 4, CloakSecurity::security_high);

    hexDump(dataBytes, 4);
}

int main(int argc, char ** argv) {
    int defaultLogLevel = LOG_LEVEL_ALL;

    Logger & log = Logger::getInstance();
    log.init("clk.log", defaultLogLevel);

    addFileToImage("/Users/guy/test.dat", "/Users/guy/flowers.png", testKey, 16);
    extractFileFromImage("/Users/guy/out.dat", "/Users/guy/flowers.png", testKey, 16);

    // read_write_image("/Users/guy/flowers.png", "/Users/guy/out.png");

    // test_merge_extract();

    log.close();

    return 0;
}
