#include <string>
#include <string.h>
#include <stdlib.h>

#include "logger.h"
#include "cloakable.h"
#include "algorithm.h"
#include "clk_error.h"

using namespace std;

#ifndef __INCL_ENCRYPTABLE
#define __INCL_ENCRYPTABLE

class DecryptableFile : public CloakableOutputFile {
    protected:
        uint8_t * key;
        size_t keyLength;

        EncryptionAlgorithm * algorithm;

        virtual void decryptBlock(uint8_t * buffer, size_t bufferLength) = 0;
    
    public:
        virtual size_t getBlockSize() {
            return algorithm->getBlockSize();
        }

        size_t writeBlock(uint8_t * buffer, size_t blockSize) override {
            decryptBlock(buffer, blockSize);
            size_t bytesWritten = CloakableOutputFile::writeBlock(buffer, blockSize);

            return bytesWritten;
        }

        virtual void setKey(uint8_t * key, size_t keyLength) {
            this->keyLength = keyLength;
            this->key = (uint8_t *)malloc(keyLength);

            if (this->key == NULL) {
                throw clk_error(
                    clk_error::buildMsg(
                        "Failed to allocate %u bytes for key",
                        keyLength), 
                    __FILE__, 
                    __LINE__);
            }

            memcpy(this->key, key, keyLength);
        }
};

class AESDecryptableFile : public DecryptableFile {
    protected:
        virtual void decryptBlock(uint8_t * buffer, size_t bufferLength) override;

    public:
        AESDecryptableFile() {
            algorithm = new AESEncryptionAlgorithm();
        }

        LengthBlock extractInitialisationBlockFromBuffer(uint8_t * buffer) override {
            LengthBlock block = CloakableOutputFile::extractInitialisationBlockFromBuffer(buffer);

            size_t blockSize = algorithm->getBlockSize();

            uint8_t * iv = (uint8_t *)malloc(blockSize);
            memcpy(iv, &buffer[sizeof(LengthBlock)], blockSize);

            algorithm->setIV(iv, blockSize);

            return block;
        }
};

class XORDecryptableFile : public DecryptableFile {
    private:
        size_t keyPointer;

    protected:
        virtual void decryptBlock(uint8_t * buffer, size_t bufferLength) override;

    public:
        XORDecryptableFile() {
            algorithm = new XOREncryptionAlgorithm();
            keyPointer = 0;
        }

        virtual void setKey(uint8_t * key, size_t keyLength) override;
};

#endif
