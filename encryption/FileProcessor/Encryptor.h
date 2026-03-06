#pragma once
#include <openssl/evp.h>
#include <openssl/rand.h>

class Encryptor {
private:
    EVP_CIPHER_CTX* ctx;
    unsigned char key[32];   // 256-bit key
    unsigned char iv[16];    // 128-bit IV

public:
    Encryptor();
    bool init();
    bool update(unsigned char* in, int inLen,
        unsigned char* out, int& outLen);
    bool finalize(unsigned char* out, int& outLen);
    const unsigned char* getIV() const;
    ~Encryptor();
};