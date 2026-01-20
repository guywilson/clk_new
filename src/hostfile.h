#include <string>

#include "logger.h"
#include "binary.h"
#include "cloak.h"

using namespace std;

#ifndef __INCL_HOSTFILE
#define __INCL_HOSTFILE

class HostFileReader : public BinaryFile {
    private:
        void calculateFileLength();

    protected:
        Logger & log = Logger::getInstance();

        virtual void onOpen() {}
        virtual void onClose() {}

    public:
        virtual ~HostFileReader() {
            close();
        }

        virtual void open(const string & filename) override;
        virtual void close() override {
            onClose();
            BinaryFile::close();
        }

        virtual void readBlock(uint8_t * buffer, size_t bufferLength) = 0;
};

class HostFileWriter : public BinaryFile {
    protected:
        Logger & log = Logger::getInstance();

        virtual void onOpen() {}
        virtual void onClose() {}

    public:
        virtual ~HostFileWriter() {
            close();
        }

        virtual void open(const string & filename) override;
        virtual void close() override {
            onClose();
            BinaryFile::close();
        }

        virtual void writeBlock(uint8_t * buffer, size_t bufferLength) = 0;
};

class HostFile {
    protected:
        Logger & log = Logger::getInstance();

        CloakSecurity security;

        HostFileReader * reader;
        HostFileWriter * writer;

    public:
        void setCloakSecurityLevel(CloakSecurity & securityLevel) {
            security = securityLevel;
        }

        virtual void readBlock(uint8_t * buffer, size_t bufferLength) {
            reader->readBlock(buffer, bufferLength);
        }

        virtual void writeBlock(uint8_t * buffer, size_t bufferLength) {
            writer->writeBlock(buffer, bufferLength);
        }

        virtual void addBlock(
                        uint8_t * sourceBlock, 
                        size_t sourceBlockSize) = 0;
        virtual void extractBlock(
                        uint8_t * targetBlock, 
                        size_t targetBlockSize) = 0;
};

#endif