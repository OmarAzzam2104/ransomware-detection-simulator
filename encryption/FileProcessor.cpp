#include "FileProcessor.h"
#include <fstream>
#include <vector>
#include <iostream>

namespace fs = std::filesystem;

static const std::string MAGIC = "RSIMv1";

FileProcessor::FileProcessor(unsigned char key)
    : detector(3, 2.0)
{
}

void FileProcessor::simulateAttack(const fs::path& folderPath)
{
    logger.start();

    // 🔹 Step 1: Count all files first
    size_t totalFiles = 0;

    for (auto& entry : fs::directory_iterator(folderPath))
    {
        if (entry.is_regular_file())
            totalFiles++;
    }

    size_t encryptedCount = 0;

    // 🔹 Step 2: Start encryption loop
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

        // Write MAGIC header
        out.write(MAGIC.c_str(), MAGIC.size());

        // Write IV at beginning (needed for decryption)
        out.write(reinterpret_cast<const char*>(encryptor.getIV()), 16);

        std::vector<unsigned char> inBuffer(4096);
        std::vector<unsigned char> outBuffer(4096 + 16);

        while (in)
        {
            in.read(reinterpret_cast<char*>(inBuffer.data()), inBuffer.size());
            std::streamsize bytesRead = in.gcount();

            if (bytesRead > 0)
            {
                int outLen = 0;

                if (!encryptor.update(inBuffer.data(),
                    static_cast<int>(bytesRead),
                    outBuffer.data(),
                    outLen))
                {
                    std::cout << "Encryption update failed\n";
                    break;
                }

                out.write(reinterpret_cast<char*>(outBuffer.data()), outLen);
            }
        }

        // Finalize encryption (padding)
        int finalLen = 0;

        if (!encryptor.finalize(outBuffer.data(), finalLen))
        {
            std::cout << "Encryption finalize failed\n";
        }
        else
        {
            out.write(reinterpret_cast<char*>(outBuffer.data()), finalLen);
        }

        logger.logFile();
        encryptedCount++;

        std::cout << "Encrypted: " << input.filename() << "\n";

        // 🔹 Detection check
        if (detector.check(logger))
        {
            std::cout << "\n[ALERT] Ransomware-like burst detected!\n";
            std::cout << "[PREVENTION] Encryption halted immediately.\n\n";
            break;
        }
    }

    // 🔹 Final reporting
    size_t encrypted = logger.getFilesEncrypted();
    size_t protectedFiles = totalFiles > encrypted ? totalFiles - encrypted : 0;

    std::cout << "===== REPORT =====\n";
    std::cout << "Files encrypted before detection: " << encrypted << "\n";
    std::cout << "Files protected from attack: " << protectedFiles << "\n";
}