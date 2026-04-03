#include "ExtensionChangeRule.h"

ExtensionChangeRule::ExtensionChangeRule(size_t threshold, int weight)
    : changeThreshold(threshold), riskWeight(weight)
{
}

int ExtensionChangeRule::evaluate(const Logger& logger, const DetectionContext& /*ctx*/)
{
    if (logger.getExtensionChanges() >= changeThreshold)
        return riskWeight;

    return 0;
}

const char* ExtensionChangeRule::getName() const
{
    return "Mass Extension Change";
}