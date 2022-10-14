#include <fstream>

#include "file.h"

bytes load(std::string file) {
  std::ifstream infile(file);
  infile.seekg(0, std::ios::end);
  size_t length = infile.tellg();
  infile.seekg(0, std::ios::beg);
  auto buffer = new char[length];
  infile.read(buffer, length);
  bytes result(length);
  for (size_t i = 0; i != length; i++) result[i] = buffer[i];
  return result;
}