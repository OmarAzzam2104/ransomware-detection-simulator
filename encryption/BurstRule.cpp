#include "BurstRule.h"

BurstRule::BurstRule(size_t threshold, double seconds, int weight)
    : fileThreshold(threshold),
    timeWindow(seconds),
    riskWeight(weight)
{
}

int BurstRule::evaluate(const Logger& logger)
{
    if (logger.getFilesEncrypted() >= fileThreshold &&
        logger.getElapsedSeconds() <= timeWindow)
    {
        return riskWeight;
    }

    return 0;
}

const char* BurstRule::getName() const
{
    return "Burst Encryption Activity";
}