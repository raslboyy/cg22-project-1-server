#include "handlers/gamma.h"

#include <string>
#include "core/pnm.h"
#include "gamma/gamma.h"

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/logging/log.hpp>
#include <userver/server/handlers/http_handler_base.hpp>

namespace service_template {

namespace {

using server::core::pnm::color_space::ColorSpace;
using namespace server::core::pnm;

class ConvertGamma final : public userver::server::handlers::HttpHandlerBase {
 public:
  static constexpr std::string_view kName = "handler-convert-gamma";

  ConvertGamma(const userver::components::ComponentConfig& config,
               const userver::components::ComponentContext& component_context)
      : HttpHandlerBase(config, component_context) {}

  std::string HandleRequestThrow(
      const userver::server::http::HttpRequest& request,
      userver::server::request::RequestContext&) const override {
    auto file = request.GetFormDataArg("file").value;
    auto gamma = request.GetArg("gamma");

    LOG_DEBUG() << gamma;

    if (file.empty() || gamma.empty()) {
      request.GetHttpResponse().SetStatus(
          userver::server::http::HttpStatus::kBadRequest);
      return {};
    }

    auto& response = request.GetHttpResponse();
    response.SetContentType("application/form-data");
    response.SetStatus(userver::server::http::HttpStatus::kOk);
    PNM<ColorSpace::RGB> image({file.begin(), file.end()});
    auto converted = server::core::gamma::ConvertGamma(image, std::stod(gamma));
    auto raw = converted.GetRaw();

    return {raw.begin(), raw.end()};
  }
};

}  // namespace

void AppendConvertGamma(userver::components::ComponentList& component_list) {
  component_list.Append<ConvertGamma>();
}

}  // namespace service_template
