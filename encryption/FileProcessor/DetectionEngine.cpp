#include "DetectionEngine.h"

DetectionEngine::DetectionEngine(int threshold)
    : riskThreshold(threshold)
{
}

void DetectionEngine::addRule(std::unique_ptr<BehaviorRule> rule)
{
    rules.push_back(std::move(rule));
}

int DetectionEngine::evaluate(const Logger& logger, const DetectionContext& ctx)
{
    int totalRisk = 0;
    for (const auto& rule : rules)
        totalRisk += rule->evaluate(logger, ctx);
    return totalRisk;
}

bool DetectionEngine::isMalicious(int totalRisk) const
{
    return totalRisk >= riskThreshold;
}