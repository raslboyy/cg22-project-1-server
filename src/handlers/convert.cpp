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
    response.SetStatus(userver::server::http::HttpStatus::kOk);
    // TODO: template magic (string to enum)
    return ConvertFromTo(from, to, channel, file);
  }
};

template <ColorSpace From, ColorSpace To, Mask Channel>
PNM<To> ConvertFromTo(std::string_view file) {
  PNM<From> from({file.begin(), file.end()});
  auto rgb = ColorSpaceConversion<From, ColorSpace::RGB, Channel>(from);
  auto to = ColorSpaceConversion<ColorSpace::RGB, To, Channel>(rgb);
  return to;
}

template <ColorSpace From, ColorSpace To>
std::string ConvertFromTo(std::string_view channel, std::string_view file) {
  server::core::bytes raw;
  if (channel == "first") {
    auto res = ColorSpaceConversion<To, ColorSpace::NONE, Mask::FIRST>(
        ConvertFromTo<From, To, Mask::ALL>(file));
    raw = res.GetRaw();
  } else if (channel == "second") {
    auto res = ColorSpaceConversion<To, ColorSpace::NONE, Mask::SECOND>(
        ConvertFromTo<From, To, Mask::ALL>(file));
    raw = res.GetRaw();
  } else if (channel == "third") {
    auto res = ColorSpaceConversion<To, ColorSpace::NONE, Mask::THIRD>(
        ConvertFromTo<From, To, Mask::ALL>(file));
    raw = res.GetRaw();
  } else if (channel == "all") {
    auto res = ConvertFromTo<From, To, Mask::ALL>(file);
    raw = res.GetRaw();
  } else {
    LOG_DEBUG() << "not impl in channel";
    throw std::logic_error("not impl");
  }
  return {raw.begin(), raw.end()};
}

template <ColorSpace From>
std::string ConvertFromTo(std::string_view to, std::string_view channel,
                          std::string_view file) {
  if (to == "RGB") return ConvertFromTo<From, ColorSpace::RGB>(channel, file);
  if (to == "HSL") return ConvertFromTo<From, ColorSpace::HSL>(channel, file);
  if (to == "HSV") return ConvertFromTo<From, ColorSpace::HSV>(channel, file);
  if (to == "YCbCr601")
    return ConvertFromTo<From, ColorSpace::YCbCr601>(channel, file);
  if (to == "YCbCr709")
    return ConvertFromTo<From, ColorSpace::YCbCr709>(channel, file);
  if (to == "YCoCg")
    return ConvertFromTo<From, ColorSpace::YCoCg>(channel, file);
  if (to == "CMY") return ConvertFromTo<From, ColorSpace::CMY>(channel, file);
  LOG_DEBUG() << "not impl in to_channel";
  throw std::logic_error("not impl");
}

std::string ConvertFromTo(std::string_view from, std::string_view to,
                          std::string_view channel, std::string_view file) {
  if (from == "RGB") return ConvertFromTo<ColorSpace::RGB>(to, channel, file);
  if (from == "HSL") return ConvertFromTo<ColorSpace::HSL>(to, channel, file);
  if (from == "HSV") return ConvertFromTo<ColorSpace::HSV>(to, channel, file);
  if (from == "YCbCr601")
    return ConvertFromTo<ColorSpace::YCbCr601>(to, channel, file);
  if (from == "YCbCr709")
    return ConvertFromTo<ColorSpace::YCbCr709>(to, channel, file);
  if (from == "YCoCg")
    return ConvertFromTo<ColorSpace::YCoCg>(to, channel, file);
  if (from == "CMY") return ConvertFromTo<ColorSpace::CMY>(to, channel, file);
  LOG_DEBUG() << "not impl in from_to_channel";
  throw std::logic_error("not impl");
}

}  // namespace

void AppendConvert(userver::components::ComponentList& component_list) {
  component_list.Append<Convert>();
}

}  // namespace service_template
