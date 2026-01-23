#include <iostream>
#include <string>
#include <stdio.h>

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#endif

#include "filefactory.h"
#include "cloakable.h"
#include "encryptable.h"
#include "decryptable.h"
#include "pnghost.h"
#include "pngrw.h"
#include "cloak.h"
#include "logger.h"
#include "cmdarg.h"
#include "version.h"
#include "xdump.h"

#define OPERATION_MERGE                 "merge"
#define OPERATION_EXTRACT               "extract"
#define MAX_PASSWORD_LENGTH             256

using namespace std;

int __getch(void) {
	int		ch;

#ifndef _WIN32
	struct termios current;
	struct termios original;

	tcgetattr(fileno(stdin), &original); /* grab old terminal i/o settings */
	current = original; /* make new settings same as old settings */
	current.c_lflag &= ~ICANON; /* disable buffered i/o */
	current.c_lflag &= ~ECHO; /* set echo mode */
	tcsetattr(fileno(stdin), TCSANOW, &current); /* use these new terminal i/o settings now */
#endif

#ifdef _WIN32
    ch = _getch();
#else
    ch = getchar();
#endif

#ifndef _WIN32
	tcsetattr(0, TCSANOW, &original);
#endif

    return ch;
}

CloakSecurity getSecurityLevelArg(const string & arg) {
    CloakSecurity security;

    if (arg == "high" || arg == "hi") {
        security = CloakSecurity::security_high;
    }
    else if (arg == "medium" || arg == "med") {
        security = CloakSecurity::security_medium;
    }
    else if (arg == "low" || arg == "lo") {
        security = CloakSecurity::security_low;
    }
    else if (arg == "none" || arg == "no") {
        security = CloakSecurity::security_none;
    }
    else if (arg.length() == 0) {
        security = CloakSecurity::security_high;
    }
    else {
        throw clk_error(clk_error::buildMsg("Invalid security level supplied, do not understand '%s'", arg.c_str()));
    }

    return security;
}

AlgorithmType getAlgorithmArg(const string & arg) {
    AlgorithmType algorithm;

    if (arg == "aes" || arg == "aes256") {
        algorithm = AlgorithmType::aes_encryption;
    }
    else if (arg == "xor" || arg == "otp") {
        algorithm = AlgorithmType::xor_encryption;
    }
    else if (arg == "none" || arg == "no") {
        algorithm = AlgorithmType::no_encryption;
    }
    else if (arg.length() == 0) {
        algorithm = AlgorithmType::no_encryption;
    }
    else {
        throw clk_error(clk_error::buildMsg("Invalid algorithm supplied, do not understand '%s'", arg.c_str()));
    }

    return algorithm;
}

pair<uint8_t *, size_t> getKeyFromUser() {
    size_t keyBufferLength = (size_t)gcry_md_get_algo_dlen(GCRY_MD_SHA3_256);

    uint8_t * keyBuffer = (uint8_t *)malloc(keyBufferLength);

    if (keyBuffer == NULL) {
        throw clk_error(
                clk_error::buildMsg(
                    "Failed to allocate %zu bytes for the key", 
                    keyBufferLength), 
                __FILE__, 
                __LINE__);
    }

#ifndef RUN_IN_DEBUGGER
    string password;
	int i = 0;

    cout << "Enter password: ";

    while (i < MAX_PASSWORD_LENGTH) {
        int ch = __getch();

        if (ch != '\n' && ch != '\r') {
            putchar('*');
            fflush(stdout);
            password += (char)ch;
        }
        else {
            break;
        }
    }

    putchar('\n');
    fflush(stdout);
#else
    string password = "password";
#endif

	gcry_md_hash_buffer(GCRY_MD_SHA3_256, keyBuffer, password.c_str(), password.length());
    password.clear();

    pair<uint8_t *, size_t> key = {keyBuffer, keyBufferLength};

    return key;
}

pair<uint8_t *, size_t> getKeyFromFile(const string & keyFilename) {
    CloakableInputFile keyFile;
    keyFile.open(keyFilename);

    size_t keyBufferLength = keyFile.size();

    uint8_t * keyBuffer = (uint8_t *)malloc(keyBufferLength);

    if (keyBuffer == NULL) {
        keyFile.close();

        throw clk_error(
                clk_error::buildMsg(
                    "Failed to allocate %zu bytes for the key", 
                    keyBufferLength), 
                __FILE__, 
                __LINE__);
    }

    keyFile.read(keyBuffer, keyBufferLength);

    keyFile.close();

    pair<uint8_t *, size_t> key = {keyBuffer, keyBufferLength};

    return key;
}

pair<uint8_t *, size_t> generateOTP(const string & keyFilename, size_t keyLength) {
    uint8_t * keyBuffer = (uint8_t *)malloc(keyLength);

    if (keyBuffer == NULL) {
        throw clk_error(
                clk_error::buildMsg(
                    "Failed to allocate %zu bytes for the key", 
                    keyLength), 
                __FILE__, 
                __LINE__);
    }

    FILE * fptrRand = fopen("/dev/random", "rb");

    if (fptrRand == NULL) {
        throw clk_fatal("Failed to open random device for reading", __FILE__, __LINE__);
    }

    FILE *fptrOutput = fopen(keyFilename.c_str(), "wb");

    if (fptrOutput == NULL) {
        throw clk_error(
                clk_error::buildMsg(
                    "Failed to open otp output file '%s' for writing", 
                    keyFilename.c_str()),
                __FILE__,
                __LINE__);
    }

    cout << "Wrote " << to_string(keyLength) << " random bytes to '" << keyFilename << "'" << endl << endl;

    fread(keyBuffer, sizeof(uint8_t), keyLength, fptrRand);
    fwrite(keyBuffer, sizeof(uint8_t), keyLength, fptrOutput);

    fclose(fptrRand);
    fclose(fptrOutput);

    pair<uint8_t *, size_t> key = {keyBuffer, keyLength};

    return key;
}

