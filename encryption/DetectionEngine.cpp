#include "DetectionEngine.h"
#include <iostream>
#include <fstream>
DetectionEngine::DetectionEngine(int threshold)
    : riskThreshold(threshold)
{
}

void DetectionEngine::addRule(std::unique_ptr<BehaviorRule> rule)
{
    rules.push_back(std::move(rule));
}

int DetectionEngine::evaluate(const Logger& logger, const DetectionContext& ctx, bool verbose, std::ofstream* log)
{
    int totalRisk = 0;
    for (const auto& rule : rules)
    {
        int ruleRisk = rule->evaluate(logger, ctx);
        if (ruleRisk > 0)
        {
            if (verbose)
                std::cout << "  [RULE TRIGGERED] " << rule->getName()
                << " (+" << ruleRisk << ")\n";
            if (log && log->is_open())
                *log << "  [RULE TRIGGERED] " << rule->getName()
                << " (+" << ruleRisk << ")\n";
        }
        totalRisk += ruleRisk;
    }
    return totalRisk;
}
bool DetectionEngine::isMalicious(int totalRisk) const
{
    return totalRisk >= riskThreshold;
}