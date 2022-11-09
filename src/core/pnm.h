#ifndef D033A8B5_97FB_421A_B35C_76213600980C
#define D033A8B5_97FB_421A_B35C_76213600980C

#include <algorithm>
#include <string>
#include <vector>
#include "color_space.h"
#include "pixel.h"
#include "pnm_type.h"

// #include "body.h"
#include "bytes.h"
#include "header.h"

namespace server::core::pnm {

template <color_space::ColorSpace colorSpace>
class PNM {
 public:
  PNM() = default;
  explicit PNM(bytes&& buffer);
  PNM(const Header& header, const Body<colorSpace>& body);
  [[nodiscard]] uint32_t width() const { return header_.width; }
  [[nodiscard]] uint32_t height() const { return header_.height; }

  template <color_space::ColorSpace From, color_space::ColorSpace To>
  friend PNM<To> ColorSpaceConversion(PNM<From> from);
  friend bool operator==(const PNM<colorSpace>& a, const PNM<colorSpace>& b) {
    return a.header_ == b.header_ && a.body_ == b.body_;
  }

 private:
  Header header_{};
  Body<colorSpace> body_{};
  static void cursor_skip_whitespaces(size_t& cursor, const bytes& buffer);
  static int32_t read_int(size_t& cursor, const bytes& buffer);
};

template <color_space::ColorSpace From, color_space::ColorSpace To>
PNM<To> ColorSpaceConversion(PNM<From> from);
}  // namespace server::core::pnm

#endif /* D033A8B5_97FB_421A_B35C_76213600980C */