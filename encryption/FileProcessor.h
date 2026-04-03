#pragma once
#include <filesystem>

#include "Logger.h"
#include "DetectionEngine.h"
#include "BurstRule.h"
#include "EntropyRule.h"
#include "DetectionContext.h"
#include "KeyVault.h"

class FileProcessor {
private:
    Logger logger;
    DetectionEngine detectionEngine;
    KeyVault keyVault;

public:
    FileProcessor();
    void simulateAttack(const std::filesystem::path& folderPath);
};