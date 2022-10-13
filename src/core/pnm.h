#ifndef D033A8B5_97FB_421A_B35C_76213600980C
#define D033A8B5_97FB_421A_B35C_76213600980C

#include <string>
#include <vector>
#include <algorithm>

#include "pnm_type.h"

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

template<PnmType type>
struct Pixel;
template<>
struct Pixel<PnmType::P5> {
  int32_t scale{};
};
template<>
struct Pixel<PnmType::P6> {
  int32_t r{};
  int32_t g{};
  int32_t b{};
};

template<PnmType type>
struct Body {
  explicit Body(bytes &&buffer, uint32_t width, uint32_t height);
  Body() = default;
  uint32_t width{};
  uint32_t height{};
  std::vector<Pixel<type>> pixels{};
};

template<PnmType type>
class PNM {
 public:
  explicit PNM(bytes &&buffer);
  [[nodiscard]] uint32_t width() const { return header_.width; }
  [[nodiscard]] uint32_t height() const { return header_.height; }
 private:
  Header header_{};
  Body<type> body_{};
  static void cursor_skip_whitespaces(size_t &cursor, const bytes &buffer);
  static int32_t read_int(size_t &cursor, const bytes &buffer);
};

#endif /* D033A8B5_97FB_421A_B35C_76213600980C */