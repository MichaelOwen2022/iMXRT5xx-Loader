# CROSS COMPILER SETTING
SET(CMAKE_SYSTEM_NAME Generic)
CMAKE_MINIMUM_REQUIRED (VERSION 3.10.0)

# THE VERSION NUMBER
SET (Tutorial_VERSION_MAJOR 1)
SET (Tutorial_VERSION_MINOR 0)

# ENABLE ASM
ENABLE_LANGUAGE(ASM)

SET(CMAKE_STATIC_LIBRARY_PREFIX)
SET(CMAKE_STATIC_LIBRARY_SUFFIX)

SET(CMAKE_EXECUTABLE_LIBRARY_PREFIX)
SET(CMAKE_EXECUTABLE_LIBRARY_SUFFIX)

# CURRENT DIRECTORY
SET(ProjDirPath ${CMAKE_CURRENT_SOURCE_DIR})

SET(EXECUTABLE_OUTPUT_PATH ${ProjDirPath}/${CMAKE_BUILD_TYPE})
SET(LIBRARY_OUTPUT_PATH ${ProjDirPath}/${CMAKE_BUILD_TYPE})


project(dsp_xaf_playback_cm33)

set(MCUX_SDK_PROJECT_NAME dsp_xaf_playback_cm33.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../main_cm33.c"
"${ProjDirPath}/../main_cm33.h"
"${ProjDirPath}/../cmd.c"
"${ProjDirPath}/../cmd.h"
"${ProjDirPath}/../dsp_ipc.c"
"${ProjDirPath}/../dsp_ipc.h"
"${ProjDirPath}/../ffconf.h"
"${ProjDirPath}/../dsp_config.h"
"${ProjDirPath}/../srtm_config.h"
"${ProjDirPath}/../srtm_config_audio.h"
"${ProjDirPath}/../srtm_config_nn.h"
"${ProjDirPath}/../rpmsg_config.h"
"${ProjDirPath}/../hihat_dec_out.pcm.h"
"${ProjDirPath}/../hihat.mp3.h"
"${ProjDirPath}/../hihat_48k.mp3.h"
"${ProjDirPath}/../hihat.ogg.h"
"${ProjDirPath}/../hihat_trim.pcm.h"
"${ProjDirPath}/../hihat_trim.sbc.h"
"${ProjDirPath}/../hihat_vorbis_dec_out_trim.pcm.h"
"${ProjDirPath}/../hihat_vor.h"
"${ProjDirPath}/../hihat_pcm.h"
"${ProjDirPath}/../sbc_test_02_trim.sbc.h"
"${ProjDirPath}/../sbc_test_02_trim.out.h"
"${ProjDirPath}/../testvector04-48000-2ch_trim.out.h"
"${ProjDirPath}/../testvector04.bit.h"
"${ProjDirPath}/../testvector11-16000-1ch_trim.out.h"
"${ProjDirPath}/../testvector11-16kHz-1ch-20kbps_trim.bit.h"
"${ProjDirPath}/../aac.adts.h"
"${ProjDirPath}/../aac.ref.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../incbin.S"
"${ProjDirPath}/../dsp_support.c"
"${ProjDirPath}/../dsp_support.h"
"${ProjDirPath}/../FreeRTOSConfig.h"
"${ProjDirPath}/../pmic_support.c"
"${ProjDirPath}/../pmic_support.h"
"${ProjDirPath}/../sdmmc_config.c"
"${ProjDirPath}/../sdmmc_config.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../../dsp/binary/dsp_reset_release.bin"
"${ProjDirPath}/../../dsp/binary/dsp_text_release.bin"
"${ProjDirPath}/../../dsp/binary/dsp_data_release.bin"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/..
    ${ProjDirPath}/../../dsp/binary
    ${ProjDirPath}/../../../..
)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../../devices/MIMXRT595S/drivers
    ${ProjDirPath}/../../../../../../middleware/multicore
    ${ProjDirPath}/../../../../../../rtos/freertos/freertos_kernel
    ${ProjDirPath}/../../../../../../devices/MIMXRT595S/utilities
    ${ProjDirPath}/../../../../../../middleware/sdmmc
    ${ProjDirPath}/../../../../../../middleware/fatfs
    ${ProjDirPath}/../../../../../../components/pca9420
    ${ProjDirPath}/../../../../../../components/codec/wm8904
    ${ProjDirPath}/../../../../../../components/codec
    ${ProjDirPath}/../../../../../../components/codec/i2c
    ${ProjDirPath}/../../../../../../components/i2c
    ${ProjDirPath}/../../../../../../components/gpio
    ${ProjDirPath}/../../../../../../devices/MIMXRT595S
    ${ProjDirPath}/../../../../../../components/uart
    ${ProjDirPath}/../../../../../../components/serial_manager
    ${ProjDirPath}/../../../../../../components/lists
    ${ProjDirPath}/../../../../flash_config
    ${ProjDirPath}/../../../../../../CMSIS/Core/Include
    ${ProjDirPath}/../../../../../../components/osa
)

