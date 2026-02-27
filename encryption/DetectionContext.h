#pragma once

struct DetectionContext {
    // Latest entropy delta observed (after - before) for current file chunk or file
    double lastEntropyDelta = 0.0;
};