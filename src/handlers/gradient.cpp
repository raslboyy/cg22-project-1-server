#include "handlers/gradient.h"

#include <string>

#include "core/color_space.h"
#include "core/pnm.h"
#include "utils/file.h"

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/logging/log.hpp>
#include <userver/server/handlers/http_handler_base.hpp>

namespace service_template {

namespace {

using server::core::pnm::color_space::ColorSpace;
using namespace server::core::pnm;

class Gradient final : public userver::server::handlers::HttpHandlerBase {
 public:
  static constexpr std::string_view kName = "handler-gradient";

  Gradient(const userver::components::ComponentConfig& config,
           const userver::components::ComponentContext& component_context)
      : HttpHandlerBase(config, component_context) {}

  std::string HandleRequestThrow(
      const userver::server::http::HttpRequest& request,
      userver::server::request::RequestContext&) const override {
    auto file = request.GetFormDataArg("file").value;
    auto w = request.GetArg("w");
    auto h = request.GetArg("h");
    LOG_DEBUG() << w << " " << h;

    if (file.empty() || w.empty() || h.empty()) {
      request.GetHttpResponse().SetStatus(
          userver::server::http::HttpStatus::kBadRequest);
      return {};
    }

    auto& response = request.GetHttpResponse();
    response.SetContentType("application/form-data");
    response.SetStatus(userver::server::http::HttpStatus::kOk);
    PNM<ColorSpace::NONE> image({file.begin(), file.end()});

    auto raw = server::core::utils::GenerateGradient<ColorSpace::NONE>(
                   std::stoi(h), std::stoi(w))
                   .GetRaw();

    return {raw.begin(), raw.end()};
  }
};

}  // namespace

void AppendGradient(userver::components::ComponentList& component_list) {
  component_list.Append<Gradient>();
}

}  // namespace service_template
