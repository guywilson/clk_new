#include <string>
#include <string.h>
#include <stdlib.h>

#include "logger.h"
#include "cloakable.h"
#include "clk_error.h"

using namespace std;

#ifndef __INCL_ENCRYPTABLE
#define __INCL_ENCRYPTABLE

#define DEFAULT_BLOCK_SIZE                  64U

class EncryptableFile : public CloakableInputFile {
    protected:
        uint8_t * key;
        size_t keyLength;

        virtual void encryptBlock(uint8_t * buffer, size_t bufferLength) = 0;
    
    public:
        virtual size_t getBlockSize() = 0;

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
    protected:
        virtual void encryptBlock(uint8_t * buffer, size_t bufferLength) override;

    public:
        size_t getBlockSize() override;
};

class XOREncryptableFile : public EncryptableFile {
    private:
        size_t keyPointer;

    protected:
        virtual void encryptBlock(uint8_t * buffer, size_t bufferLength) override;

    public:
        size_t getBlockSize() override {
            return (size_t)DEFAULT_BLOCK_SIZE;
        }

        virtual void setKey(uint8_t * key, size_t keyLength) override;
};

#endif
