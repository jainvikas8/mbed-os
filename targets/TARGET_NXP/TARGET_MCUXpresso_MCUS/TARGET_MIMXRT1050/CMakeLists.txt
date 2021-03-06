# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if("EVK" IN_LIST MBED_TARGET_LABELS)
    target_include_directories(mbed-core
        INTERFACE
            TARGET_EVK
            TARGET_EVK/xip
    )

    target_sources(mbed-core
        INTERFACE
            TARGET_EVK/PeripheralPins.c
            TARGET_EVK/fsl_clock_config.c
            TARGET_EVK/fsl_phy.c
            TARGET_EVK/lpm.c
            TARGET_EVK/mbed_overrides.c
            TARGET_EVK/specific.c

            TARGET_EVK/xip/evkbimxrt1050_flexspi_nor_config.c
            TARGET_EVK/xip/evkbimxrt1050_sdram_ini_dcd.c
    )
endif()

if(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE device/TOOLCHAIN_ARM_STD/startup_MIMXRT1052.S)
    set(LINKER_FILE device/TOOLCHAIN_ARM_STD/MIMXRT1052xxxxx.sct)
elseif(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE device/TOOLCHAIN_GCC_ARM/startup_MIMXRT1052.S)
    set(LINKER_FILE device/TOOLCHAIN_GCC_ARM/MIMXRT1052xxxxx.ld)
endif()

target_include_directories(mbed-core
    INTERFACE
        .
        device
        drivers
)

target_sources(mbed-core
    INTERFACE
        usb_device_ch9.c
        usb_device_class.c

        device/system_MIMXRT1052.c

        drivers/fsl_adc.c
        drivers/fsl_adc_etc.c
        drivers/fsl_aipstz.c
        drivers/fsl_aoi.c
        drivers/fsl_bee.c
        drivers/fsl_cache.c
        drivers/fsl_clock.c
        drivers/fsl_cmp.c
        drivers/fsl_common.c
        drivers/fsl_csi.c
        drivers/fsl_dcdc.c
        drivers/fsl_dcp.c
        drivers/fsl_dmamux.c
        drivers/fsl_edma.c
        drivers/fsl_elcdif.c
        drivers/fsl_enc.c
        drivers/fsl_enet.c
        drivers/fsl_ewm.c
        drivers/fsl_flexcan.c
        drivers/fsl_flexio.c
        drivers/fsl_flexio_camera.c
        drivers/fsl_flexio_camera_edma.c
        drivers/fsl_flexio_i2c_master.c
        drivers/fsl_flexio_i2s.c
        drivers/fsl_flexio_i2s_edma.c
        drivers/fsl_flexio_mculcd.c
        drivers/fsl_flexio_mculcd_edma.c
        drivers/fsl_flexio_spi.c
        drivers/fsl_flexio_spi_edma.c
        drivers/fsl_flexio_uart.c
        drivers/fsl_flexio_uart_edma.c
        drivers/fsl_flexram.c
        drivers/fsl_flexspi.c
        drivers/fsl_flexspi_nor_boot.c
        drivers/fsl_gpc.c
        drivers/fsl_gpio.c
        drivers/fsl_gpt.c
        drivers/fsl_kpp.c
        drivers/fsl_lpi2c.c
        drivers/fsl_lpi2c_edma.c
        drivers/fsl_lpspi.c
        drivers/fsl_lpspi_edma.c
        drivers/fsl_lpuart.c
        drivers/fsl_lpuart_edma.c
        drivers/fsl_ocotp.c
        drivers/fsl_pit.c
        drivers/fsl_pmu.c
        drivers/fsl_pwm.c
        drivers/fsl_pxp.c
        drivers/fsl_qtmr.c
        drivers/fsl_rtwdog.c
        drivers/fsl_sai.c
        drivers/fsl_sai_edma.c
        drivers/fsl_semc.c
        drivers/fsl_snvs_hp.c
        drivers/fsl_snvs_lp.c
        drivers/fsl_spdif.c
        drivers/fsl_spdif_edma.c
        drivers/fsl_src.c
        drivers/fsl_tempmon.c
        drivers/fsl_trng.c
        drivers/fsl_tsc.c
        drivers/fsl_usdhc.c
        drivers/fsl_wdog.c
        drivers/fsl_xbara.c
        drivers/fsl_xbarb.c
)
