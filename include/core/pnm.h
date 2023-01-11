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
 private:
  struct BracketsProxy;

 public:
  PNM() = default;
  explicit PNM(bytes&& buffer);
  PNM(uint32_t width, uint32_t height,
      color_space::Pixel<colorSpace> color = color_space::Pixel<colorSpace>{});
  [[nodiscard]] uint32_t width() const { return header_.width; }
  [[nodiscard]] uint32_t height() const { return header_.height; }
  [[nodiscard]] bytes GetRaw() const;

  template <color_space::ColorSpace From, color_space::ColorSpace To,
            color_space::Mask Channel>
  friend PNM<To> ColorSpaceConversion(const PNM<From>&);

  friend bool operator==(const PNM<colorSpace>& a, const PNM<colorSpace>& b) {
    return a.header_ == b.header_ && a.body_ == b.body_;
  }

  auto operator[](size_t i) { return BracketsProxy(*this, i * header_.width); }
  auto operator[](size_t i) const {
    return static_cast<const BracketsProxy>(
        BracketsProxy(const_cast<PNM&>(*this), i * header_.width));
  }

  double gamma() const { return gamma_; }
  void set_gamma(double gamma) { gamma_ = gamma; }

 private:
  Header header_{};
  Body<colorSpace> body_{};
  double gamma_;
  static void cursor_skip_whitespaces(size_t& cursor, const bytes& buffer);
  static int32_t read_int(size_t& cursor, const bytes& buffer);

  struct BracketsProxy {
    BracketsProxy(PNM<colorSpace>& inner, size_t offset)
        : offset(offset), inner(inner) {}
    auto& operator[](size_t j) { return inner.body_.Get(offset + j); }
    auto operator[](size_t j) const { return inner.body_.Get(offset + j); }

   private:
    size_t offset;
    PNM<colorSpace>& inner;
  };
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

static inline std::string to_string(const bytes& raw) {
  return {raw.begin(), raw.end()};
}

}  // namespace server::core::pnm

#endif /* D033A8B5_97FB_421A_B35C_76213600980C */