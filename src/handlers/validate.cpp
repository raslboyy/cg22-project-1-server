#include "validate.h"
#include "pnm.h"

#include <string>

#include <userver/logging/log.hpp>
#include <userver/server/handlers/http_handler_base.hpp>

namespace service_template {

namespace {
using namespace server::core::pnm;
class Validate final : public userver::server::handlers::HttpHandlerBase {
 public:
  static constexpr std::string_view kName = "handler-validate";

  using HttpHandlerBase::HttpHandlerBase;

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
      PNM<color_space::ColorSpace::RGB> p6({buffer.begin(), buffer.end()});
      LOG_DEBUG() << "it's pnm - P6";
    } catch (std::exception& e) {
      try {
        PNM<color_space::ColorSpace::NONE> p5({buffer.begin(), buffer.end()});
        LOG_DEBUG() << "it's pnm - P5";
      } catch (std::exception& e) {
        response.SetStatus(userver::server::http::HttpStatus::kBadRequest);
        LOG_DEBUG() << "not pnm";
        return {};
      }
    }
    response.SetStatus(userver::server::http::HttpStatus::kOk);

    return buffer;
  }
};

}  // namespace

void AppendValidate(userver::components::ComponentList& component_list) {
  component_list.Append<Validate>();
}

}  // namespace service_template
