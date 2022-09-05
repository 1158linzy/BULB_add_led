# Generated by Kconfiglib (https://github.com/ulfalizer/Kconfiglib)

#
# Target Chip
#
# CONFIG_TARGET_CHIP_HI3861 is not set
CONFIG_TARGET_CHIP_HI3861L=y
# end of Target Chip

#
# Security Settings
#
# CONFIG_TARGET_SIG_RSA_V15 is not set
# CONFIG_TARGET_SIG_RSA_PSS is not set
# CONFIG_TARGET_SIG_ECC is not set
CONFIG_TARGET_SIG_SHA256=y
# CONFIG_BOOT_ENCRYPT is not set
CONFIG_TARGET_KERNEL_VER=0
CONFIG_TARGET_BOOT_VER=0
# CONFIG_TEE_HUKS_SUPPORT is not set
# CONFIG_TEE_HUKS_DEMO_SUPPORT is not set
# CONFIG_FLASH_ENCRYPT_SUPPORT is not set
# end of Security Settings

#
# Factory Test Settings
#
# CONFIG_FACTORY_TEST_SUPPORT is not set
# end of Factory Test Settings

#
# BSP Settings
#
# CONFIG_I2C_SUPPORT is not set
# CONFIG_I2S_SUPPORT is not set
# CONFIG_SPI_SUPPORT is not set
# CONFIG_DMA_SUPPORT is not set
# CONFIG_SDIO_SUPPORT is not set
# CONFIG_SPI_DMA_SUPPORT is not set
# CONFIG_UART_DMA_SUPPORT is not set
# CONFIG_PWM_SUPPORT is not set
# CONFIG_PWM_HOLD_AFTER_REBOOT is not set
CONFIG_AT_SUPPORT=y
# CONFIG_FILE_SYSTEM_SUPPORT is not set
CONFIG_UART0_SUPPORT=y
CONFIG_UART1_SUPPORT=y
# CONFIG_UART2_SUPPORT is not set
# end of BSP Settings

#
# WiFi Settings
#
# CONFIG_WPS_SUPPORT is not set
CONFIG_CE_SUPPORT=y
# CONFIG_FCC_SUPPORT is not set
# CONFIG_MAX_POWER_SUPPORT is not set
# CONFIG_MESH_SUPPORT is not set
# CONFIG_WPA3_SUPPORT is not set
# CONFIG_QUICK_SEND_MODE is not set
# CONFIG_MGMT_FRAME_SIZE_1500 is not set
# end of WiFi Settings

#
# Third Party library
#
# CONFIG_CJSON is not set
# CONFIG_CJSON_NESTING_LIMIT=1000
# CONFIG_LIBCOAP is not set
# CONFIG_MQTT is not set
CONFIG_IPERF=y
# end of Third Party library

#
# Lwip Settings
#
CONFIG_DHCPS_GW=y
# CONFIG_NETIF_HOSTNAME is not set
# CONFIG_DHCP_VENDOR_CLASS_IDENTIFIER is not set
# CONFIG_LWIP_LOWPOWER is not set
# end of Lwip Settings

#
# OTA Settings
#
CONFIG_COMPRESSION_OTA_SUPPORT=y
# CONFIG_DUAL_PARTITION_OTA_SUPPORT is not set
# end of OTA Settings

#
# Link Settings
#
# CONFIG_HILINK is not set
# end of Link Settings

#
# Debug Log Settings
#
# CONFIG_DIAG_SUPPORT is not set
# end of Debug Log Settings
