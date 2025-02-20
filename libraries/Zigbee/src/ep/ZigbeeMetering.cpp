/* Implementation of Zigbee Metering sensor endpoint */

#include "ZigbeeMetering.h"
#if SOC_IEEE802154_SUPPORTED && CONFIG_ZB_ENABLED

esp_zb_cluster_list_t *zigbee_metering_clusters_create(zigbee_metering_cfg_t *metering) {
  esp_zb_basic_cluster_cfg_t *basic_cfg = metering ? &(metering->basic_cfg) : NULL;
  esp_zb_identify_cluster_cfg_t *identify_cfg = metering ? &(metering->identify_cfg) : NULL;
  esp_zb_metering_cluster_cfg_t *metering_cfg = metering ? &(metering->metering_cfg) : NULL;
  
  esp_zb_cluster_list_t *cluster_list = esp_zb_zcl_cluster_list_create();
  esp_zb_cluster_list_add_basic_cluster(cluster_list, esp_zb_basic_cluster_create(basic_cfg), ESP_ZB_ZCL_CLUSTER_SERVER_ROLE);
  esp_zb_cluster_list_add_identify_cluster(cluster_list, esp_zb_identify_cluster_create(identify_cfg), ESP_ZB_ZCL_CLUSTER_SERVER_ROLE);
  esp_zb_cluster_list_add_metering_cluster(cluster_list, esp_zb_metering_cluster_create(metering_cfg), ESP_ZB_ZCL_CLUSTER_SERVER_ROLE);
  return cluster_list;
}

ZigbeeMetering::ZigbeeMetering(uint8_t endpoint) : ZigbeeEP(endpoint) {
  _device_id = ESP_ZB_HA_SIMPLE_METERING_DEVICE_ID;
  
  zigbee_metering_cfg_t metering_cfg = ZIGBEE_DEFAULT_METERING_CONFIG();
  _cluster_list = zigbee_metering_clusters_create(&metering_cfg);
  
  _ep_config = {.endpoint = _endpoint, .app_profile_id = ESP_ZB_AF_HA_PROFILE_ID, .app_device_id = ESP_ZB_HA_SIMPLE_METERING_DEVICE_ID, .app_device_version = 0};
}

void ZigbeeMetering::setEnergy(uint32_t value) {
  esp_zb_attribute_list_t *metering_cluster =
    esp_zb_cluster_list_get_cluster(_cluster_list, ESP_ZB_ZCL_CLUSTER_ID_METERING, ESP_ZB_ZCL_CLUSTER_SERVER_ROLE);
  esp_zb_cluster_update_attr(metering_cluster, ESP_ZB_ZCL_ATTR_METERING_CURRENT_SUMMATION_ID, (void *)&value);
}

void ZigbeeMetering::setPowerDemand(uint32_t value) {
  esp_zb_attribute_list_t *metering_cluster =
    esp_zb_cluster_list_get_cluster(_cluster_list, ESP_ZB_ZCL_CLUSTER_ID_METERING, ESP_ZB_ZCL_CLUSTER_SERVER_ROLE);
  esp_zb_cluster_update_attr(metering_cluster, ESP_ZB_ZCL_ATTR_METERING_INSTANTANEOUS_DEMAND_ID, (void *)&value);
}

void ZigbeeMetering::report() {
  esp_zb_zcl_report_attr_cmd_t report_attr_cmd;
  report_attr_cmd.address_mode = ESP_ZB_APS_ADDR_MODE_DST_ADDR_ENDP_NOT_PRESENT;
  report_attr_cmd.attributeID = ESP_ZB_ZCL_ATTR_METERING_CURRENT_SUMMATION_ID;
  report_attr_cmd.direction = ESP_ZB_ZCL_CMD_DIRECTION_TO_CLI;
  report_attr_cmd.clusterID = ESP_ZB_ZCL_CLUSTER_ID_METERING;
  report_attr_cmd.zcl_basic_cmd.src_endpoint = _endpoint;
  
  esp_zb_lock_acquire(portMAX_DELAY);
  esp_zb_zcl_report_attr_cmd_req(&report_attr_cmd);
  esp_zb_lock_release();
}

#endif  //SOC_IEEE802154_SUPPORTED && CONFIG_ZB_ENABLED
