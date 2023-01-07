#include "handlers/upload.h"
#include "core/color_space.h"
#include "handlers/storage_component.h"

#include <string>

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/logging/log.hpp>
#include <userver/server/handlers/http_handler_base.hpp>

namespace service_template {

namespace {

using server::core::pnm::color_space::ColorSpace;
using namespace server::core::pnm;

class Upload final : public userver::server::handlers::HttpHandlerBase {
 public:
  static constexpr std::string_view kName = "handler-upload";

  Upload(const userver::components::ComponentConfig& config,
         const userver::components::ComponentContext& component_context)
      : HttpHandlerBase(config, component_context),
        storageService_(
            component_context.FindComponent<StorageServiceComponent>()) {}

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
      PNM<ColorSpace::RGB> p6({buffer.begin(), buffer.end()});
      storageService_.Add<ColorSpace::RGB>(std::move(p6));
      LOG_DEBUG() << "it's pnm - P6";
    } catch (std::exception& e) {
      try {
        PNM<ColorSpace::NONE> p5({buffer.begin(), buffer.end()});
        storageService_.Add<ColorSpace::NONE>(std::move(p5));
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

 private:
  StorageServiceComponent& storageService_;
};

}  // namespace

void AppendUpload(userver::components::ComponentList& component_list) {
  component_list.Append<Upload>();
}

}  // namespace service_template
