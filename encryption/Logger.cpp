#include "Logger.h"

void Logger::start()
{
    startTime = std::chrono::steady_clock::now(); // captures the exact current moment in time and stores it in startTime.

    filesEncrypted = 0;
    extensionChanges = 0;
}

void Logger::logFile()
{
    filesEncrypted++; //adds 1 to the counter, Called once per file encrypted.
}

size_t Logger::getFilesEncrypted() const 
{
    return filesEncrypted;
}

double Logger::getElapsedSeconds() const
{
    auto now = std::chrono::steady_clock::now(); // auto, let the compiler figure out the type automatically instead of you writing it out. here it becomes a time_point.
    return std::chrono::duration<double>(now - startTime).count(); //
}
void Logger::logExtensionChange()
{
    extensionChanges++;
}
size_t Logger::getExtensionChanges() const
{
    return extensionChanges;
}