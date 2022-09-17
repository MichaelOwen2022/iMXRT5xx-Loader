include_guard()
message("middleware_freertos-aws_iot_demos_greengrass_discovery component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/demos/greengrass_connectivity/aws_greengrass_discovery_demo.c
)


include(middleware_freertos-aws_iot_libraries_abstractions_backoff_algorithm_MIMXRT595S_cm33)

include(middleware_freertos-aws_iot_libraries_abstractions_platform_MIMXRT595S_cm33)

include(middleware_freertos-aws_iot_libraries_abstractions_transport_MIMXRT595S_cm33)

include(middleware_freertos-aws_iot_libraries_c_sdk_standard_common_MIMXRT595S_cm33)

include(middleware_freertos-aws_iot_libraries_coremqtt_MIMXRT595S_cm33)

include(middleware_freertos-aws_iot_libraries_freertos_plus_aws_greengrass_MIMXRT595S_cm33)

include(middleware_freertos-aws_iot_pkcs11_helpers_MIMXRT595S_cm33)

include(middleware_freertos-kernel_MIMXRT595S_cm33)

