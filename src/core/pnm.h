#ifndef D033A8B5_97FB_421A_B35C_76213600980C
#define D033A8B5_97FB_421A_B35C_76213600980C

#include <algorithm>
#include <string>
#include <vector>

#include "pixel.h"

namespace server::core::pnm {
using byte = char;
using bytes = std::vector<byte>;

struct Header {
  Header() = default;
  Header(bytes type, uint32_t width, uint32_t height, uint32_t max_color_value);
  uint32_t width{};
  uint32_t height{};
  uint32_t max_color_value{};
  PnmType type{};
};

template <ColorSpace colorSpace>
struct Body {
  explicit Body(bytes&& buffer, uint32_t width, uint32_t height);
  Body() = default;
  uint32_t width{};
  uint32_t height{};
  std::vector<Pixel<colorSpace>> pixels{};
};

template <ColorSpace colorSpace>
class PNM {
 public:
  explicit PNM(bytes&& buffer);
  PNM(const Header& header, const Body<colorSpace>& body);
  [[nodiscard]] uint32_t width() const { return header_.width; }
  [[nodiscard]] uint32_t height() const { return header_.height; }

 private:
  Header header_{};
  Body<colorSpace> body_{};
  static void cursor_skip_whitespaces(size_t& cursor, const bytes& buffer);
  static int32_t read_int(size_t& cursor, const bytes& buffer);
};
}  // namespace server::core::pnm

#endif /* D033A8B5_97FB_421A_B35C_76213600980C */