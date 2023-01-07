#ifndef SERVICE_TEMPLATE_TYPE_H
#define SERVICE_TEMPLATE_TYPE_H

#include "core/pnm.h"

namespace server::core::scaling {

using pnm::color_space::ColorSpace;
using namespace pnm;

enum class Algorithm { NearestNeighbor = 0, BiLinear, Lanczos, BcSplines };

template <Algorithm Algo>
class Scaling {};

}  // namespace server::core::scaling

#endif  // SERVICE_TEMPLATE_TYPE_H
