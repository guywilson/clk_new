#include <string>
#include <string.h>
#include <stdlib.h>

#include "logger.h"
#include "cloakable.h"
#include "algorithm.h"
#include "clk_error.h"

using namespace std;

#ifndef __INCL_DECRYPTABLE
#define __INCL_DECRYPTABLE

class DecryptableFile : public CloakableOutputFile {
    protected:
        uint8_t * key;
        size_t keyLength;

        EncryptionAlgorithm * algorithm;

        virtual void decryptBlock(uint8_t * buffer, size_t bufferLength) = 0;
    
    public:
        size_t getBlockSize() override {
            return algorithm->getBlockSize();
        }

        size_t getInitialisationBlockBufferSize() override {
            return CloakableFile::getInitialisationBlockBufferSize() + getBlockSize();
        }

        inline size_t getBytesLeftToWrite() {
            return bytesLeftToWrite;
        }

        inline size_t getBytesToWrite() {
            return (getBytesLeftToWrite() >= getBlockSize() ? getBlockSize() : getBytesLeftToWrite());
        }

        size_t writeBlock(uint8_t * buffer, size_t bytesToWrite) override {
            size_t blockSize = getBlockSize();

            decryptBlock(buffer, blockSize);
            size_t bytesWritten = CloakableOutputFile::writeBlock(buffer, bytesToWrite);

            bytesLeftToWrite -= bytesWritten;

            return bytesWritten;
        }

        size_t writeBlock(uint8_t * buffer) {
            return writeBlock(buffer, getBytesToWrite());
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

        void extractAdditionalInitialisationBlock(uint8_t * initialisationBlockBuffer) override {
            size_t blockSize = algorithm->getBlockSize();

            uint8_t * iv = (uint8_t *)malloc(blockSize);
            memcpy(iv, &initialisationBlockBuffer[sizeof(LengthBlock)], blockSize);

            algorithm->setIV(iv, blockSize);
        }

    public:
        AESDecryptableFile() {
            algorithm = new AESEncryptionAlgorithm();
        }

        LengthBlock extractInitialisationBlockFromBuffer(uint8_t * initialisationBlockBuffer) override {
            LengthBlock block = CloakableOutputFile::extractInitialisationBlockFromBuffer(initialisationBlockBuffer);

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
