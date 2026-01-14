#include <string>
#include <string.h>
#include <stdlib.h>

#include <gcrypt.h>

#include "logger.h"
#include "cloakable.h"
#include "decryptable.h"
#include "clk_error.h"

using namespace std;

void AESDecryptableFile::decryptBlock(uint8_t * buffer, size_t bufferLength) {

}

size_t AESDecryptableFile::getBlockSize() {
    size_t blockSize = 0;
    return blockSize;
}

void XORDecryptableFile::decryptBlock(uint8_t * buffer, size_t bufferLength) {
    size_t blockSize = getBlockSize();
    keyPointer += blockSize;

    if (keyPointer > keyLength) {
        throw clk_error("Key overrun error", __FILE__, __LINE__);
    }
}

void XORDecryptableFile::setKey(uint8_t * key, size_t keyLength) {
    if (keyLength < size()) {
        throw clk_error(
            clk_error::buildMsg(
                "The key must be as least as long as the file (%u bytes) to be encrypted for XOR encryption",
                size()), 
            __FILE__, 
            __LINE__);
    }

    DecryptableFile::setKey(key, keyLength);
}
