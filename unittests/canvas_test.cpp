#include <userver/utest/utest.hpp>

#include "default_vars.h"

#include "entities/line.h"

template <auto Space>
using Line = server::entities::Line<Space>;

TEST(DrawLine, SingleLine) {
  Line<ColorSpace::NONE> line({0, 0}, {2, 2}, 1, {'b'}, 1.);
  auto with_line = line(default_p5);
  auto expected = "P5 4 3 255 baaaabaaaaba";
  EXPECT_EQ(expected, server::core::pnm::to_string(with_line.GetRaw()));
}

TEST(DrawLine, BoldLine) {
  Line<ColorSpace::NONE> line({1, 0}, {1, 2}, 3, {'b'}, 1.);
  auto with_line = line(default_p5);
  auto expected = "P5 4 3 255 bbbabbbbbbba";
  EXPECT_EQ(expected, server::core::pnm::to_string(with_line.GetRaw()));
}

TEST(DrawLine, HardLine) {
  Line<ColorSpace::NONE> line({100, 100}, {300, 900}, 7, {0}, 1.);
  auto with_line = line(big_p5);
  server::core::utils::save(with_line.GetRaw(), "draw_grayscale_line_out.pgm");
}

TEST(DrawLine, HardLineRgb) {
  Line<ColorSpace::RGB> line({100, 100}, {300, 900}, 13, {154, 92, 130}, 0.3);
  auto with_line = line(big_p6);
  server::core::utils::save(with_line.GetRaw(), "draw_rgb_line_out.pnm");
}
