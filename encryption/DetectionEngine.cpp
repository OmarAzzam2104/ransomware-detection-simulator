#include "DetectionEngine.h"

DetectionEngine::DetectionEngine(int threshold)
    : riskThreshold(threshold)
{
}

void DetectionEngine::addRule(std::unique_ptr<BehaviorRule> rule)
{
    rules.push_back(std::move(rule));
}

int DetectionEngine::evaluate(const Logger& logger)
{
    int totalRisk = 0;

    for (const auto& rule : rules)
    {
        totalRisk += rule->evaluate(logger);
    }

    return totalRisk;
}

bool DetectionEngine::isMalicious(int totalRisk) const
{
    return totalRisk >= riskThreshold;
}