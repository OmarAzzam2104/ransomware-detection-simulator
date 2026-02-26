#pragma once
#include "Logger.h"

class BehaviorRule {
public:
    virtual int evaluate(const Logger& logger) = 0;
    virtual const char* getName() const = 0;
    virtual ~BehaviorRule() = default;
};