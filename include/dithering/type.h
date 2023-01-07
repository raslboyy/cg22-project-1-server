#ifndef SERVICE_TEMPLATE_TYPE_H
#define SERVICE_TEMPLATE_TYPE_H

#include "core/pnm.h"

namespace server::core::dithering {

using pnm::color_space::ColorSpace;
using namespace pnm;

enum class Algorithm { Random = 0, Ordered, FloydSteinberg, Atkinson };

template <Algorithm Algo, ColorSpace Space = ColorSpace::NONE>
class Dithering {};

}  // namespace server::core::dithering

#endif  // SERVICE_TEMPLATE_TYPE_H
