#ifndef SERVICE_TEMPLATE_SRC_UTILS_FILE_H_
#define SERVICE_TEMPLATE_SRC_UTILS_FILE_H_

#include <limits>
#include <string>
#include <utility>

#include "core/pnm.h"

namespace server::core::utils {
using namespace server::core::pnm;
using namespace server::core::pnm::color_space;

bytes load(std::string file);

template <ColorSpace Space>
PNM<Space> LoadPnm(std::string file) {
  auto raw = load(std::move(file));
  return PNM<Space>(std::move(raw));
}

void save(const bytes&, std::string name);

template <ColorSpace Space>
PNM<Space> GetBlankPnm(int n, int m, Pixel<Space> color = Pixel<Space>{}) {
  return PNM<Space>(m, n, color);
}

template <ColorSpace Space>
PNM<Space> GenerateGradient(int n, int m);

template <>
PNM<ColorSpace::NONE> GenerateGradient(int n, int m);

}  // namespace server::core::utils

#endif  // SERVICE_TEMPLATE_SRC_UTILS_FILE_H_
