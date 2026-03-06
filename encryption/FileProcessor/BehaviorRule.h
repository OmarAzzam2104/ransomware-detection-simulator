#pragma once
#include "Logger.h"
#include "DetectionContext.h"

class BehaviorRule {
public:
    virtual int evaluate(const Logger& logger, const DetectionContext& ctx) = 0;
    virtual const char* getName() const = 0;
    virtual ~BehaviorRule() = default;
};