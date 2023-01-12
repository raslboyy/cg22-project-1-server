#include "jpeg/haffman.h"

#include <cassert>
#include <iostream>
#include <vector>

namespace server::core::jpeg {

Huffman::Huffman(const bytes& raw, int k) {
  int offset = 0;
  header = raw[k + offset++];

  std::vector<byte> lengths(16);
  for (; offset < 17; offset++) lengths[offset - 1] = raw[k + offset];
  //  for (int i = 0; i < 16; i++) std::cout << (int)lengths[i] << " ";
  //  std::cout << std::endl;

  std::vector<byte> elements;
  for (int num : lengths) {
    for (int i = 0; i < num; i++) elements.emplace_back(raw[k + offset + i]);
    offset += num;
  }

  int ii = 0;
  key_t code = 0;
  for (int i = 0; i < 16; i++) {
    if (lengths[i] == 0) continue;
    for (int j = 0; j < lengths[i]; j++, ii++, code++) {
      assert(code != (1 << (i + 1)) - 1);
      table[{code, i}] = elements[ii];
      //      std::cout << (int)code << std::endl;
    }
    code = code << 1;
  }

  //  for (auto [key, value] : table)
  //    std::cout << (int)key << " " << (int)value << std::endl;
}

int GetBit(const bytes& raw, int k) {
  int offset = k % 8;
  int res = (raw[k / 8] & ((1 << (offset + 1)) - 1)) >> offset;
  assert(res == 0 || res == 1);
  return res;
}

std::pair<int, byte> Huffman::Decode(const bytes& raw, int& k) {
  key_t code = 0;
  for (int i = 1; i < 16 && k < (int)raw.size(); i++, k++) {
    code += GetBit(raw, k);
    //    std::cout << code << std::endl;
    if (table.find({code, i}) != table.end()) return {i, table[{code, i}]};
    code = (code << 1);
  }
  //  std::cout << k << " " << (int)raw.size() << std::endl;
  //  for (auto [key, value] : table)
  //    std::cout << (int)key.first << " " << (int)key.second << " " <<
  //    (int)value
  //              << std::endl;
  //    if (k == (int)raw.size()) return 0;
  //  assert(false);
  //  if (k != (int)raw.size()) throw std::exception();
  return {1, 1};
}

}  // namespace server::core::jpeg
