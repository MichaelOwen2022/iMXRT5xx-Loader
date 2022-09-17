include_guard()
message("driver_codec component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/fsl_codec_common.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/.
)


#OR Logic component
if(CONFIG_USE_component_wm8904_adapter_MIMXRT595S_cm33)
     include(component_wm8904_adapter_MIMXRT595S_cm33)
endif()
if(CONFIG_USE_component_wm8960_adapter_MIMXRT595S_cm33)
     include(component_wm8960_adapter_MIMXRT595S_cm33)
endif()
if(CONFIG_USE_component_wm8524_adapter_MIMXRT595S_cm33)
     include(component_wm8524_adapter_MIMXRT595S_cm33)
endif()
if(CONFIG_USE_component_sgtl_adapter_MIMXRT595S_cm33)
     include(component_sgtl_adapter_MIMXRT595S_cm33)
endif()
if(CONFIG_USE_component_da7212_adapter_MIMXRT595S_cm33)
     include(component_da7212_adapter_MIMXRT595S_cm33)
endif()
if(CONFIG_USE_component_ak4497_adapter_MIMXRT595S_cm33)
     include(component_ak4497_adapter_MIMXRT595S_cm33)
endif()
if(CONFIG_USE_component_tfa9xxx_adapter_MIMXRT595S_cm33)
     include(component_tfa9xxx_adapter_MIMXRT595S_cm33)
endif()
if(CONFIG_USE_component_tfa9896_adapter_MIMXRT595S_cm33)
     include(component_tfa9896_adapter_MIMXRT595S_cm33)
endif()
if(CONFIG_USE_component_cs42888_adapter_MIMXRT595S_cm33)
     include(component_cs42888_adapter_MIMXRT595S_cm33)
endif()
if(CONFIG_USE_component_cs42448_adapter_MIMXRT595S_cm33)
     include(component_cs42448_adapter_MIMXRT595S_cm33)
endif()
if(CONFIG_USE_component_codec_adapters_MIMXRT595S_cm33)
     include(component_codec_adapters_MIMXRT595S_cm33)
endif()
if(NOT (CONFIG_USE_component_wm8904_adapter_MIMXRT595S_cm33 OR CONFIG_USE_component_wm8960_adapter_MIMXRT595S_cm33 OR CONFIG_USE_component_wm8524_adapter_MIMXRT595S_cm33 OR CONFIG_USE_component_sgtl_adapter_MIMXRT595S_cm33 OR CONFIG_USE_component_da7212_adapter_MIMXRT595S_cm33 OR CONFIG_USE_component_ak4497_adapter_MIMXRT595S_cm33 OR CONFIG_USE_component_tfa9xxx_adapter_MIMXRT595S_cm33 OR CONFIG_USE_component_tfa9896_adapter_MIMXRT595S_cm33 OR CONFIG_USE_component_cs42888_adapter_MIMXRT595S_cm33 OR CONFIG_USE_component_cs42448_adapter_MIMXRT595S_cm33 OR CONFIG_USE_component_codec_adapters_MIMXRT595S_cm33))
    message(WARNING "Since component_wm8904_adapter_MIMXRT595S_cm33/component_wm8960_adapter_MIMXRT595S_cm33/component_wm8524_adapter_MIMXRT595S_cm33/component_sgtl_adapter_MIMXRT595S_cm33/component_da7212_adapter_MIMXRT595S_cm33/component_ak4497_adapter_MIMXRT595S_cm33/component_tfa9xxx_adapter_MIMXRT595S_cm33/component_tfa9896_adapter_MIMXRT595S_cm33/component_cs42888_adapter_MIMXRT595S_cm33/component_cs42448_adapter_MIMXRT595S_cm33/component_codec_adapters_MIMXRT595S_cm33 is not included at first or config in config.cmake file, use component_wm8904_adapter_MIMXRT595S_cm33 by default.")
    include(component_wm8904_adapter_MIMXRT595S_cm33)
endif()

include(driver_common_MIMXRT595S_cm33)

