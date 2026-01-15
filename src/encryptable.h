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

class EncryptableFile : public CloakableInputFile {
    protected:
        uint8_t * key;
        size_t keyLength;

        EncryptionAlgorithm * algorithm;

        virtual void encryptBlock(uint8_t * buffer, size_t bufferLength) = 0;
    
    public:
        virtual size_t getBlockSize() {
            return algorithm->getBlockSize();
        }

        size_t readBlock(uint8_t * buffer, size_t blockSize) override {
            size_t bytesRead = CloakableInputFile::readBlock(buffer, blockSize);
            encryptBlock(buffer, bytesRead);

            return bytesRead;
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

class AESEncryptableFile : public EncryptableFile {
    private:
        size_t getEncryptedFileSizeDifference() {
            size_t blockSize = algorithm->getBlockSize();
            return (blockSize - (size() % blockSize)) + blockSize;
        }

    protected:
        virtual void encryptBlock(uint8_t * buffer, size_t bufferLength) override;

    public:
        AESEncryptableFile() {
            algorithm = new AESEncryptionAlgorithm();
        }

        void fillInitialisationBlockBuffer(uint8_t * buffer) override  {
            LengthBlock block = {(cloaked_len_t)size(), (uint8_t)getEncryptedFileSizeDifference()};

            memcpy(buffer, &block, CLOAKED_LENGTH_BLOCK_SIZE);

            uint8_t * iv = algorithm->getIV();

            memcpy(&buffer[CLOAKED_LENGTH_BLOCK_SIZE], iv, getBlockSize());
        }
};

class XOREncryptableFile : public EncryptableFile {
    private:
        size_t keyPointer;

    protected:
        virtual void encryptBlock(uint8_t * buffer, size_t bufferLength) override;

    public:
        XOREncryptableFile() {
            algorithm = new XOREncryptionAlgorithm();
            keyPointer = 0;
        }

        virtual void setKey(uint8_t * key, size_t keyLength) override;
};

#endif
