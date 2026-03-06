#pragma once
#include <chrono>

class Logger {
private:
    std::chrono::steady_clock::time_point startTime;
    size_t filesEncrypted = 0;

public:
    void start();
    void logFile();
    size_t getFilesEncrypted() const;
    double getElapsedSeconds() const;
};