#pragma once
#include <cstddef>

class EntropyAnalyzer {
public:
    // Computes Shannon entropy in bits/byte for a buffer
    static double shannonEntropy(const unsigned char* data, size_t size);
};