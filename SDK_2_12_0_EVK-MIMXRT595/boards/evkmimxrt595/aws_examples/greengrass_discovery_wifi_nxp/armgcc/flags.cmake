SET(CMAKE_ASM_FLAGS_FLASH_DEBUG " \
    ${CMAKE_ASM_FLAGS_FLASH_DEBUG} \
    -DDEBUG \
    -D__STARTUP_CLEAR_BSS \
    -mcpu=cortex-m33 \
    -mfloat-abi=hard \
    -mfpu=fpv5-sp-d16 \
    -mthumb \
")
SET(CMAKE_ASM_FLAGS_FLASH_RELEASE " \
    ${CMAKE_ASM_FLAGS_FLASH_RELEASE} \
    -DNDEBUG \
    -D__STARTUP_CLEAR_BSS \
    -mcpu=cortex-m33 \
    -mfloat-abi=hard \
    -mfpu=fpv5-sp-d16 \
    -mthumb \
")
SET(CMAKE_C_FLAGS_FLASH_DEBUG " \
    ${CMAKE_C_FLAGS_FLASH_DEBUG} \
    -include ${ProjDirPath}/../app_config.h \
    -DDEBUG \
    -DFSL_SDK_DRIVER_QUICK_ACCESS_ENABLE=1 \
    -DLWIP_TIMEVAL_PRIVATE=0 \
    -DCPU_MIMXRT595SFFOC_cm33 \
    -DBOOT_HEADER_ENABLE=1 \
    -DUSE_RTOS=1 \
    -DggdconfigTCP_RECEIVE_TIMEOUT_MS=15000 \
    -DXIP_IMAGE \
    -DXIP_EXTERNAL_FLASH \
    -DSDIO_ENABLED \
    -DSDK_OS_FREE_RTOS \
    -DMFLASH_FILE_BASEADDR=7340032 \
    -DSDK_I2C_BASED_COMPONENT_USED=1 \
    -DSERIAL_PORT_TYPE_UART=1 \
    -DMCUXPRESSO_SDK \
    -g \
    -O0 \
    -fomit-frame-pointer \
    -Wno-unused-function \
    -mcpu=cortex-m33 \
    -Wall \
    -mfloat-abi=hard \
    -mfpu=fpv5-sp-d16 \
    -mthumb \
    -MMD \
    -MP \
    -fno-common \
    -ffunction-sections \
    -fdata-sections \
    -ffreestanding \
    -fno-builtin \
    -mapcs \
    -std=gnu99 \
")
SET(CMAKE_C_FLAGS_FLASH_RELEASE " \
    ${CMAKE_C_FLAGS_FLASH_RELEASE} \
    -include ${ProjDirPath}/../app_config.h \
    -DNDEBUG \
    -DFSL_SDK_DRIVER_QUICK_ACCESS_ENABLE=1 \
    -DLWIP_TIMEVAL_PRIVATE=0 \
    -DCPU_MIMXRT595SFFOC_cm33 \
    -DBOOT_HEADER_ENABLE=1 \
    -DUSE_RTOS=1 \
    -DggdconfigTCP_RECEIVE_TIMEOUT_MS=15000 \
    -DXIP_IMAGE \
    -DXIP_EXTERNAL_FLASH \
    -DSDIO_ENABLED \
    -DSDK_OS_FREE_RTOS \
    -DMFLASH_FILE_BASEADDR=7340032 \
    -DSDK_I2C_BASED_COMPONENT_USED=1 \
    -DSERIAL_PORT_TYPE_UART=1 \
    -DMCUXPRESSO_SDK \
    -Os \
    -fomit-frame-pointer \
    -Wno-unused-function \
    -mcpu=cortex-m33 \
    -Wall \
    -mfloat-abi=hard \
    -mfpu=fpv5-sp-d16 \
    -mthumb \
    -MMD \
    -MP \
    -fno-common \
    -ffunction-sections \
    -fdata-sections \
    -ffreestanding \
    -fno-builtin \
    -mapcs \
    -std=gnu99 \
")
SET(CMAKE_CXX_FLAGS_FLASH_DEBUG " \
    ${CMAKE_CXX_FLAGS_FLASH_DEBUG} \
    -DDEBUG \
    -DSERIAL_PORT_TYPE_UART=1 \
    -DMCUXPRESSO_SDK \
    -g \
    -O0 \
    -mcpu=cortex-m33 \
    -Wall \
    -mfloat-abi=hard \
    -mfpu=fpv5-sp-d16 \
    -mthumb \
    -MMD \
    -MP \
    -fno-common \
    -ffunction-sections \
    -fdata-sections \
    -ffreestanding \
    -fno-builtin \
    -mapcs \
    -fno-rtti \
    -fno-exceptions \
")
SET(CMAKE_CXX_FLAGS_FLASH_RELEASE " \
    ${CMAKE_CXX_FLAGS_FLASH_RELEASE} \
    -DNDEBUG \
    -DSERIAL_PORT_TYPE_UART=1 \
    -DMCUXPRESSO_SDK \
    -Os \
    -mcpu=cortex-m33 \
    -Wall \
    -mfloat-abi=hard \
    -mfpu=fpv5-sp-d16 \
    -mthumb \
    -MMD \
    -MP \
    -fno-common \
    -ffunction-sections \
    -fdata-sections \
    -ffreestanding \
    -fno-builtin \
    -mapcs \
    -fno-rtti \
    -fno-exceptions \
")
SET(CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG " \
    ${CMAKE_EXE_LINKER_FLAGS_FLASH_DEBUG} \
    -g \
    -mcpu=cortex-m33 \
    -Wall \
    -mfloat-abi=hard \
    -mfpu=fpv5-sp-d16 \
    --specs=nosys.specs \
    -fno-common \
    -ffunction-sections \
    -fdata-sections \
    -ffreestanding \
    -fno-builtin \
    -mthumb \
    -mapcs \
    -Xlinker \
    --gc-sections \
    -Xlinker \
    -static \
    -Xlinker \
    -z \
    -Xlinker \
    muldefs \
    -Xlinker \
    -Map=output.map \
    -Wl,--print-memory-usage \
    -Xlinker \
    --defsym=__stack_size__=0x400 \
    -Xlinker \
    --defsym=__heap_size__=0x200 \
    -T${ProjDirPath}/linker/MIMXRT595Sxxxx_cm33_flash.ld -static \
")
SET(CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE " \
    ${CMAKE_EXE_LINKER_FLAGS_FLASH_RELEASE} \
    -mcpu=cortex-m33 \
    -Wall \
    -mfloat-abi=hard \
    -mfpu=fpv5-sp-d16 \
    --specs=nosys.specs \
    -fno-common \
    -ffunction-sections \
    -fdata-sections \
    -ffreestanding \
    -fno-builtin \
    -mthumb \
    -mapcs \
    -Xlinker \
    --gc-sections \
    -Xlinker \
    -static \
    -Xlinker \
    -z \
    -Xlinker \
    muldefs \
    -Xlinker \
    -Map=output.map \
    -Wl,--print-memory-usage \
    -Xlinker \
    --defsym=__stack_size__=0x400 \
    -Xlinker \
    --defsym=__heap_size__=0x200 \
    -T${ProjDirPath}/linker/MIMXRT595Sxxxx_cm33_flash.ld -static \
")