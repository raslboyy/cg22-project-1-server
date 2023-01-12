#include "jpeg/jpeg.h"

#include "jpeg/idct.h"

#include <string>
#include <unordered_map>

#include <iostream>

namespace {

enum Marker {
  START = 0xFFD8,
  HEADER = 0xFFE0,
  QTABLE = 0xFFDB,
  FRAME = 0xFFC0,
  HTABLE = 0xFFC4,
  SCAN = 0xFFDA,
  END = 0xFFD9,
};

std::unordered_map<Marker, std::string> marker_mapping{
    {Marker::START, "Start of Image"},
    {Marker::HEADER, "Application Default Header"},
    {Marker::QTABLE, "Quantization Table"},
    {Marker::FRAME, "Start of Frame"},
    {Marker::HTABLE, "Huffman Table"},
    {Marker::SCAN, "Start of Scan"},
    {Marker::END, "End of Image"},
};

}  // namespace

namespace server::core::jpeg {

static uint16_t ToUint16(byte a, byte b) { return (a << 8) + b; }

static int RemoveFF00(bytes& data) {
  bytes datapro;
  int i = 0;
  while (true) {
    auto a = data[i];
    auto b = data[i + 1];
    if (a == 0xff) {
      if (b != 0) break;
      datapro.emplace_back(data[i]);
      i += 2;
    } else {
      datapro.emplace_back(data[i]);
      i++;
    }
  }
  return i;
}

static auto DecodeNumber(int code, byte bits) {
  auto l = 1 << (code - 1);
  if (bits >= l)
    return bits;
  else
    return byte(bits - (2 * l - 1));
}

auto Jpeg::BuildMatrix(const bytes& data, int& k, byte idx, bytes quant,
                       double olddccoeff) {
  auto i = Idct();

  //  assert(htables_.find(idx) != htables_.end());
  auto res = htables_[idx].Decode(data, k);
  auto code = res.first;
  auto bits = res.second;
  //  auto dccoeff = 0.;
  auto dccoeff = bits + olddccoeff;

  i.base[0] = (dccoeff)*quant[0];
  auto l = 1;
  while (l < 64) {
    res = htables_[16 + idx].Decode(data, k);
    code = res.first;
    if (k == (int)data.size()) break;
    //    code--;

    if (code > 15) {
      l += code >> 4;
      code = code & 0x0F;
    }

    if (k + code >= (int)data.size()) break;
    //    assert(k + code < (int)data.size());
    //    bits = 0;
    //    for (int j = 0; j < code; j++) bits = (bits << 1) + GetBit(data, k++);
    bits = res.second;

    if (l < 64) {
      auto coeff = DecodeNumber(code, bits);
      i.base[l] = coeff * quant[l];
      l += 1;
    }
  }

  i.RearrangeUsingZigzag();
  i.PerformIdct();

  return std::pair{i, dccoeff};
}

// pnm::PNM<pnm::color_space::ColorSpace::RGB>
void Jpeg::Decode() {
  int n = (int)buffer.size();
  std::unordered_map<Marker, bool> check;
  int count = 0;
  for (int i = 0; i < n;) {
    if (i + 1 >= n) throw std::logic_error("incorrect format");
    auto marker = static_cast<Marker>(ToUint16(buffer[i], buffer[i + 1]));
    std::cout << (int)marker << " " << marker_mapping[marker] << std::endl;
    if (check.find(marker) == check.end()) {
      check[marker] = true;
      count++;
    }
    if (marker == Marker::START)
      i += 2;
    else if (marker == Marker::END)
      break;
    else {
      auto chunk_len = ((int)buffer[i + 2] << 8) + buffer[i + 3];
      if (marker == Marker::HTABLE) {
        Huffman huffman(buffer, i + 4);
        htables_.emplace(huffman.header, std::move(huffman));
      } else if (marker == Marker::QTABLE) {
        auto hdr = buffer[i + 4];
        qtables_[hdr] =
            bytes{buffer.begin() + i + 5, buffer.begin() + i + 5 + 64};
      } else if (marker == Marker::FRAME) {
        int offset = 4;
        auto hdr = buffer[i + offset++];
        int h = ToUint16(buffer[i + offset], buffer[i + offset + 1]);
        height = h;
        offset += 2;
        int w = ToUint16(buffer[i + offset], buffer[i + offset + 1]);
        width = w;
        offset += 2;
        auto n_comp = buffer[i + offset++];
        std::cout << (int)h << " " << (int)w << " " << (int)n_comp << std::endl;
        for (int j = 0; j < n_comp; j++) {
          auto id = buffer[3 * j + offset + 2];
          qtables_mapping.emplace_back(id);
          std::cout << 3 * j + offset + 2 << " " << (int)id << std::endl;
        }
      } else if (marker == Marker::SCAN) {
        bytes data(buffer.begin() + i + 4, buffer.end());
        auto len = RemoveFF00(data);
        double oldlumdccoeff = 0;
        double oldCbdccoeff = 0;
        double oldCrdccoeff = 0;

        pnm = pnm::PNM<ColorSpace::YCbCr601>(width, height);

        int k = 0;
        for (int y = 0; y < height / 8; y++)
          for (int x = 0; x < width / 8; x++) {
            if (k == (int)data.size()) continue;
            auto res = BuildMatrix(data, k, qtables_mapping[0],
                                   qtables_[qtables_mapping[0]], oldlumdccoeff);
            auto matL = res.first;
            oldlumdccoeff = res.second;
            if (k == (int)data.size()) continue;
            res = BuildMatrix(data, k, qtables_mapping[0],
                              qtables_[qtables_mapping[1]], oldCrdccoeff);
            auto matCr = res.first;
            oldCrdccoeff = res.second;
            if (k == (int)data.size()) continue;
            res = BuildMatrix(data, k, qtables_mapping[0],
                              qtables_[qtables_mapping[2]], oldCbdccoeff);
            auto matCb = res.first;
            oldCbdccoeff = res.second;

            Draw(x, y, matL.base, matCr.base, matCb.base);
          }

        i = i + 4 + len;
        continue;
      }
      chunk_len += 2;
      i += chunk_len;
    }
  }
  if (count != (int)marker_mapping.size()) throw std::exception();
}

template <typename T>
void Jpeg::Draw(int x, int y, T a, T b, T c) {
  for (int yy = 0; yy < 8; yy++)
    for (int xx = 0; xx < 8; xx++) {
      auto x1 = (x * 8 + xx) * 2;
      auto y1 = (y * 8 + yy) * 2;
      auto x2 = (x * 8 + (xx + 1)) * 2;
      auto y2 = (y * 8 + (yy + 1)) * 2;
      for (int i = x1; i <= x2; i++)
        for (int j = y1; j <= y2; j++) {
          if (j >= (int)pnm.height() || i >= (int)pnm.width()) continue;
          pnm[j][i].luma = a[yy * 8 + xx];
          pnm[j][i].blue_diff = b[yy * 8 + xx];
          pnm[j][i].red_diff = c[yy * 8 + xx];
        }
    }
}

}  // namespace server::core::jpeg
