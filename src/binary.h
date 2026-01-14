#include <string>

#include <stdio.h>

using namespace std;

#ifndef __INCL_BINARY_FILE
#define __INCL_BINARY_FILE

class BinaryFile {
    protected:
        uint8_t * data;
        FILE * fptr;

        string fileName;
        size_t fileLength;

    public:
        virtual void open(const string & filename) = 0;
        virtual void close() {
            if (fptr != NULL) {
                fclose(fptr);
                fptr = NULL;
            }
        }

        size_t size() {
            return fileLength;
        }
};

#endif
