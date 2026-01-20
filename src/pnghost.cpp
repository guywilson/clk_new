#include <string>
#include <string.h>
#include <stdlib.h>

#include <png.h>

#include "binary.h"
#include "clk_error.h"
#include "pngrw.h"
#include "pnghost.h"
#include "cloak.h"

using namespace std;

void PNGHost::addBlock(uint8_t * sourceBlock, size_t sourceBlockSize) {
    size_t numImageBytesRequired = CloakAlgorithm::calculateBlockHostBytesRequired(sourceBlockSize, security);

    PNGReader * pngReader = (PNGReader *)reader;

    uint8_t * imageBuffer = pngReader->getBlockPointer(numImageBytesRequired);

    CloakAlgorithm::mergeBlock(imageBuffer, numImageBytesRequired, sourceBlock, sourceBlockSize, security);
}

void PNGHost::extractBlock(uint8_t * targetBlock, size_t targetBlockSize) {
    size_t numImageBytesRequired = CloakAlgorithm::calculateBlockHostBytesRequired(targetBlockSize, security);

    PNGReader * pngReader = (PNGReader *)reader;

    uint8_t * imageBuffer = pngReader->getBlockPointer(numImageBytesRequired);

    CloakAlgorithm::extractBlock(imageBuffer, numImageBytesRequired, targetBlock, targetBlockSize, security);
}
