#include <userver/utest/utest.hpp>

#include "default_vars.h"
#include "scaling/bc_splines.h"
#include "scaling/bi_linear.h"
#include "scaling/lanczos.h"
#include "scaling/nearest_neighbor.h"
#include "utils/file.h"

namespace {
using namespace server::core::scaling;
using server::core::pnm::color_space::ColorSpace;
using namespace server::core::utils;
}  // namespace

TEST(Scaling, NearestNeighbor) {
  GTEST_SKIP() << "skip nearest neighbor";
  {
    Scaling<Algorithm::NearestNeighbor> scaling(1200);
    save(scaling(rgb_sample_240).GetRaw(), "scaling/nearest_neighbor_x5.pnm");
  }
  {
    Scaling<Algorithm::NearestNeighbor> scaling(240);
    save(scaling(rgb_sample_1200).GetRaw(),
         "scaling/nearest_neighbor_x0.2.pnm");
  }
}

TEST(Scaling, BiLinear) {
  GTEST_SKIP() << "skip bi linear";
  {
    Scaling<Algorithm::BiLinear> scaling(1200);
    save(scaling(rgb_sample_240).GetRaw(), "scaling/bi_linear_x5.pnm");
  }
  {
    Scaling<Algorithm::BiLinear> scaling(240);
    save(scaling(rgb_sample_1200).GetRaw(), "scaling/bi_linear_x0.2.pnm");
  }
}

TEST(Scaling, Lanczos) {
  GTEST_SKIP() << "skip lanczos";
  {
    Scaling<Algorithm::Lanczos> scaling(1200);
    save(scaling(rgb_sample_240).GetRaw(), "scaling/lanczos_x5.pnm");
  }
  {
    Scaling<Algorithm::Lanczos> scaling(240);
    save(scaling(rgb_sample_1200).GetRaw(), "scaling/lanczos_x0.2.pnm");
  }
}

TEST(Scaling, BcSplines) {
  GTEST_SKIP() << "skip bc splines";
  {
    Scaling<Algorithm::BcSplines> scaling(1200);
    save(scaling(rgb_sample_240).GetRaw(), "scaling/bc_splines_x5.pnm");
  }
  {
    Scaling<Algorithm::BcSplines> scaling(240);
    save(scaling(rgb_sample_1200).GetRaw(), "scaling/bc_splines_x0.2.pnm");
  }
}
