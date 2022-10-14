#include "hello.hpp"
#include "userver/logging/log.hpp"
#include "../core/pnm.h"

#include <fmt/format.h>

#include <userver/server/handlers/http_handler_base.hpp>

namespace service_template {

namespace {

class Hello final : public userver::server::handlers::HttpHandlerBase {
 public:
  static constexpr std::string_view kName = "handler-hello";

  using HttpHandlerBase::HttpHandlerBase;

  std::string HandleRequestThrow(
      const userver::server::http::HttpRequest& request,
      userver::server::request::RequestContext&) const override {
    auto raw_buffer = request.RequestBody().c_str();
    auto size = request.RequestBody().size();
    bytes buffer1{raw_buffer, raw_buffer + size};
    bytes buffer2{buffer1};
    auto& response = request.GetHttpResponse();
    std::string s = "bad";
    try {
      PNM<PnmType::P5> p5(std::move(buffer1));
      response.SetStatus(userver::server::http::HttpStatus::kOk);
      LOG_DEBUG() << "it's pnm - P5";
      s = "P5";
    }
    catch (std::exception &e) {
      try {
        PNM<PnmType::P6> p6(std::move(buffer2));
        response.SetStatus(userver::server::http::HttpStatus::kOk);
        LOG_DEBUG() << "it's pnm - P6";
        s = "P6";
      }
      catch (std::exception &e) {
        LOG_DEBUG() << "not pnm";
        response.SetStatus(userver::server::http::HttpStatus::kBadRequest);
      };
    }

    return s;
  }
};

}  // namespace

void AppendHello(userver::components::ComponentList& component_list) {
  component_list.Append<Hello>();
}

}  // namespace service_template
