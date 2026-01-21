#include "cloak.h"
#include "logger.h"
#include "clk_error.h"

void CloakAlgorithm::mergeByte(uint8_t * imageData, size_t imageDataLen, uint8_t dataByte, const CloakSecurity & security) {
	uint8_t	secretBits;
	int	bitCounter = 0;

	uint8_t mask = getBitMask(security);

    for (int i = 0;i < imageDataLen;i++) {
        secretBits = (dataByte >> bitCounter) & mask;
        imageData[i] = (imageData[i] & ~mask) | secretBits;

        bitCounter += (int)security;

        if (bitCounter == 8) {
            bitCounter = 0;
        }
    }
}

uint8_t CloakAlgorithm::extractByte(uint8_t * imageData, size_t imageDataLen, const CloakSecurity & security) {
	uint8_t	secretBits = 0x00;
	int	bitCounter = 0;

	uint8_t mask = getBitMask(security);

	uint8_t	dataByte = 0x00;

    for (int i = 0;i < imageDataLen;i++) {
        secretBits = imageData[i] & mask;
        dataByte += secretBits << bitCounter;

        bitCounter += (int)security;

        if (bitCounter == 8) {
            bitCounter = 0;
        }
    }

	return dataByte;
}

void CloakAlgorithm::mergeBlock(
                        uint8_t * imageData, 
                        uint8_t * dataBlock, 
                        size_t blockLength, 
                        const CloakSecurity & security)
{
    size_t numImageBytes = calculateBlockHostBytesRequired(1, security);

    size_t imgPtr = 0;
    for (size_t i = 0;i < blockLength;i++) {
        mergeByte(&imageData[imgPtr], numImageBytes, dataBlock[i], security);
        imgPtr += numImageBytes;
    }
}

void CloakAlgorithm::extractBlock(                        
                        uint8_t * imageData, 
                        uint8_t * dataBlock, 
                        size_t blockLength, 
                        const CloakSecurity & security)
{
    size_t numImageBytes = calculateBlockHostBytesRequired(1, security);

    size_t imgPtr = 0;
    for (size_t i = 0;i < blockLength;i++) {
        dataBlock[i] = extractByte(&imageData[imgPtr], numImageBytes, security);
        imgPtr += numImageBytes;
    }
}
