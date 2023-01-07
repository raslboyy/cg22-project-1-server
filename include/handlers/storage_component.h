#ifndef SERVICE_TEMPLATE_STORAGE_COMPONENT_H
#define SERVICE_TEMPLATE_STORAGE_COMPONENT_H

#include "core/pnm.h"

#include <unordered_map>

#include <userver/components/loggable_component_base.hpp>

namespace service_template {

using server::core::pnm::color_space::ColorSpace;
template <ColorSpace Space>
using PNM = server::core::pnm::PNM<Space>;

class StorageServiceComponent final
    : public userver::components::LoggableComponentBase {
 public:
  static constexpr std::string_view kName = "storage-service";

  StorageServiceComponent(const userver::components::ComponentConfig& config,
                          const userver::components::ComponentContext& context);

  template <ColorSpace Space>
  uint32_t Add(PNM<Space>&& pnm) {
    return GetStorage<Space>().Add(std::move(pnm));
  }

  template <ColorSpace Space>
  PNM<Space>& Get(uint32_t key) {
    return GetStorage<Space>().Get(key);
  }

 private:
  template <ColorSpace Space>
  struct Storage {
   public:
    uint32_t Add(PNM<Space>&& pnm) {
      auto key = counter_++;
      storage_.template emplace(key, std::move(pnm));
      return key;
    }
    PNM<Space>& Get(uint32_t key) { return storage_[key]; }

   private:
    std::unordered_map<uint32_t, PNM<Space>> storage_{};
    uint32_t counter_{};
  };

  template <ColorSpace Space>
  Storage<Space>& GetStorage() {
    static auto storage = Storage<Space>();
    return storage;
  }
};
}  // namespace service_template

#endif  // SERVICE_TEMPLATE_STORAGE_COMPONENT_H
