#include <userver/utest/utest.hpp>

#include "../src/core/pnm.h"

using namespace server::core::pnm;

TEST(PnmBasic, P5Struct) {
  std::string buffer = "P5 2 2 255 aaaa";
  EXPECT_NO_THROW(PNM<ColorSpace::NONE>(bytes{buffer.begin(), buffer.end()}););
}

TEST(PnmBasic, P6Struct) {
  std::string buffer = "P6 2 2 255 aaabbbcccddd";
  EXPECT_NO_THROW(PNM<ColorSpace::RGB>(bytes{buffer.begin(), buffer.end()}););
}