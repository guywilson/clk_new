#include <string>

#include <png.h>

#include "image.h"

using namespace std;

#ifndef __INCL_PNG_HOST
#define __INCL_PNG_HOST

class PNGHost : public Image {
    public:
        void addBlock(                        
                uint8_t * sourceBlock, 
                size_t sourceBlockSize) override;

        void extractBlock(
                        uint8_t * targetBlock, 
                        size_t targetBlockSize) override;
};

#endif
