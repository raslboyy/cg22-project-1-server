#include <userver/utest/utest.hpp>

#include "core/pnm.h"

using namespace server::core::pnm;
using namespace server::core::pnm::color_space;

TEST(PnmBasic, P5Struct) {
  std::string buffer = "P5\n2 2\n255\nfrfg";
  EXPECT_NO_THROW(
      PNM<color_space::ColorSpace::NONE>(bytes{buffer.begin(), buffer.end()}););
}

TEST(PnmBasic, P6Struct) {
  std::string buffer = "P6 2 2 255 aaabbbcccddd";
  EXPECT_NO_THROW(
      PNM<color_space::ColorSpace::RGB>(bytes{buffer.begin(), buffer.end()}););
}
