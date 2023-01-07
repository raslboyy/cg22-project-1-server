#include "handlers/storage_component.h"

namespace service_template {

StorageServiceComponent::StorageServiceComponent(
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& context)
    : LoggableComponentBase(config, context) {}

}  // namespace service_template
