#include "KeyVault.h"

void KeyVault::storeKey(const std::string& filename, const unsigned char* key, size_t keySize)
{
    keys[filename] = std::vector<unsigned char>(key, key + keySize);
}

const std::vector<unsigned char>* KeyVault::getKey(const std::string& filename) const
{
    auto it = keys.find(filename);
    if (it != keys.end())
        return &it->second;
    return nullptr;
}