# include modules
include(driver_clock_MIMXRT595S_cm33)

include(driver_flexcomm_i2c_MIMXRT595S_cm33)

include(middleware_multicore_rpmsg_lite_MIMXRT595S_cm33)

include(middleware_multicore_rpmsg_lite_freertos_MIMXRT595S_cm33)

include(middleware_freertos-kernel_cm33_nonsecure_port_MIMXRT595S_cm33)

include(utility_shell_MIMXRT595S_cm33)

include(driver_lpc_gpio_MIMXRT595S_cm33)

include(driver_usdhc_MIMXRT595S_cm33)

include(middleware_sdmmc_sd_MIMXRT595S_cm33)

include(middleware_sdmmc_common_MIMXRT595S_cm33)

include(middleware_sdmmc_host_usdhc_MIMXRT595S_cm33)

include(middleware_sdmmc_host_usdhc_freertos_MIMXRT595S_cm33)

include(middleware_fatfs_MIMXRT595S_cm33)

include(middleware_fatfs_sd_MIMXRT595S_cm33)

include(driver_common_MIMXRT595S_cm33)

include(driver_power_MIMXRT595S_cm33)

include(driver_reset_MIMXRT595S_cm33)

include(driver_dsp_MIMXRT595S_cm33)

include(driver_inputmux_MIMXRT595S_cm33)

include(middleware_multicore_rpmsg_lite_evkmimxrt595_freertos_MIMXRT595S_cm33)

include(driver_mu_MIMXRT595S_cm33)

include(driver_pca9420_MIMXRT595S_cm33)

include(driver_wm8904_MIMXRT595S_cm33)

include(driver_codec_MIMXRT595S_cm33)

include(component_wm8904_adapter_MIMXRT595S_cm33)

include(component_codec_i2c_MIMXRT595S_cm33)

include(component_i3c_adapter_MIMXRT595S_cm33)

include(driver_i3c_MIMXRT595S_cm33)

include(component_rt_gpio_adapter_MIMXRT595S_cm33)

include(device_MIMXRT595S_CMSIS_MIMXRT595S_cm33)

include(utility_debug_console_MIMXRT595S_cm33)

include(utility_assert_MIMXRT595S_cm33)

include(component_usart_adapter_MIMXRT595S_cm33)

include(driver_flexspi_MIMXRT595S_cm33)

include(driver_cache_cache64_MIMXRT595S_cm33)

include(component_serial_manager_MIMXRT595S_cm33)

include(component_lists_MIMXRT595S_cm33)

include(component_serial_manager_uart_MIMXRT595S_cm33)

include(driver_flexcomm_usart_MIMXRT595S_cm33)

include(driver_flash_config_evkmimxrt595_MIMXRT595S_cm33)

include(device_MIMXRT595S_startup_MIMXRT595S_cm33)

include(driver_flexcomm_MIMXRT595S_cm33)

include(driver_lpc_iopctl_MIMXRT595S_cm33)

include(CMSIS_Include_core_cm_MIMXRT595S_cm33)

include(middleware_freertos-kernel_MIMXRT595S_cm33)

include(middleware_freertos-kernel_extension_MIMXRT595S_cm33)

include(middleware_sdmmc_osa_freertos_MIMXRT595S_cm33)

include(component_osa_free_rtos_MIMXRT595S_cm33)

include(driver_inputmux_connections_MIMXRT595S_cm33)

include(middleware_freertos-kernel_heap_4_MIMXRT595S_cm33)

include(utilities_misc_utilities_MIMXRT595S_cm33)

include(driver_iap_MIMXRT595S_cm33)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)

ADD_CUSTOM_COMMAND(TARGET ${MCUX_SDK_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_OBJCOPY}
-Obinary ${EXECUTABLE_OUTPUT_PATH}/${MCUX_SDK_PROJECT_NAME} ${EXECUTABLE_OUTPUT_PATH}/sdk20-app.bin)

