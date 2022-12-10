
#ifndef SERVICE_TEMPLATE_SRC_CORE_HEADER_H_
#define SERVICE_TEMPLATE_SRC_CORE_HEADER_H_

#include "body.h"
#include "bytes.h"
#include "pnm_type.h"

namespace server::core::pnm {
struct Header {
  Header() = default;
  Header(bytes type, uint32_t width, uint32_t height, uint32_t max_color_value);
  bytes GetRaw() const;
  uint32_t width{};
  uint32_t height{};
  uint32_t max_color_value{};
  PnmType type{};
  friend bool operator==(const Header& a, const Header& b) {
    return a.width == b.width && a.height == b.height &&
           a.max_color_value == b.max_color_value && a.type == b.type;
  };
};
}  // namespace server::core::pnm

#endif  // SERVICE_TEMPLATE_SRC_CORE_HEADER_H_
