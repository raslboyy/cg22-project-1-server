#ifndef CG22_PROJECT_1_SERVER_SRC_CORE_PNM_TYPE_H_
#define CG22_PROJECT_1_SERVER_SRC_CORE_PNM_TYPE_H_

#include "color_space.h"

namespace server::core::pnm {
enum PnmType { P5 = 5, P6 };
//constexpr ColorSpace PnmTypeToColorSpace(PnmType type) {
//  switch (type) {
//    case P5:
//      return ColorSpace::NONE;
//      break;
//    case P6:
//      return ColorSpace::RGB;
//      break;
//  }
//}
}

#endif  // CG22_PROJECT_1_SERVER_SRC_CORE_PNM_TYPE_H_
