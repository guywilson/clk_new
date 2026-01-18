#include <string>
#include <memory>

#include "cloakable.h"
#include "encryptable.h"
#include "decryptable.h"
#include "logger.h"

using namespace std;

#ifndef __INCL_FILEFACTORY
#define __INCL_FILEFACTORY

enum class AlgorithmType {
    aes_encryption,
    xor_encryption
};

class CloakableFileFactory {
    public:
        static unique_ptr<CloakableInputFile> createInputFile(const string & filename) {
            unique_ptr<CloakableInputFile> file = make_unique<CloakableInputFile>();
            file->open(filename);
            return file;
        }

        static unique_ptr<CloakableOutputFile> createOutputFile(const string & filename) {
            unique_ptr<CloakableOutputFile> file = make_unique<CloakableOutputFile>();
            file->open(filename);
            return file;
        }
};

class EncryptableFileFactory {
    public:
        static unique_ptr<EncryptableFile> create(const string & filename, const AlgorithmType & type) {
            if (type == AlgorithmType::aes_encryption) {
                unique_ptr<EncryptableFile> file = make_unique<AESEncryptableFile>();
                file->open(filename);
                return file;
            }
            else if (type == AlgorithmType::xor_encryption) {
                unique_ptr<EncryptableFile> file = make_unique<XOREncryptableFile>();
                file->open(filename);
                return file;
            }

            return nullptr;
        }
};

class DecryptableFileFactory {
    public:
        static unique_ptr<DecryptableFile> create(const string & filename, const AlgorithmType & type) {
            if (type == AlgorithmType::aes_encryption) {
                unique_ptr<DecryptableFile> file = make_unique<AESDecryptableFile>();
                file->open(filename);
                return file;
            }
            else if (type == AlgorithmType::xor_encryption) {
                unique_ptr<DecryptableFile> file = make_unique<XORDecryptableFile>();
                file->open(filename);
                return file;
            }

            return nullptr;
        }
};

#endif
