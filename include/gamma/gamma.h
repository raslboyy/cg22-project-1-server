#ifndef SERVICE_TEMPLATE_GAMMA_H
#define SERVICE_TEMPLATE_GAMMA_H

#include "core/pnm.h"

namespace server::core::gamma {

using pnm::color_space::ColorSpace;
using namespace pnm;

PNM<ColorSpace::RGB> ConvertGamma(PNM<ColorSpace::RGB> image, double gamma);

}  // namespace server::core::gamma

#endif  // SERVICE_TEMPLATE_GAMMA_H
