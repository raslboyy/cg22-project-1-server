#include "handlers/dithering.h"

#include <string>

#include "core/color_space.h"
#include "core/pnm.h"
#include "dithering/atkinson.h"
#include "dithering/floyd_steinberg.h"
#include "dithering/ordered.h"
#include "dithering/random.h"

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/logging/log.hpp>
#include <userver/server/handlers/http_handler_base.hpp>

namespace service_template {

namespace {

template <auto Space, auto Algo>
using DitheringFunction = server::core::dithering::Dithering<Space, Algo>;
using Algorithm = server::core::dithering::Algorithm;
using server::core::pnm::color_space::ColorSpace;
using namespace server::core::pnm;

class Dithering final : public userver::server::handlers::HttpHandlerBase {
 public:
  static constexpr std::string_view kName = "handler-dithering";

  Dithering(const userver::components::ComponentConfig& config,
            const userver::components::ComponentContext& component_context)
      : HttpHandlerBase(config, component_context) {}

  std::string HandleRequestThrow(
      const userver::server::http::HttpRequest& request,
      userver::server::request::RequestContext&) const override {
    auto file = request.GetFormDataArg("file").value;
    auto k = request.GetArg("k");
    auto alg = request.GetArg("alg");
    LOG_DEBUG() << k << " " << alg;

    if (file.empty() || k.empty() || alg.empty()) {
      request.GetHttpResponse().SetStatus(
          userver::server::http::HttpStatus::kBadRequest);
      return {};
    }

    auto& response = request.GetHttpResponse();
    response.SetContentType("application/form-data");
    response.SetStatus(userver::server::http::HttpStatus::kOk);
    PNM<ColorSpace::NONE> image({file.begin(), file.end()});

    auto k_int = std::stoi(k);
    server::core::bytes raw;
    if (alg == "random") {
      DitheringFunction<Algorithm::Random, ColorSpace::NONE> dithering(k_int);
      raw = dithering(image).GetRaw();
    } else if (alg == "ordered") {
      DitheringFunction<Algorithm::Ordered, ColorSpace::NONE> dithering(k_int);
      raw = dithering(image).GetRaw();
    } else if (alg == "floyd_steinberg") {
      DitheringFunction<Algorithm::FloydSteinberg, ColorSpace::NONE> dithering(
          k_int);
      raw = dithering(image).GetRaw();
    } else if (alg == "atkinson") {
      DitheringFunction<Algorithm::Atkinson, ColorSpace::NONE> dithering(k_int);
      raw = dithering(image).GetRaw();
    } else {
      throw std::logic_error("not impl");
    }

    return {raw.begin(), raw.end()};
  }
};

}  // namespace

void AppendDithering(userver::components::ComponentList& component_list) {
  component_list.Append<Dithering>();
}

}  // namespace service_template
