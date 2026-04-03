#pragma once
#include <filesystem>
#include "KeyVault.h"

class Decryptor {
private:
	KeyVault& keyVault;

public:
	Decryptor(KeyVault& vault);
	bool decryptFile(const std::filesystem::path& encFilePath);

};