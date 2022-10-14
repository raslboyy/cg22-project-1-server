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
    try {
      PNM<PnmType::P5> p5(std::move(buffer1));
      response.SetStatus(userver::server::http::HttpStatus::kOk);
    }
    catch (std::exception &e) {
      LOG_DEBUG() << "not p5";
    }
    try {
      PNM<PnmType::P6> p6(std::move(buffer2));
      response.SetStatus(userver::server::http::HttpStatus::kOk);
    }
    catch (std::exception &e) {
      LOG_DEBUG() << "not p6";
      response.SetStatus(userver::server::http::HttpStatus::kBadRequest);
    };
    return {};
  }
};

}  // namespace

void AppendHello(userver::components::ComponentList& component_list) {
  component_list.Append<Hello>();
}

}  // namespace service_template
