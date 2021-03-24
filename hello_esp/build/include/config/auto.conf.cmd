deps_config := \
	/Projects/CO_ESP/esp-idf/components/app_trace/Kconfig \
	/Projects/CO_ESP/esp-idf/components/aws_iot/Kconfig \
	/Projects/CO_ESP/esp-idf/components/bt/Kconfig \
	/Projects/CO_ESP/esp-idf/components/driver/Kconfig \
	/Projects/CO_ESP/esp-idf/components/esp32/Kconfig \
	/Projects/CO_ESP/esp-idf/components/esp_adc_cal/Kconfig \
	/Projects/CO_ESP/esp-idf/components/esp_event/Kconfig \
	/Projects/CO_ESP/esp-idf/components/esp_http_client/Kconfig \
	/Projects/CO_ESP/esp-idf/components/esp_http_server/Kconfig \
	/Projects/CO_ESP/esp-idf/components/ethernet/Kconfig \
	/Projects/CO_ESP/esp-idf/components/fatfs/Kconfig \
	/Projects/CO_ESP/esp-idf/components/freemodbus/Kconfig \
	/Projects/CO_ESP/esp-idf/components/freertos/Kconfig \
	/Projects/CO_ESP/esp-idf/components/heap/Kconfig \
	/Projects/CO_ESP/esp-idf/components/libsodium/Kconfig \
	/Projects/CO_ESP/esp-idf/components/log/Kconfig \
	/Projects/CO_ESP/esp-idf/components/lwip/Kconfig \
	/Projects/CO_ESP/esp-idf/components/mbedtls/Kconfig \
	/Projects/CO_ESP/esp-idf/components/mdns/Kconfig \
	/Projects/CO_ESP/esp-idf/components/mqtt/Kconfig \
	/Projects/CO_ESP/esp-idf/components/nvs_flash/Kconfig \
	/Projects/CO_ESP/esp-idf/components/openssl/Kconfig \
	/Projects/CO_ESP/esp-idf/components/pthread/Kconfig \
	/Projects/CO_ESP/esp-idf/components/spi_flash/Kconfig \
	/Projects/CO_ESP/esp-idf/components/spiffs/Kconfig \
	/Projects/CO_ESP/esp-idf/components/tcpip_adapter/Kconfig \
	/Projects/CO_ESP/esp-idf/components/vfs/Kconfig \
	/Projects/CO_ESP/esp-idf/components/wear_levelling/Kconfig \
	/Projects/CO_ESP/esp-idf/components/bootloader/Kconfig.projbuild \
	/Projects/CO_ESP/esp-idf/hello_esp/components/drivers/Kconfig.projbuild \
	/Projects/CO_ESP/esp-idf/components/esptool_py/Kconfig.projbuild \
	/Projects/CO_ESP/esp-idf/components/partition_table/Kconfig.projbuild \
	/Projects/CO_ESP/esp-idf/Kconfig

include/config/auto.conf: \
	$(deps_config)

ifneq "$(IDF_CMAKE)" "n"
include/config/auto.conf: FORCE
endif

$(deps_config): ;
