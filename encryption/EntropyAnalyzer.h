#pragma once
#include <cstddef> //gives us size_t, needed in the header because the function uses it.

class EntropyAnalyzer {
public:
    // Computes Shannon entropy in bits/byte for a buffer
    static double shannonEntropy(const unsigned char* data, size_t size); // a pointer to a block of raw bytes. unsigned char means values from 0-255, which is exactly what rawfile data is.
};