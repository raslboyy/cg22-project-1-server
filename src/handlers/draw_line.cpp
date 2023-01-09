#include "handlers/draw_line.h"

#include "core/pnm.h"
#include "entities/line.h"
#include "handlers/storage_component.h"

#include <userver/logging/log.hpp>
#include <userver/server/handlers/http_handler_base.hpp>

namespace service_template {
namespace {
template <auto Space>
using Line = server::entities::Line<Space>;
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
    auto file = request.GetFormDataArg("file").value;
    auto x1 = request.GetArg("x1");
    auto x2 = request.GetArg("x2");
    auto y1 = request.GetArg("y1");
    auto y2 = request.GetArg("y2");
    auto thickness = request.GetArg("thickness");
    auto alpha = request.GetArg("alpha");
    auto r = request.GetArg("r");
    auto g = request.GetArg("g");
    auto b = request.GetArg("b");
    LOG_DEBUG() << x1 << " " << x2 << " " << y1 << " " << y2 << " " << thickness
                << " " << alpha << " " << r << " " << g << " " << b;
    if (file.empty() || x1.empty() || x2.empty() || y1.empty() || y2.empty() ||
        thickness.empty() || alpha.empty() || r.empty() || g.empty() ||
        b.empty()) {
      request.GetHttpResponse().SetStatus(
          userver::server::http::HttpStatus::kBadRequest);
      return {};
    }

    auto& response = request.GetHttpResponse();
    response.SetContentType("application/form-data");
    response.SetStatus(userver::server::http::HttpStatus::kOk);
    PNM<ColorSpace::RGB> image({file.begin(), file.end()});
    Line<ColorSpace::RGB> line(
        {std::stoi(x1), std::stoi(y1)}, {std::stoi(x2), std::stoi(y2)},
        std::stoi(thickness), {std::stoi(r), std::stoi(g), std::stoi(b)},
        std::stof(alpha));
    auto raw = line(image).GetRaw();

    return {raw.begin(), raw.end()};
  }

 private:
  StorageServiceComponent& storageService_;
};

}  // namespace

void AppendDrawLine(userver::components::ComponentList& component_list) {
  component_list.Append<DrawLine>();
}
}  // namespace service_template
