#include "hello.hpp"
#include "userver/logging/log.hpp"
#include "../core/pnm.h"

#include <fmt/format.h>
#include <codecvt>
#include <locale>
#include <string>

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
//    auto raw_buffer = request.RequestBody();
//    LOG_ERROR()<< "----" << request.RequestBody() << "----";
//    std::string raw_buffer1{request.RequestBody().begin(), request.RequestBody().begin() + request.RequestBody().size()};
//    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
//    auto ss = converter.from_bytes(raw_buffer1);
//    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter1;
//    std::wstring start_str = converter1.from_bytes(std::string("form-data; name=\"file\"\\r\\n\\r\\n"));
//    int start = ss.find(start_str) + start_str.size();
//    std::wstring finish_str = ;converter1.from_bytes(std::string("\\r\\n--dart-http-boundary"));
//    int finish = raw_buffer.find(finish_str);
//    LOG_ERROR() << start << " " << finish;
//    int size = raw_buffer.size();
//    raw_buffer = raw_buffer.substr(start, finish - start);
//    LOG_ERROR() << raw_buffer;
//    LOG_ERROR() << "sssssssssssss" << raw_buffer.size();
//    bytes buffer1{raw_buffer.begin(), raw_buffer.begin() + raw_buffer.size()};
//    bytes buffer2{buffer1};
//    auto& response = request.GetHttpResponse();
//    std::string s = "bad";
//    try {
//      PNM<PnmType::P5> p5(std::move(buffer1));
//      response.SetStatus(userver::server::http::HttpStatus::kOk);
//      LOG_DEBUG() << "it's pnm - P5";
//      s = "P5";
//    }
//    catch (std::exception &e) {
//      try {
//        PNM<PnmType::P6> p6(std::move(buffer2));
//        response.SetStatus(userver::server::http::HttpStatus::kOk);
//        LOG_DEBUG() << "it's pnm - P6";
//        s = "P6";
//      }
//      catch (std::exception &e) {
//        response.SetStatus(userver::server::http::HttpStatus::kBadRequest);
//        LOG_DEBUG() << "not pnm";
//      };
//    }
//    response.SetStatus(userver::server::http::HttpStatus::kBadRequest);

    return {};
  }
};

}  // namespace

void AppendHello(userver::components::ComponentList& component_list) {
  component_list.Append<Hello>();
}

}  // namespace service_template
