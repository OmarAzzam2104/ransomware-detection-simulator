#pragma once
#include <map>
#include <string>
#include <vector>

class KeyVault {
private:
	std::map<std::string, std::vector<unsigned char>> keys;

public:
	void storeKey(const std::string& filename, const unsigned char* key, size_t keySize);
	const std::vector<unsigned char>* getKey(const std::string& filename) const;
};