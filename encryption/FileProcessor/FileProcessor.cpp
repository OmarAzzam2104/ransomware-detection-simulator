#include "FileProcessor.h"
#include "Encryptor.h"
#include "EntropyAnalyzer.h"

#include <fstream>
#include <vector>
#include <iostream>

namespace fs = std::filesystem;

static const std::string MAGIC = "RSIMv1";

FileProcessor::FileProcessor(unsigned char /*key*/)
    : detectionEngine(70) // risk threshold
{
    detectionEngine.addRule(std::make_unique<BurstRule>(3, 2.0, 40));
    detectionEngine.addRule(std::make_unique<EntropyRule>(2.0, 35));
}

void FileProcessor::simulateAttack(const fs::path& folderPath)
{
    logger.start();

    // ✅ Count all files first
    size_t totalFiles = 0;
    for (auto& entry : fs::directory_iterator(folderPath))
    {
        if (entry.is_regular_file())
            totalFiles++;
    }

    bool halted = false;

    for (auto& entry : fs::directory_iterator(folderPath))
    {
        if (!entry.is_regular_file())
            continue;

        fs::path input = entry.path();
        fs::path output = input.string() + ".enc";

        std::ifstream in(input, std::ios::binary);
        if (!in)
            continue;

        std::ofstream out(output, std::ios::binary);
        if (!out)
            continue;

        Encryptor encryptor;
        if (!encryptor.init())
        {
            std::cout << "Encryption init failed\n";
            continue;
        }

        // Header
        out.write(MAGIC.c_str(), MAGIC.size());
        out.write(reinterpret_cast<const char*>(encryptor.getIV()), 16);

        std::vector<unsigned char> inBuffer(4096);
        std::vector<unsigned char> outBuffer(4096 + 16);

        // Track max entropy delta for this file
        double maxEntropyDelta = 0.0;

        while (in)
        {
            in.read(reinterpret_cast<char*>(inBuffer.data()), inBuffer.size());
            std::streamsize bytesRead = in.gcount();

            if (bytesRead <= 0)
                continue;

            double hBefore = EntropyAnalyzer::shannonEntropy(
                inBuffer.data(),
                static_cast<size_t>(bytesRead)
            );

            int outLen = 0;
            if (!encryptor.update(
                inBuffer.data(),
                static_cast<int>(bytesRead),
                outBuffer.data(),
                outLen))
            {
                std::cout << "Encryption update failed\n";
                break;
            }

            if (outLen > 0)
            {
                double hAfter = EntropyAnalyzer::shannonEntropy(
                    outBuffer.data(),
                    static_cast<size_t>(outLen)
                );

                double delta = hAfter - hBefore;
                if (delta > maxEntropyDelta)
                    maxEntropyDelta = delta;
            }

            out.write(reinterpret_cast<char*>(outBuffer.data()), outLen);
        }

        // Finalize (padding)
        int finalLen = 0;
        if (!encryptor.finalize(outBuffer.data(), finalLen))
        {
            std::cout << "Encryption finalize failed\n";
        }
        else if (finalLen > 0)
        {
            out.write(reinterpret_cast<char*>(outBuffer.data()), finalLen);
        }

        // ✅ Log after file is done
        logger.logFile();

        // ✅ Build the context expected by your DetectionEngine
        DetectionContext ctx;
        ctx.lastEntropyDelta = maxEntropyDelta;

        std::cout << "Encrypted: " << input.filename()
            << " | EntropyDelta(max): " << maxEntropyDelta << "\n";

        // ✅ Evaluate multilayer detection
        int risk = detectionEngine.evaluate(logger, ctx);

        if (detectionEngine.isMalicious(risk))
        {
            std::cout << "\n[ALERT] Multi-layer ransomware behavior detected!\n";
            std::cout << "Risk Score: " << risk << "\n";
            std::cout << "[PREVENTION] Encryption halted immediately.\n\n";
            halted = true;
            break;
        }
    }

    size_t encrypted = logger.getFilesEncrypted();
    size_t protectedFiles = totalFiles > encrypted ? totalFiles - encrypted : 0;

    std::cout << "===== REPORT =====\n";
    std::cout << "Total files in folder: " << totalFiles << "\n";
    std::cout << "Files encrypted before detection: " << encrypted << "\n";
    std::cout << "Files protected from attack: " << protectedFiles << "\n";
    std::cout << "Status: " << (halted ? "HALTED (Detected)" : "COMPLETED (No Detection)") << "\n";
}