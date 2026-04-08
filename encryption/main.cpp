#include "FileProcessor.h"
#include <iostream>
#include <string>
#include <filesystem>

int main(int argc, char* argv[])
{
    std::cout.setf(std::ios::unitbuf);

    std::cout << "==== Ransomware Behavior Detection Prototype ====\n";
    std::cout << "WARNING: Only run this inside a designated lab folder!\n\n";

    std::filesystem::path demoPath = "C:\\RansomSimLab\\DemoFolder";

    if (argc >= 2)
    {
        std::string input = argv[1];
        if (input.find("RansomSimLab") == std::string::npos)
        {
            std::cout << "[BLOCKED] Unsafe path rejected: " << input << "\n";
            std::cout << "Path must be inside RansomSimLab folder.\n";
            return 1;
        }
        demoPath = input;
    }

    std::cout << "Target folder: " << demoPath << "\n\n";

    FileProcessor processor;
    processor.simulateAttack(demoPath);

    return 0;
}