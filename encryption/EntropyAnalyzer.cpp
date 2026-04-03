#include "EntropyAnalyzer.h"
#include <array>
#include <cmath>

double EntropyAnalyzer::shannonEntropy(const unsigned char* data, size_t size)
{
    if (!data || size == 0) return 0.0; //safety check. If the pointer is null or the size is zero, return 0 immediatly rather than crash.

    std::array<size_t, 256> freq{}; //creates a fixed array of 256 counters, all starting at 0, 
    for (size_t i = 0; i < size; ++i)
        freq[data[i]]++; 

    double entropy = 0.0;
    const double invSize = 1.0 / static_cast<double>(size); // performance optimization. //converts sizee from an integer to a decimal number so the division gives a proper decimal result

    for (size_t count : freq)
    {
        if (count == 0) continue;
        double p = static_cast<double>(count) * invSize; // calc the probability of this byte value appearing
        entropy -= p * std::log2(p); // this is shannon entropy formula. the minus sign is because log2 of a number between 0 and 1 is always negative,
                                    //minus makes it positive,the more random the data the higher the value get.
    }

    return entropy; // bits per byte 
}