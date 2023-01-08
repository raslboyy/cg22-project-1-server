#include "gamma/gamma.h"

namespace server::core::gamma {

PNM<ColorSpace::RGB> ConvertGamma(PNM<ColorSpace::RGB> image, double gamma) {
  image[0][0] = gamma * color_space::Pixel<ColorSpace::RGB>(0, 0, 0);
  return image;
}

}  // namespace server::core::gamma