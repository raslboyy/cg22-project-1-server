#include <userver/utest/utest.hpp>

#include "jpeg/haffman.h"
#include "jpeg/jpeg.h"
#include "utils/file.h"

using namespace server::core::jpeg;
using namespace server::core;
using namespace server::core::pnm;
// using ColorSpace = color_space::ColorSpace;

TEST(Jpeg, Basic) {
  auto jpeg = Jpeg(server::core::utils::load("sample_640×426.jpeg"));
  jpeg.Decode();
  //  auto pnm =
  //      ColorSpaceConversion<ColorSpace::YCbCr601, ColorSpace::RGB>(jpeg.pnm);
  server::core::utils::save(jpeg.pnm.GetRaw(), "jpg.ppm");
}

TEST(Jpeg, Huffman) {
  Huffman huffman({0, 0, 1, 5, 1, 1, 1, 1, 1, 1, 0, 0,  0,  0, 0,
                   0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12},
                  0);

  server::core::bytes raw({5});
  int i = 0;
  //  std::cout << (int)huffman.Decode(raw, i) << std::endl;
}