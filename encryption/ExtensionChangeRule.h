#pragma once
#include "BehaviorRule.h"

class ExtensionChangeRule : public BehaviorRule {
private:
    size_t changeThreshold;
    int riskWeight;

public:
    ExtensionChangeRule(size_t threshold, int weight);
    int evaluate(const Logger& logger, const DetectionContext& ctx) override;
    const char* getName() const override;
};