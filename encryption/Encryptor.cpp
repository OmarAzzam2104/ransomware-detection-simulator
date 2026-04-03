#include "Encryptor.h"
#include <cstring>

Encryptor::Encryptor()
{
    ctx = EVP_CIPHER_CTX_new(); // creates a new enc. workspace in memory and stores addr. in ctx.

    // Generate random key and IV (lab simulation)
    RAND_bytes(key, sizeof(key));
    RAND_bytes(iv, sizeof(iv));
}

bool Encryptor::init()
{
    if (!EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv)) //initializes the enc. context. 
        return false;

    return true;
}

bool Encryptor::update(unsigned char* in, int inLen,
    unsigned char* out, int& outLen)
{
    if (!EVP_EncryptUpdate(ctx, out, &outLen, in, inLen))
        return false;

    return true;
}

bool Encryptor::finalize(unsigned char* out, int& outLen)
{
    if (!EVP_EncryptFinal_ex(ctx, out, &outLen))
        return false;

    return true;
}

const unsigned char* Encryptor::getIV() const
{
    return iv;
}

Encryptor::~Encryptor()
{
    EVP_CIPHER_CTX_free(ctx);
}