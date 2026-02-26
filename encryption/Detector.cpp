#include "Detector.h"

Detector::Detector(size_t threshold, double seconds)
    : fileThreshold(threshold), timeWindow(seconds) {
}

bool Detector::check(const Logger& logger)
{
    return logger.getFilesEncrypted() >= fileThreshold &&
        logger.getElapsedSeconds() <= timeWindow;
}