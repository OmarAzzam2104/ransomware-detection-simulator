#pragma once
#include "BehaviorRule.h"

class BurstRule : public BehaviorRule {
private:
    size_t fileThreshold;
    double timeWindow;
    int riskWeight;

public:
    BurstRule(size_t threshold, double seconds, int weight);
    int evaluate(const Logger& logger, const DetectionContext& ctx) override;
    const char* getName() const override;
};