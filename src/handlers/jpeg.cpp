#include "handlers/jpeg.h"
#include "core/pnm.h"
#include "jpeg/jpeg.h"

#include <string>

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/logging/log.hpp>
#include <userver/server/handlers/http_handler_base.hpp>

namespace service_template {

namespace {

using server::core::pnm::color_space::ColorSpace;
using namespace server::core::pnm;

class Jpeg final : public userver::server::handlers::HttpHandlerBase {
 public:
  static constexpr std::string_view kName = "handler-jpeg";

  Jpeg(const userver::components::ComponentConfig& config,
       const userver::components::ComponentContext& component_context)
      : HttpHandlerBase(config, component_context) {}

  std::string HandleRequestThrow(
      const userver::server::http::HttpRequest& request,
      userver::server::request::RequestContext&) const override {
    auto file = request.GetFormDataArg("file").value;
    if (file.empty()) {
      request.GetHttpResponse().SetStatus(
          userver::server::http::HttpStatus::kBadRequest);
      return {};
    }
    std::string buffer{file.begin(), file.end()};

    auto& response = request.GetHttpResponse();
    response.SetContentType("application/form-data");
    try {
      server::core::jpeg::Jpeg jpeg({file.begin(), file.end()});
      jpeg.Decode();
      LOG_DEBUG() << "it's jpeg";
    } catch (std::exception& e) {
      response.SetStatus(userver::server::http::HttpStatus::kBadRequest);
      LOG_DEBUG() << "not jpeg";
      return {};
    }
    response.SetStatus(userver::server::http::HttpStatus::kOk);

    return buffer;
  }
};

}  // namespace

void AppendJpeg(userver::components::ComponentList& component_list) {
  component_list.Append<Jpeg>();
}

}  // namespace service_template
