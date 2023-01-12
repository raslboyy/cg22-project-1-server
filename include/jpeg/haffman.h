#pragma once

#include <map>
#include "core/bytes.h"

#include <string>

namespace server::core::jpeg {
int GetBit(const bytes& raw, int k);
class Huffman {
  using key_t = uint32_t;

 public:
  byte header;

  Huffman() = default;
  Huffman(const bytes& raw, int k);
  std::pair<int, byte> Decode(const bytes& raw, int& k);

 private:
  //  struct Node {
  //    Node* left = nullptr;
  //    Node* right = nullptr;
  //    byte value{};
  //  };
  //  Node* root = nullptr;
  std::map<std::pair<key_t, int>, byte> table;
};
}  // namespace server::core::jpeg