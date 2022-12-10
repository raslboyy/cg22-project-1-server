#ifndef SERVICE_TEMPLATE_SRC_CORE_COLOR_SPACE_H_
#define SERVICE_TEMPLATE_SRC_CORE_COLOR_SPACE_H_

namespace server::core::pnm::color_space {
enum class ColorSpace { RGB, HSL, HSV, YCbCr601, YCbCr709, YCoCg, CMY, NONE };
enum class Mask { FIRST, SECOND, THIRD, ALL };
}  // namespace server::core::pnm::color_space

#endif  // SERVICE_TEMPLATE_SRC_CORE_COLOR_SPACE_H_
