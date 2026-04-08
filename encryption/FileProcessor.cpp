#include "FileProcessor.h"
#include "Encryptor.h"
#include "EntropyAnalyzer.h"
#include "ExtensionChangeRule.h"
#include <ctime>
#include "Decryptor.h"
#include <fstream>
#include <vector>
#include <iostream>
#include <thread>
#include <chrono>

namespace fs = std::filesystem;

static const std::string MAGIC = "RSIMv1"; // A 6-character marker written at the start of every .enc file. "RSIM" stands for Ransomware SIMulator, "v1" is version 1. This is how the decryptor will later recognize a valid encrypted file.

FileProcessor::FileProcessor() //Constructor
    : detectionEngine(70) // risk threshold ,Passes 70 to detection engine constructor
{
  detectionEngine.addRule(std::make_unique<BurstRule>(3, 2.0, 40)); //creates a BurstRule with threshold=3 files, time window = 2.0seconds,weight=40
  detectionEngine.addRule(std::make_unique<EntropyRule>(2.0, 35)); //Creates an EntropyRule with delta threshold=2.0,weight=35
  detectionEngine.addRule(std::make_unique<ExtensionChangeRule>(3, 30));
}

void FileProcessor::simulateAttack(const fs::path& folderPath)
{
    logger.start(); //Resets the counter to 0 and captures the start time.Must be called before the loop begins.
    std::ofstream log("C:\\RansomSimLab\\simulation_log.txt", std::ios::app);
    log << "=== RANSOMWARE SIMULATION LOG ===\n";

    std::time_t now = std::time(nullptr);
    log << "Run Time: " << now << "\n";
    log << "Target Folder: " << folderPath << "\n";
    log << "=====================================\n\n";

    // ✅ Count all files first
    size_t totalFiles = 0;
    for (auto& entry : fs::directory_iterator(folderPath)) // Opens the folder and lets you loop through every item inside it. Each entry is one item,could be a file or a subfolder.
    {
        if (entry.is_regular_file()) //Returns true only for actual files,not subfoldersmor special system files.
            totalFiles++;
    }

    bool halted = false;

    for (auto& entry : fs::directory_iterator(folderPath))
    {
        if (!entry.is_regular_file())
            continue;

        fs::path input = entry.path(); //gets the full path of the current file.
        fs::path output = input.string() + ".enc"; //Builds the output path by appending".enc" to the original filename.
        if (input.extension() == ".enc") continue; //Skips already encrypted files.

        if (input.extension() == ".enc") continue;

        // Honeypot check
        if (input.filename() == "_honeypot_do_not_touch.txt")
        {
            std::cout << "\n[HONEYPOT TRIGGERED] Trap file accessed: "
                << input.filename() << "\n";
            std::cout << "[ALERT] Ransomware detected via honeypot!\n";
            std::cout << "[PREVENTION] Encryption halted immediately.\n\n";

            log << "\n[HONEYPOT TRIGGERED] Trap file accessed: "
                << input.filename() << "\n";
            log << "[ALERT] Ransomware detected via honeypot!\n";
            log << "[PREVENTION] Encryption halted immediately.\n\n";

            halted = true;
            break;
        }
       

        std::ifstream in(input, std::ios::binary); //Opens the file for reading in binary mode.
                                                  //std::ios::binary is critical, without it, on Windows, certain byte values get automatically converted which would corrupt the data. 
        if (!in)
            continue; //If the file failed to open for any reason , skip it safely instead of crashing.

       

        std::ofstream out(output, std::ios::binary); //Opens the output .enc file for writing in binary mode.
        if (!out)
            continue;

        Encryptor encryptor; //Creates a fresh encryptor, gen new random key and IV.
        if (!encryptor.init())
        {
            std::cout << "Encryption init failed\n";
            continue;
        }

        // Header
        out.write(MAGIC.c_str(), MAGIC.size()); //Writes the 6-byte magic header to the output file.
        out.write(reinterpret_cast<const char*>(encryptor.getIV()), 16);//reinterpret_cast<const char*>(encryptor.getIV()) — Writes the 16-byte IV after the magic. reinterpret_cast converts between pointer types — here converting unsigned char* to char* because write() expects char*. It doesn't change the data, just the type label.

        std::vector<unsigned char> inBuffer(4096); // A 4096-byte Buffer for reading input. Files are processed in 4KB chunks instead of all at once - efficient for large files.
        std::vector<unsigned char> outBuffer(4096 + 16); //Output Buffer is slightly larger because AES-CBC can add up to 16 bytes of padding per chunk.

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
                    maxEntropyDelta = delta; //Keeps only the highest entropy jump seen across all chunk of this file.
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
        keyVault.storeKey(input.filename().string(), encryptor.getKey(), 32);
        logger.logFile(); //Increments the encrypted file counter, Called after the file is fully encrypted.
        logger.logExtensionChange();
        // ✅ Build the context expected by your DetectionEngine
        DetectionContext ctx;// Creates a fresh context object and loads it with this files max entropy delta.

        ctx.lastEntropyDelta = maxEntropyDelta;

        std::cout << "Encrypted: " << input.filename()
            << " | EntropyDelta(max): " << maxEntropyDelta << "\n";
        log << "Encrypted: " << input.filename()
            << " | EntropyDelta(max): " << maxEntropyDelta << "\n";

        // ✅ Evaluate multilayer detection
        int risk = detectionEngine.evaluate(logger, ctx, true, &log); //Runs all rules,prints triggered ones, returns total risk score.

        if (detectionEngine.isMalicious(risk)) //if score >=70,returns true.
        {
            std::cout << "\n[ALERT] Multi-layer ransomware behavior detected!\n";
            log << "\n[ALERT] Multi-layer ransomware behavior detected!\n";
            std::cout << "Risk Score: " << risk << "\n";
            log << "Risk Score: " << risk << "\n";
            std::cout << "[PREVENTION] Encryption halted immediately.\n\n";
            log << "[PREVENTION] Encryption halted immediately. \n\n";
            halted = true;
            break;
        }
    
       
    }

    size_t encrypted = logger.getFilesEncrypted();
    size_t protectedFiles = totalFiles > encrypted ? totalFiles - encrypted : 0; //


    // Decrypt all .enc files using stored keys
    std::cout << "\n===== DECRYPTION =====\n";
    Decryptor decryptor(keyVault);
    for (auto& entry : fs::directory_iterator(folderPath))
    {
        if (entry.path().extension() == ".enc")
            decryptor.decryptFile(entry.path());
    }


    std::cout << "===== REPORT =====\n";
    log << "===== REPORT =====\n";
    std::cout << "Total files in folder: " << totalFiles << "\n";
    log << "Total files: " << totalFiles << "\n";
    std::cout << "Files encrypted before detection: " << encrypted << "\n";
    log << "Files encrypted: " << encrypted << "\n";
    std::cout << "Files protected from attack: " << protectedFiles << "\n";
    log << "Files protected: " << protectedFiles << "\n";
    std::cout << "Status: " << (halted ? "HALTED (Detected)" : "COMPLETED (No Detection)") << "\n";
    log << "Status: " << (halted ? "HALTED (Detected)" : "COMPLETED") << "\n";
}

