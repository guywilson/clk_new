#include "hostfile.h"

#ifndef __INCL_IMAGE
#define __INCL_IMAGE

class Image : public HostFile {
    protected:
        size_t width;
        size_t height;
};

#endif
