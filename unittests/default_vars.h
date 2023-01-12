#ifndef SERVICE_TEMPLATE_DEFAULT_VARS_H
#define SERVICE_TEMPLATE_DEFAULT_VARS_H

#include <climits>
#include "core/pnm.h"
#include "utils/file.h"

template <auto Space>
using Pnm = server::core::pnm::PNM<Space>;
using ColorSpace = server::core::pnm::color_space::ColorSpace;

std::string_view static constexpr default_raw_p5 = "P5 4 3 255 aaaaaaaaaaaa";
const auto static default_p5 =
    Pnm<ColorSpace::NONE>({default_raw_p5.begin(), default_raw_p5.end()});

const auto static big_p5 = server::core::utils::GetBlankPnm<
    server::core::pnm::color_space::ColorSpace::NONE>(
    1000, 1000, {(server::core::byte)255});

const auto static big_p6 = server::core::utils::GetBlankPnm<
    server::core::pnm::color_space::ColorSpace::RGB>(1000, 1000,
                                                     {155, 205, 88});

const auto static rgb_sample_240 =
    server::core::utils::LoadPnm<ColorSpace::RGB>("sample_240.pnm");

const auto static rgb_sample_1200 =
    server::core::utils::LoadPnm<ColorSpace::RGB>("sample_1200.pnm");

#endif  // SERVICE_TEMPLATE_DEFAULT_VARS_H
