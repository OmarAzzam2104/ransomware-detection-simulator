#pragma once
#include "BehaviorRule.h"

class EntropyRule : public BehaviorRule {
private:
    double deltaThreshold; // e.g. 2.0 bits/byte
    int riskWeight;

public:
    EntropyRule(double deltaThresh, int weight);
    int evaluate(const Logger& logger, const DetectionContext& ctx) override;
    const char* getName() const override;
};