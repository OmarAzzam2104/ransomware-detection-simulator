#include "FileProcessor.h"
#include <iostream>

int main()
{
    std::cout << "==== Ransomware Behavior Detection Prototype ====\n";
    std::cout << "Running controlled simulation inside C:\\RansomSimLab\\DemoFolder\n\n";

    std::filesystem::path demoPath = "C:\\RansomSimLab\\DemoFolder";

    FileProcessor processor(0xAA);
    processor.simulateAttack(demoPath);

    return 0;
}