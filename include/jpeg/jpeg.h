#pragma once

#include <unordered_map>
#include <vector>
#include "core/bytes.h"
#include "core/pnm.h"
#include "jpeg/haffman.h"

namespace server::core::jpeg {

using ColorSpace = pnm::color_space::ColorSpace;

class Jpeg {
 public:
  explicit Jpeg(bytes&& buffer) : buffer(buffer){};
  void Decode();

  pnm::PNM<ColorSpace::YCbCr601> pnm{};

 private:
  int height{};
  int width{};
  std::unordered_map<byte, Huffman> htables_{};
  std::unordered_map<byte, bytes> qtables_{};
  std::vector<int> qtables_mapping;
  bytes buffer;

  template <typename T>
  void Draw(int x, int y, T a, T b, T c);
  auto BuildMatrix(const bytes& data, int& k, byte idx, bytes quant,
                   double olddccoeff);
};
}  // namespace server::core::jpeg