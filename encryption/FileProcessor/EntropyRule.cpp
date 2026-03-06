#include "EntropyRule.h"

EntropyRule::EntropyRule(double deltaThresh, int weight)
    : deltaThreshold(deltaThresh), riskWeight(weight)
{
}

int EntropyRule::evaluate(const Logger& /*logger*/, const DetectionContext& ctx)
{
    // If the latest observed entropy delta is high, add risk
    if (ctx.lastEntropyDelta >= deltaThreshold)
        return riskWeight;

    return 0;
}

const char* EntropyRule::getName() const
{
    return "Entropy Spike";
}