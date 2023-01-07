#include "core/pnm.h"
#include "handlers/upload.h"

#include <functional>
#include <string>

#include <userver/logging/log.hpp>
#include <userver/server/handlers/http_handler_base.hpp>

namespace service_template {

namespace {
using namespace server::core::pnm;
using ColorSpace = color_space::ColorSpace;
using Mask = color_space::Mask;

std::string ConvertFromTo(std::string_view from, std::string_view to,
                          std::string_view channel, std::string_view file);

class Convert final : public userver::server::handlers::HttpHandlerBase {
 public:
  static constexpr std::string_view kName = "handler-convert";

  using HttpHandlerBase::HttpHandlerBase;

  std::string HandleRequestThrow(
      const userver::server::http::HttpRequest& request,
      userver::server::request::RequestContext&) const override {
    auto file = request.GetFormDataArg("file").value;
    auto from = request.GetArg("from");
    auto to = request.GetArg("to");
    auto channel = request.GetArg("channel");
    LOG_DEBUG() << channel << " " << from << " " << to;
    if (file.empty() || from.empty() || to.empty()) {
      request.GetHttpResponse().SetStatus(
          userver::server::http::HttpStatus::kBadRequest);
      return {};
    }

    auto& response = request.GetHttpResponse();
    response.SetContentType("application/form-data");
    // TODO: template magic (string to enum)
    return ConvertFromTo(from, to, channel, file);
  }
};

template <ColorSpace From, ColorSpace To, Mask Channel>
std::string ConvertFromTo(std::string_view file) {
  PNM<From> from({file.begin(), file.end()});
  auto to = ColorSpaceConversion<From, To, Channel>(from);
  auto raw = to.GetRaw();
  return {raw.begin(), raw.end()};
}

template <ColorSpace From, ColorSpace To>
std::string ConvertFromTo(std::string_view channel, std::string_view file) {
  if constexpr (To == ColorSpace::NONE) {
    if (channel == "one")
      return ConvertFromTo<From, To, Mask::FIRST>(file);
    else if (channel == "two")
      return ConvertFromTo<From, To, Mask::SECOND>(file);
    else if (channel == "three")
      return ConvertFromTo<From, To, Mask::THIRD>(file);
  } else {
    if (channel == "all") return ConvertFromTo<From, To, Mask::ALL>(file);
  }
  throw std::logic_error("not impl");
}

template <ColorSpace From>
std::string ConvertFromTo(std::string_view to, std::string_view channel,
                          std::string_view file) {
  if (to == "HSL") return ConvertFromTo<From, ColorSpace::HSL>(channel, file);
  throw std::logic_error("not impl");
}

std::string ConvertFromTo(std::string_view from, std::string_view to,
                          std::string_view channel, std::string_view file) {
  if (from == "RGB") return ConvertFromTo<ColorSpace::RGB>(to, channel, file);
  throw std::logic_error("not impl");
}

}  // namespace

void AppendConvert(userver::components::ComponentList& component_list) {
  component_list.Append<Convert>();
}

}  // namespace service_template
