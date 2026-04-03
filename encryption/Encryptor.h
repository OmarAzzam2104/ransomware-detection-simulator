#pragma once
#include <openssl/evp.h> //gives access to openssl encryption functions. EVP is openssl high level interface 
                           //that supports many encryption algo through the same functions
#include <openssl/rand.h> // Openssl cryptographically secure randm number generator.

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
    ~Encryptor(); //~ it means it runs automatically when the object is destroyed. it frees the Openssl context from memory
                    // without this you would have a memory leak.
};