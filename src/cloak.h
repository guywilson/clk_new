#include <string>
#include <stdint.h>

using namespace std;

#ifndef __INCL_CLOAK
#define __INCL_CLOAK

enum class CloakSecurity {
    security_high = 1,
    security_medium = 2,
    security_low = 4,
    security_none = 8
};

class CloakAlgorithm {
    private:
        static void mergeByte(uint8_t * imageData, size_t imageDataLen, uint8_t dataByte, CloakSecurity & security);
        static uint8_t extractByte(uint8_t * imageData, size_t imageDataLen, CloakSecurity & security);

    public:
        static size_t calculateBlockHostBytesRequired(size_t blockSize, CloakSecurity & security) {
            return ((blockSize * (size_t)8) / (size_t)security);
        }

        static void mergeBlock(
                        uint8_t * imageData, 
                        size_t imageDataLen, 
                        uint8_t * dataBlock, 
                        size_t blockLength, 
                        CloakSecurity & security);

        static void extractBlock(
                        uint8_t * imageData, 
                        size_t imageDataLen, 
                        uint8_t * dataBlock, 
                        size_t blockLength, 
                        CloakSecurity & security);
};

#endif
