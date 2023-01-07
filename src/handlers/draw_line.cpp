#include "handlers/draw_line.h"

#include "core/pnm.h"
#include "handlers/storage_component.h"

#include <userver/logging/log.hpp>
#include <userver/server/handlers/http_handler_base.hpp>

namespace service_template {
namespace {
using namespace server::core::pnm;
using ColorSpace = color_space::ColorSpace;
using Mask = color_space::Mask;

class DrawLine final : public userver::server::handlers::HttpHandlerBase {
 public:
  static constexpr std::string_view kName = "handler-draw-line";

  DrawLine(const userver::components::ComponentConfig& config,
           const userver::components::ComponentContext& component_context)
      : HttpHandlerBase(config, component_context),
        storageService_(
            component_context.FindComponent<StorageServiceComponent>()) {}

  using HttpHandlerBase::HttpHandlerBase;

  std::string HandleRequestThrow(
      const userver::server::http::HttpRequest& request,
      userver::server::request::RequestContext&) const override {
    auto pnm = storageService_.Get<ColorSpace::RGB>(0);

    return {};
  }

 private:
  StorageServiceComponent& storageService_;
};

}  // namespace

void AppendDrawLine(userver::components::ComponentList& component_list) {
  component_list.Append<DrawLine>();
}
}  // namespace service_template
