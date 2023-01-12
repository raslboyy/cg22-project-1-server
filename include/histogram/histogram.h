#pragma once

#include "core/pnm.h"

namespace server::core::histogram {
using namespace server::core::pnm;
using namespace server::core::pnm::color_space;
std::vector<std::vector<int>> CalculateFrequencies(PNM<ColorSpace::RGB> image);
PNM<ColorSpace::RGB> ContrastCorrection(PNM<ColorSpace::RGB> image,
                                        double ignore);
}  // namespace server::core::histogram
