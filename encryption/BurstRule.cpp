#include "BurstRule.h"

BurstRule::BurstRule(size_t threshold, double seconds, int weight)
    : fileThreshold(threshold),
    timeWindow(seconds),
    riskWeight(weight)
{
}

int BurstRule::evaluate(const Logger& /*logger*/, const DetectionContext& /*ctx*/)
{
    auto now = std::chrono::steady_clock::now();

    // Add current timestamp to the window
    recentFiles.push_back(now);

    // Remove timestamps older than timeWindow seconds
    while (!recentFiles.empty())
    {
        double age = std::chrono::duration<double>(now - recentFiles.front()).count();
        if (age > timeWindow)
            recentFiles.pop_front();
        else
            break;
    }

    // If enough recent files, burst detected
    if (recentFiles.size() >= fileThreshold)
        return riskWeight;

    return 0;
}

const char* BurstRule::getName() const
{
    return "Burst Encryption Activity";
}