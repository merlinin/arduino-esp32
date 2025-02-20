/* Class of Zigbee Metering sensor endpoint inherited from common EP class */

#pragma once

#include "soc/soc_caps.h"
#include "sdkconfig.h"
#if SOC_IEEE802154_SUPPORTED && CONFIG_ZB_ENABLED

#include "ZigbeeEP.h"
#include "ha/esp_zigbee_ha_standard.h"

// clang-format off
#define ZIGBEE_DEFAULT_METERING_CONFIG()                               \
  {                                                                   \
    .basic_cfg =                                                      \
      {                                                               \
        .zcl_version = ESP_ZB_ZCL_BASIC_ZCL_VERSION_DEFAULT_VALUE,    \
        .power_source = ESP_ZB_ZCL_BASIC_POWER_SOURCE_DEFAULT_VALUE,  \
      },                                                              \
    .identify_cfg =                                                   \
      {                                                               \
        .identify_time = ESP_ZB_ZCL_IDENTIFY_IDENTIFY_TIME_DEFAULT_VALUE, \
      },                                                              \
    .metering_cfg =                                                   \
      {                                                               \
        .current_summation = 0,                                       \
        .instantaneous_demand = 0,                                    \
      },                                                              \
  }
// clang-format on

typedef struct zigbee_metering_cfg_s {
  esp_zb_basic_cluster_cfg_t basic_cfg;
  esp_zb_identify_cluster_cfg_t identify_cfg;
  esp_zb_metering_cluster_cfg_t metering_cfg;
} zigbee_metering_cfg_t;

class ZigbeeMetering : public ZigbeeEP {
public:
  ZigbeeMetering(uint8_t endpoint);
  ~ZigbeeMetering() {}

  // Set the current energy consumption (Wh)
  void setEnergy(uint32_t value);

  // Set the power demand (W)
  void setPowerDemand(uint32_t value);

  // Report the metering data
  void report();
};

#endif  //SOC_IEEE802154_SUPPORTED && CONFIG_ZB_ENABLED
