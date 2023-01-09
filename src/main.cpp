#include <userver/clients/http/component.hpp>
#include <userver/components/minimal_server_component_list.hpp>
#include <userver/server/handlers/ping.hpp>
#include <userver/server/handlers/tests_control.hpp>
#include <userver/testsuite/testsuite_support.hpp>
#include <userver/utils/daemon_run.hpp>

#include "handlers/convert.h"
#include "handlers/dithering.h"
#include "handlers/draw_line.h"
#include "handlers/scaling.h"
#include "handlers/storage_component.h"
#include "handlers/upload.h"

int main(int argc, char* argv[]) {
  auto component_list =
      userver::components::MinimalServerComponentList()
          .Append<userver::server::handlers::Ping>()
          .Append<userver::components::TestsuiteSupport>()
          .Append<userver::components::HttpClient>()
          .Append<userver::server::handlers::TestsControl>()
          .Append<service_template::StorageServiceComponent>();

  service_template::AppendUpload(component_list);
  service_template::AppendConvert(component_list);
  service_template::AppendDrawLine(component_list);
  service_template::AppendDithering(component_list);
  service_template::AppendScaling(component_list);

  return userver::utils::DaemonMain(argc, argv, component_list);
}
