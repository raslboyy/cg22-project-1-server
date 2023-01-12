#include "handlers/histogram.h"
#include "core/pnm.h"
#include "histogram/histogram.h"

#include <functional>
#include <string>

#include <userver/logging/log.hpp>
#include <userver/server/handlers/http_handler_base.hpp>

namespace service_template {

namespace {
using namespace server::core::pnm;
using ColorSpace = color_space::ColorSpace;

class Histogram final : public userver::server::handlers::HttpHandlerBase {
 public:
  static constexpr std::string_view kName = "handler-histogram";

  using HttpHandlerBase::HttpHandlerBase;

  std::string HandleRequestThrow(
      const userver::server::http::HttpRequest& request,
      userver::server::request::RequestContext&) const override {
    auto file = request.GetFormDataArg("file").value;
    auto channel = request.GetArg("channel");
    LOG_DEBUG() << channel;
    if (file.empty() || channel.empty()) {
      request.GetHttpResponse().SetStatus(
          userver::server::http::HttpStatus::kBadRequest);
      return {};
    }

    userver::formats::json::ValueBuilder response;
    PNM<ColorSpace::RGB> p6({file.begin(), file.end()});
    response["a"].Resize(0);
    for (const auto& i : server::core::histogram::CalculateFrequencies(p6))
      for (auto j : i) response["a"].PushBack(j);
  
    request.GetHttpResponse().SetStatus(userver::server::http::HttpStatus::kOk);
    return userver::formats::json::ToString(response.ExtractValue());
  }
};

}  // namespace

void AppendHistogram(userver::components::ComponentList& component_list) {
  component_list.Append<Histogram>();
}

}  // namespace service_template
