include_guard()
message("middleware_freertos-kernel_heap_3 component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/portable/MemMang/heap_3.c
)


include(middleware_freertos-kernel_MIMXRT595S_cm33)

