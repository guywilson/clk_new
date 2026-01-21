#include "cloak.h"
#include "logger.h"
#include "clk_error.h"

void CloakAlgorithm::mergeByte(uint8_t * imageData, size_t imageDataLen, uint8_t dataByte, CloakSecurity & security) {
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

uint8_t CloakAlgorithm::extractByte(uint8_t * imageData, size_t imageDataLen, CloakSecurity & security) {
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
                        size_t imageDataLen, 
                        uint8_t * dataBlock, 
                        size_t blockLength, 
                        CloakSecurity & security)
{
    size_t numImageBytes = calculateBlockHostBytesRequired(1, security);

    for (size_t i = 0;i < blockLength;i++) {
        mergeByte(imageData, numImageBytes, dataBlock[i], security);
    }
}

void CloakAlgorithm::extractBlock(                        
                        uint8_t * imageData, 
                        size_t imageDataLen, 
                        uint8_t * dataBlock, 
                        size_t blockLength, 
                        CloakSecurity & security)
{
    size_t numImageBytes = calculateBlockHostBytesRequired(1, security);

    for (size_t i = 0;i < blockLength;i++) {
        dataBlock[i] = extractByte(imageData, numImageBytes, security);
    }
}
