#pragma once
#include <chrono>

class Logger {
private:
    std::chrono::steady_clock::time_point startTime; // a variable that stores a moment in time.
                                                     // steady_clock is a clock that never goes backwards-imp for measuring durations reliably
    size_t filesEncrypted = 0; // a counter that starts at 0. size_t is an unsigned int type used for counting things - it can never be negative
    size_t extensionChanges = 0;

public: // functions can be called from anywhere outside the class
    void start(); // void returns nothing, in this case coid records the start time and reset counter
    void logFile();
    void logExtensionChange();
    size_t getFilesEncrypted() const;
    double getElapsedSeconds() const;
    size_t getExtensionChanges() const;
};