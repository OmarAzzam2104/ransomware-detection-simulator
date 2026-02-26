#pragma once
#include <filesystem>
#include "Encryptor.h"
#include "Logger.h"
#include "Detector.h"

class FileProcessor {
private:
    Encryptor encryptor;
    Logger logger;
    Detector detector;

public:
    FileProcessor(unsigned char key);
    void simulateAttack(const std::filesystem::path& folderPath);
};