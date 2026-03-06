#pragma once
#include <filesystem>

#include "Logger.h"
#include "DetectionEngine.h"
#include "BurstRule.h"
#include "EntropyRule.h"
#include "DetectionContext.h"

class FileProcessor {
private:
    Logger logger;
    DetectionEngine detectionEngine;

public:
    FileProcessor(unsigned char key); // keep signature
    void simulateAttack(const std::filesystem::path& folderPath);
};