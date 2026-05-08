#pragma once
#include "model/Sample.h"
#include <vector>

class SampleView {
public:
    void showSample(const Sample& sample) const;
    void showAllSamples(const std::vector<Sample>& samples) const;
};