int main(int argc, char ** argv) {
    int defaultLogLevel = LOG_LEVEL_ALL;
    string algo;
    string securityLevel;
    string operation;
    string imageFilename;
    string dataFilename;
    string keyFilename;
    uint8_t * key;
    size_t keyLength;
    bool generateKey = false;

    CmdArg cmdArg = CmdArg(argc, argv);

#ifndef RUN_IN_DEBUGGER
    while (cmdArg.hasMoreArgs()) {
        string arg = cmdArg.nextArg();

        if (arg == OPERATION_MERGE || arg == OPERATION_EXTRACT) {
            operation = arg;
        }
        else if (arg == "-algo") {
            algo = cmdArg.nextArg();
        }
        else if (arg =="-security-level" || arg == "-sl") {
            securityLevel = cmdArg.nextArg();
        }
        else if (arg == "-i" || arg == "-image") {
            imageFilename = cmdArg.nextArg();
        }
        else if (arg == "-key" || arg == "-k") {
            keyFilename = cmdArg.nextArg();
        }
        else if (arg == "-g" || arg == "-generate") {
            keyFilename = cmdArg.nextArg();
            generateKey = true;
        }
        else if (arg == "-v" || arg == "-version") {
            cout << "clk version " << getVersion() << ", build date [" << getBuildDate() << "]" << endl << endl;
            return 0;
        }
        else if (cmdArg.isLastArg()) {
            dataFilename = arg;
        }
    }

#else
    operation = "merge";
    algo = "aes256";
    securityLevel = "high";
    imageFilename = "/Users/guy/flowers.png";
    dataFilename = "/Users/guy/BAU-E2E.pptx";
#endif

    Logger & log = Logger::getInstance();
    log.init("clk.log", defaultLogLevel);

    try {
        AlgorithmType algorithm = getAlgorithmArg(algo);

        if (algorithm == AlgorithmType::aes_encryption) {
            generateKey = false;

            pair<uint8_t *, size_t> keyPair = getKeyFromUser();

            key = keyPair.first;
            keyLength = keyPair.second;
        }
        else if (algorithm == AlgorithmType::xor_encryption) {
            if (!generateKey) {
                pair<uint8_t *, size_t> keyPair = getKeyFromFile(keyFilename);

                key = keyPair.first;
                keyLength = keyPair.second;
            }
        }

        PNGReader * reader = new PNGReader();
        reader->open(imageFilename);

        PNGHost host;
        host.setCloakSecurityLevel(getSecurityLevelArg(securityLevel));

        if (operation == OPERATION_MERGE) {
            auto file = CloakableFileFactory::createInputFile(dataFilename, algorithm);

            size_t initBufferSize = file->getInitialisationBlockBufferSize();
            uint8_t * initBuffer = file->getInitialisationBlockBuffer();

            file->fillInitialisationBlockBuffer(initBuffer);

            size_t imageCapacity = reader->getCapacity(initBufferSize, getSecurityLevelArg(securityLevel));

            if (file->size() > imageCapacity) {
                delete reader;
                file->close();

                throw clk_error(
                    clk_error::buildMsg(
                        "The selected file '%s' (%zu bytes) is too large for the selected image '%s' (capacity %zu bytes)",
                        dataFilename.c_str(),
                        file->size(),
                        imageFilename.c_str(),
                        imageCapacity), 
                    __FILE__, __LINE__);
            }

            // Add initialisation block to image...
            host.addBlock(reader, initBuffer, initBufferSize);

            if (algorithm != AlgorithmType::no_encryption) {
                if (generateKey) {
                    pair<uint8_t *, size_t> keyPair = generateOTP(keyFilename, file->size());

                    file->setKey(keyPair.first, keyPair.second);
                }
                else {
                    file->setKey(key, keyLength);
                }
            }

            size_t blockSize = file->getBlockSize();
            uint8_t * buffer = file->getAllocatedBlock();

            while (file->hasMoreBlocks()) {
                file->readBlock(buffer);
                host.addBlock(reader, buffer, blockSize);
            }

            file->close();

            PNGWriter writer;
            writer.assignImageDetails(reader->getPNGDetails());

            reader->close();

            writer.open(imageFilename);
            writer.close();
        }
        else if (operation == OPERATION_EXTRACT) {
            auto file = CloakableFileFactory::createOutputFile(dataFilename, algorithm);

            size_t initBufferSize = file->getInitialisationBlockBufferSize();
            uint8_t * initBuffer = file->getInitialisationBlockBuffer();

            host.extractBlock(reader, initBuffer, initBufferSize);

            file->extractInitialisationBlockFromBuffer(initBuffer);

            if (algorithm != AlgorithmType::no_encryption) {
                file->setKey(key, keyLength);
            }

            uint8_t * buffer = file->getAllocatedBlock();
            size_t blockSize = file->getBlockSize();

            while (file->getBytesLeftToWrite() > 0) {
                host.extractBlock(reader, buffer, blockSize);
                file->writeBlock(buffer);
            }

            file->close();
            reader->close();
        }
        else {
            delete reader;
            throw clk_error("Invalid operation supplied");
        }

        delete reader;
    }
    catch (clk_error & e) {
        cout << "ERROR: caught exception: " << e.what() << endl << endl;
    }

    log.close();

    return 0;
}
