#include "Logger.h"

void Logger::start()
{
    startTime = std::chrono::steady_clock::now();
    filesEncrypted = 0;
}

void Logger::logFile()
{
    filesEncrypted++;
}

size_t Logger::getFilesEncrypted() const
{
    return filesEncrypted;
}

double Logger::getElapsedSeconds() const
{
    auto now = std::chrono::steady_clock::now();
    return std::chrono::duration<double>(now - startTime).count();
}