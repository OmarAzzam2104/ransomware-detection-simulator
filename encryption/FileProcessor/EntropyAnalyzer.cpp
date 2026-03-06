#include "EntropyAnalyzer.h"
#include <array>
#include <cmath>

double EntropyAnalyzer::shannonEntropy(const unsigned char* data, size_t size)
{
    if (!data || size == 0) return 0.0;

    std::array<size_t, 256> freq{};
    for (size_t i = 0; i < size; ++i)
        freq[data[i]]++;

    double entropy = 0.0;
    const double invSize = 1.0 / static_cast<double>(size);

    for (size_t count : freq)
    {
        if (count == 0) continue;
        double p = static_cast<double>(count) * invSize;
        entropy -= p * std::log2(p);
    }

    return entropy; // bits per byte
}