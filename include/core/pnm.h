#ifndef D033A8B5_97FB_421A_B35C_76213600980C
#define D033A8B5_97FB_421A_B35C_76213600980C

#include <algorithm>
#include <string>
#include <vector>
#include "color_space.h"
#include "pixel.h"
#include "pnm_type.h"

#include "body.h"
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
  [[nodiscard]] bytes GetRaw() const;

  template <color_space::ColorSpace From, color_space::ColorSpace To,
            color_space::Mask Channel>
  friend PNM<To> ColorSpaceConversion(const PNM<From>&);

  friend bool operator==(const PNM<colorSpace>& a, const PNM<colorSpace>& b) {
    return a.header_ == b.header_ && a.body_ == b.body_;
  }

 private:
  Header header_{};
  Body<colorSpace> body_{};
  static void cursor_skip_whitespaces(size_t& cursor, const bytes& buffer);
  static int32_t read_int(size_t& cursor, const bytes& buffer);
};

template <color_space::ColorSpace From, color_space::ColorSpace To,
          color_space::Mask Channel = color_space::Mask::ALL>
PNM<To> ColorSpaceConversion(const PNM<From>& from) {
  if constexpr (To == color_space::ColorSpace::NONE) {
    static_assert(From != color_space::ColorSpace::NONE,
                  "convert from NONE to NONE");
    static_assert(Channel != color_space::Mask::ALL,
                  "convert from NONE with ALL mask");
  }
  PNM<To> to;
  to.header_ = from.header_;
  if constexpr (To == color_space::ColorSpace::NONE)
    to.header_.type = PnmType::P5;
  to.body_ = ColorSpaceConversion<From, To, Channel>(from.body_);
  return to;
}

}  // namespace server::core::pnm

#endif /* D033A8B5_97FB_421A_B35C_76213600980C */