#include "Decryptor.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <openssl/evp.h>

static const std::string MAGIC = "RSIMv1";

Decryptor::Decryptor(KeyVault& vault)
    : keyVault(vault)
{
}

bool Decryptor::decryptFile(const std::filesystem::path& encFilePath)
{
    // Get original filename by removing .enc extension
    std::string encName = encFilePath.filename().string();
    std::string originalName = encName.substr(0, encName.size() - 4);

    // Look up the key
    const std::vector<unsigned char>* key = keyVault.getKey(originalName);
    if (!key)
    {
        std::cout << "[DECRYPTOR] No key found for: " << originalName << "\n";
        return false;
    }

    // Open .enc file
    std::ifstream in(encFilePath, std::ios::binary);
    if (!in)
    {
        std::cout << "[DECRYPTOR] Cannot open file: " << encFilePath << "\n";
        return false;
    }

    // Read and verify magic header
    char magic[6];
    in.read(magic, 6);
    if (std::string(magic, 6) != MAGIC)
    {
        std::cout << "[DECRYPTOR] Invalid file format.\n";
        return false;
    }

    // Read IV from header
    unsigned char iv[16];
    in.read(reinterpret_cast<char*>(iv), 16);

    // Open output file
    std::filesystem::path outPath = encFilePath.parent_path() / originalName;
    std::ofstream out(outPath, std::ios::binary);
    if (!out)
    {
        std::cout << "[DECRYPTOR] Cannot create output file.\n";
        return false;
    }

    // Initialize decryption context
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key->data(), iv);

    // Decrypt in chunks
    std::vector<unsigned char> inBuffer(4096);
    std::vector<unsigned char> outBuffer(4096 + 16);

    while (in)
    {
        in.read(reinterpret_cast<char*>(inBuffer.data()), inBuffer.size());
        std::streamsize bytesRead = in.gcount();
        if (bytesRead <= 0) break;

        int outLen = 0;
        EVP_DecryptUpdate(ctx, outBuffer.data(), &outLen,
            inBuffer.data(), static_cast<int>(bytesRead));

        if (outLen > 0)
            out.write(reinterpret_cast<char*>(outBuffer.data()), outLen);
    }

    // Finalize decryption
    int finalLen = 0;
    EVP_DecryptFinal_ex(ctx, outBuffer.data(), &finalLen);
    if (finalLen > 0)
        out.write(reinterpret_cast<char*>(outBuffer.data()), finalLen);

    EVP_CIPHER_CTX_free(ctx);

    std::cout << "[DECRYPTOR] Restored: " << originalName << "\n";
    return true;
}