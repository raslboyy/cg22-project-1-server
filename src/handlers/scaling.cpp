#include "handlers/scaling.h"

#include "core/color_space.h"
#include "scaling/bc_splines.h"
#include "scaling/bi_linear.h"
#include "scaling/lanczos.h"
#include "scaling/nearest_neighbor.h"

#include <string>

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/logging/log.hpp>
#include <userver/server/handlers/http_handler_base.hpp>

namespace service_template {

namespace {

template <auto Algo>
using ScalingFunction = server::core::scaling::Scaling<Algo>;
using Algorithm = server::core::scaling::Algorithm;
using ShiftFunction = server::core::scaling::Shift;
using server::core::pnm::color_space::ColorSpace;
using namespace server::core::pnm;

class Scaling final : public userver::server::handlers::HttpHandlerBase {
 public:
  static constexpr std::string_view kName = "handler-scaling";

  Scaling(const userver::components::ComponentConfig& config,
          const userver::components::ComponentContext& component_context)
      : HttpHandlerBase(config, component_context) {}

  std::string HandleRequestThrow(
      const userver::server::http::HttpRequest& request,
      userver::server::request::RequestContext&) const override {
    auto file = request.GetFormDataArg("file").value;
    auto x_shift_ = request.GetArg("x_shift");
    auto x_shift = x_shift_.empty() ? 0 : std::stoi(x_shift_);
    auto y_shift_ = request.GetArg("y_shift");
    auto y_shift = y_shift_.empty() ? 0 : std::stoi(y_shift_);
    auto alg = request.GetArg("alg");
    auto B_ = request.GetArg("B");
    auto B = B_.empty() ? 0 : std::stof(B_);
    auto C_ = request.GetArg("C");
    auto C = C_.empty() ? 0 : std::stof(C_);
    auto w_new_ = request.GetArg("w_new");
    auto h_new_ = request.GetArg("h_new");
    auto h_new = h_new_.empty() ? 0 : std::stoi(h_new_);

    LOG_DEBUG() << x_shift << " " << y_shift << " " << alg << " " << B << " "
                << C;

    if (file.empty() || alg.empty()) {
      request.GetHttpResponse().SetStatus(
          userver::server::http::HttpStatus::kBadRequest);
      return {};
    }

    auto& response = request.GetHttpResponse();
    response.SetContentType("application/form-data");
    response.SetStatus(userver::server::http::HttpStatus::kOk);
    PNM<ColorSpace::RGB> image({file.begin(), file.end()});

    ShiftFunction shift(x_shift, y_shift);
    auto w_new = std::stoi(w_new_);
    server::core::bytes raw;
    if (alg == "bc_splines") {
      ScalingFunction<Algorithm::BcSplines> scaling(w_new, h_new, shift, B, C);
      raw = scaling(image).GetRaw();
    } else if (alg == "bi_linear") {
      ScalingFunction<Algorithm::BiLinear> scaling(w_new, h_new, shift);
      raw = scaling(image).GetRaw();
    } else if (alg == "lanczos") {
      ScalingFunction<Algorithm::Lanczos> scaling(w_new, h_new, shift);
      raw = scaling(image).GetRaw();
    } else if (alg == "nearest_neighbor") {
      ScalingFunction<Algorithm::NearestNeighbor> scaling(w_new, h_new, shift);
      raw = scaling(image).GetRaw();
    } else {
      throw std::logic_error("not impl");
    }

    return {raw.begin(), raw.end()};
  }
};

}  // namespace

void AppendScaling(userver::components::ComponentList& component_list) {
  component_list.Append<Scaling>();
}

}  // namespace service_template
