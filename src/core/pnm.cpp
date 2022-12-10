#include <cctype>
#include <cwctype>

#include "pnm.h"
#include "userver/logging/log.hpp"

namespace server::core::pnm {
using namespace color_space;

template <color_space::ColorSpace colorSpace>
PNM<colorSpace>::PNM(bytes&& buffer) {
  LOG_DEBUG() << buffer;
  bytes type_ = {buffer[0], buffer[1]};
  size_t cursor = 2;
  cursor_skip_whitespaces(cursor, buffer);
  uint32_t width = read_int(cursor, buffer);
  uint32_t height = read_int(cursor, buffer);
  uint32_t max_color_value = read_int(cursor, buffer);
  header_ = Header(type_, width, height, max_color_value);
  cursor_skip_whitespaces(cursor, buffer);

  if (width == 0 || height == 0) return;

  // TODO: move, dont copy
  auto body = bytes{buffer.begin() + cursor, buffer.end()};
  body_ = Body<colorSpace>{std::move(body), width, height};
}

template <color_space::ColorSpace colorSpace>
bytes PNM<colorSpace>::GetRaw() const {
  auto header = header_.GetRaw();
  auto body = body_.GetRaw();

  auto n = header.size();
  auto raw = std::move(header);
  raw.resize(n + body.size());
  for (int i = n; i < (int)raw.size(); i++) raw[i] = body[i - n];

  return raw;
}

template <color_space::ColorSpace colorSpace>
int32_t PNM<colorSpace>::read_int(size_t& cursor, const bytes& buffer) {
  std::string to_int;
  while (std::isdigit(buffer[cursor])) to_int += buffer[cursor++];
  cursor_skip_whitespaces(cursor, buffer);
  return std::stoi(to_int);
}
template <color_space::ColorSpace colorSpace>
void PNM<colorSpace>::cursor_skip_whitespaces(size_t& cursor,
                                              const bytes& buffer) {
  while (std::iswspace(buffer[cursor])) cursor++;
}
template <color_space::ColorSpace colorSpace>
PNM<colorSpace>::PNM(const Header& header, const Body<colorSpace>& body)
    : header_(header), body_(body) {}

template class PNM<color_space::ColorSpace::NONE>;
template class PNM<color_space::ColorSpace::RGB>;
template class PNM<color_space::ColorSpace::HSL>;

}  // namespace server::core::pnm