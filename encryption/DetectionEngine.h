#pragma once
#include <vector>
#include <memory>
#include "BehaviorRule.h"
#include "DetectionContext.h"

class DetectionEngine {
private:
    std::vector<std::unique_ptr<BehaviorRule>> rules;
    int riskThreshold;

public:
    DetectionEngine(int threshold);
    void addRule(std::unique_ptr<BehaviorRule> rule);

    int evaluate(const Logger& logger, const DetectionContext& ctx);
    bool isMalicious(int totalRisk) const;
};