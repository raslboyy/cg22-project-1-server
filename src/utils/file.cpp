#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>

#include "utils/file.h"

namespace server::core::utils {

bytes load(std::string file) {
  std::ifstream infile(file);
  infile.seekg(0, std::ios::end);
  size_t length = infile.tellg();
  infile.seekg(0, std::ios::beg);
  auto buffer = new char[length];
  infile.read(buffer, length);
  bytes result(buffer, buffer + length);
  for (size_t i = 0; i != length; i++) result[i] = buffer[i];
  delete[] buffer;
  return result;
}

void save(const bytes& raw, std::string name) {
  std::ofstream out(name);
  if (out.fail()) throw std::invalid_argument("file does not exist");
  std::copy(raw.begin(), raw.end(), std::ostream_iterator<byte>(out, ""));
}

template <>
PNM<ColorSpace::NONE> GenerateGradient(int n, int m) {
  PNM<ColorSpace::NONE> pnm(m, n);

  int byte_max = std::numeric_limits<byte>::max();
  for (int i = 0; i < n; i++)
    for (int j = 0; j < m; j++)
      pnm[i][j].scale =
          static_cast<byte>(std::round(byte_max * ((double)j / m)));

  return pnm;
}

}  // namespace server::core::utils
