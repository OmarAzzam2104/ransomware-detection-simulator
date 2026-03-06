#pragma once
#include "Logger.h"

class Detector {
private:
    size_t fileThreshold;
    double timeWindow;

public:
    Detector(size_t threshold, double seconds);
    bool check(const Logger& logger);
};