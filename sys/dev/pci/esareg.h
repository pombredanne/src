/* $OpenBSD: esareg.h,v 1.1.4.1 2002/06/11 03:42:24 art Exp $ */
/* $NetBSD: esareg.h,v 1.8 2002/03/06 18:30:31 jmcneill Exp $ */

/*
 * Copyright (c) 2002 Lennart Augustsson
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/*
 * ESS Allegro-1 / Maestro3 Audio Driver
 * 
 * Lots of magic based on the FreeBSD maestro3 driver and
 * reverse engineering.
 * Original driver by Don Kim.
 *
 */

/* Allegro PCI configuration registers */
#define PCI_LEGACY_AUDIO_CTRL   0x40
#define DISABLE_LEGACY          0x00008000

#define ESA_PCI_ALLEGRO_CONFIG      0x50
#define ESA_SB_ADDR_240             0x00000004
#define ESA_MPU_ADDR_MASK           0x00000018
#define ESA_MPU_ADDR_330            0x00000000
#define ESA_MPU_ADDR_300            0x00000008
#define ESA_MPU_ADDR_320            0x00000010
#define ESA_MPU_ADDR_340            0x00000018
#define ESA_USE_PCI_TIMING          0x00000040
#define ESA_POSTED_WRITE_ENABLE     0x00000080
#define ESA_DMA_POLICY_MASK         0x00000700
#define ESA_DMA_DDMA                0x00000000
#define ESA_DMA_TDMA                0x00000100
#define ESA_DMA_PCPCI               0x00000200
#define ESA_DMA_WBDMA16             0x00000400
#define ESA_DMA_WBDMA4              0x00000500
#define ESA_DMA_WBDMA2              0x00000600
#define ESA_DMA_WBDMA1              0x00000700
#define ESA_DMA_SAFE_GUARD          0x00000800
#define ESA_HI_PERF_GP_ENABLE       0x00001000
#define ESA_PIC_SNOOP_MODE_0        0x00002000
#define ESA_PIC_SNOOP_MODE_1        0x00004000
#define ESA_SOUNDBLASTER_IRQ_MASK   0x00008000
#define ESA_RING_IN_ENABLE          0x00010000
#define ESA_SPDIF_TEST_MODE         0x00020000
#define ESA_CLK_MULT_MODE_SELECT_2  0x00040000
#define ESA_EEPROM_WRITE_ENABLE     0x00080000
#define ESA_CODEC_DIR_IN            0x00100000
#define ESA_HV_BUTTON_FROM_GD       0x00200000
#define ESA_REDUCED_DEBOUNCE        0x00400000
#define ESA_HV_CTRL_ENABLE          0x00800000
#define ESA_SPDIF_ENABLE            0x01000000
#define ESA_CLK_DIV_SELECT          0x06000000
#define ESA_CLK_DIV_BY_48           0x00000000
#define ESA_CLK_DIV_BY_49           0x02000000
#define ESA_CLK_DIV_BY_50           0x04000000
#define ESA_CLK_DIV_RESERVED        0x06000000
#define ESA_PM_CTRL_ENABLE          0x08000000
#define ESA_CLK_MULT_MODE_SELECT    0x30000000
#define ESA_CLK_MULT_MODE_SHIFT     28
#define ESA_CLK_MULT_MODE_0         0x00000000
#define ESA_CLK_MULT_MODE_1         0x10000000
#define ESA_CLK_MULT_MODE_2         0x20000000
#define ESA_CLK_MULT_MODE_3         0x30000000
#define ESA_INT_CLK_SELECT          0x40000000
#define ESA_INT_CLK_MULT_RESET      0x80000000

/* M3 */
#define ESA_INT_CLK_SRC_NOT_PCI     0x00100000
#define ESA_INT_CLK_MULT_ENABLE     0x80000000

#define ESA_PCI_ACPI_CONTROL        0x54
#define ESA_PCI_ACPI_D0             0x00000000
#define ESA_PCI_ACPI_D1             0xB4F70000
#define ESA_PCI_ACPI_D2             0xB4F7B4F7

#define ESA_PCI_USER_CONFIG         0x58
#define ESA_EXT_PCI_MASTER_ENABLE   0x00000001
#define ESA_SPDIF_OUT_SELECT        0x00000002
#define ESA_TEST_PIN_DIR_CTRL       0x00000004
#define ESA_AC97_CODEC_TEST         0x00000020
#define ESA_TRI_STATE_BUFFER        0x00000080
#define ESA_IN_CLK_12MHZ_SELECT     0x00000100
#define ESA_MULTI_FUNC_DISABLE      0x00000200
#define ESA_EXT_MASTER_PAIR_SEL     0x00000400
#define ESA_PCI_MASTER_SUPPORT      0x00000800
#define ESA_STOP_CLOCK_ENABLE       0x00001000
#define ESA_EAPD_DRIVE_ENABLE       0x00002000
#define ESA_REQ_TRI_STATE_ENABLE    0x00004000
#define ESA_REQ_LOW_ENABLE          0x00008000
#define ESA_MIDI_1_ENABLE           0x00010000
#define ESA_MIDI_2_ENABLE           0x00020000
#define ESA_SB_AUDIO_SYNC           0x00040000
#define ESA_HV_CTRL_TEST            0x00100000
#define ESA_SOUNDBLASTER_TEST       0x00400000

#define ESA_PCI_USER_CONFIG_C       0x5C

#define ESA_PCI_DDMA_CTRL           0x60
#define ESA_DDMA_ENABLE             0x00000001


/* Allegro registers */
#define ESA_HOST_INT_CTRL           0x18
#define ESA_SB_INT_ENABLE           0x0001
#define ESA_MPU401_INT_ENABLE       0x0002
#define ESA_ASSP_INT_ENABLE         0x0010
#define ESA_RING_INT_ENABLE         0x0020
#define ESA_HV_INT_ENABLE           0x0040
#define ESA_CLKRUN_GEN_ENABLE       0x0100
#define ESA_HV_CTRL_TO_PME          0x0400
#define ESA_SOFTWARE_RESET_ENABLE   0x8000

/*
 * should be using the above defines, probably.
 */
#define ESA_REGB_ENABLE_RESET       0x01
#define ESA_REGB_STOP_CLOCK         0x10

#define ESA_HOST_INT_STATUS         0x1A
#define ESA_SB_INT_PENDING          0x01
#define ESA_MPU401_INT_PENDING      0x02
#define ESA_ASSP_INT_PENDING        0x10
#define ESA_RING_INT_PENDING        0x20
#define ESA_HV_INT_PENDING          0x40

#define ESA_HARDWARE_VOL_CTRL       0x1B
#define ESA_SHADOW_MIX_REG_VOICE    0x1C
#define ESA_HW_VOL_COUNTER_VOICE    0x1D
#define ESA_SHADOW_MIX_REG_MASTER   0x1E
#define ESA_HW_VOL_COUNTER_MASTER   0x1F

#define ESA_CODEC_COMMAND           0x30
#define ESA_CODEC_READ_B            0x80

#define ESA_CODEC_STATUS            0x30
#define ESA_CODEC_BUSY_B            0x01

#define ESA_CODEC_DATA              0x32

#define ESA_RING_BUS_CTRL_A         0x36
#define ESA_RAC_PME_ENABLE          0x0100
#define ESA_RAC_SDFS_ENABLE         0x0200
#define ESA_LAC_PME_ENABLE          0x0400
#define ESA_LAC_SDFS_ENABLE         0x0800
#define ESA_SERIAL_AC_LINK_ENABLE   0x1000
#define ESA_IO_SRAM_ENABLE          0x2000
#define ESA_IIS_INPUT_ENABLE        0x8000

#define ESA_RING_BUS_CTRL_B         0x38
#define ESA_SECOND_CODEC_ID_MASK    0x0003
#define ESA_SPDIF_FUNC_ENABLE       0x0010
#define ESA_SECOND_AC_ENABLE        0x0020
#define ESA_SB_MODULE_INTF_ENABLE   0x0040
#define ESA_SSPE_ENABLE             0x0040
#define ESA_M3I_DOCK_ENABLE         0x0080

#define ESA_SDO_OUT_DEST_CTRL       0x3A
#define ESA_COMMAND_ADDR_OUT        0x0003
#define ESA_PCM_LR_OUT_LOCAL        0x0000
#define ESA_PCM_LR_OUT_REMOTE       0x0004
#define ESA_PCM_LR_OUT_MUTE         0x0008
#define ESA_PCM_LR_OUT_BOTH         0x000C
#define ESA_LINE1_DAC_OUT_LOCAL     0x0000
#define ESA_LINE1_DAC_OUT_REMOTE    0x0010
#define ESA_LINE1_DAC_OUT_MUTE      0x0020
#define ESA_LINE1_DAC_OUT_BOTH      0x0030
#define ESA_PCM_CLS_OUT_LOCAL       0x0000
#define ESA_PCM_CLS_OUT_REMOTE      0x0040
#define ESA_PCM_CLS_OUT_MUTE        0x0080
#define ESA_PCM_CLS_OUT_BOTH        0x00C0
#define ESA_PCM_RLF_OUT_LOCAL       0x0000
#define ESA_PCM_RLF_OUT_REMOTE      0x0100
#define ESA_PCM_RLF_OUT_MUTE        0x0200
#define ESA_PCM_RLF_OUT_BOTH        0x0300
#define ESA_LINE2_DAC_OUT_LOCAL     0x0000
#define ESA_LINE2_DAC_OUT_REMOTE    0x0400
#define ESA_LINE2_DAC_OUT_MUTE      0x0800
#define ESA_LINE2_DAC_OUT_BOTH      0x0C00
#define ESA_HANDSET_OUT_LOCAL       0x0000
#define ESA_HANDSET_OUT_REMOTE      0x1000
#define ESA_HANDSET_OUT_MUTE        0x2000
#define ESA_HANDSET_OUT_BOTH        0x3000
#define ESA_IO_CTRL_OUT_LOCAL       0x0000
#define ESA_IO_CTRL_OUT_REMOTE      0x4000
#define ESA_IO_CTRL_OUT_MUTE        0x8000
#define ESA_IO_CTRL_OUT_BOTH        0xC000

#define ESA_SDO_IN_DEST_CTRL        0x3C
#define ESA_STATUS_ADDR_IN          0x0003
#define ESA_PCM_LR_IN_LOCAL         0x0000
#define ESA_PCM_LR_IN_REMOTE        0x0004
#define ESA_PCM_LR_RESERVED         0x0008
#define ESA_PCM_LR_IN_BOTH          0x000C
#define ESA_LINE1_ADC_IN_LOCAL      0x0000
#define ESA_LINE1_ADC_IN_REMOTE     0x0010
#define ESA_LINE1_ADC_IN_MUTE       0x0020
#define ESA_MIC_ADC_IN_LOCAL        0x0000
#define ESA_MIC_ADC_IN_REMOTE       0x0040
#define ESA_MIC_ADC_IN_MUTE         0x0080
#define ESA_LINE2_DAC_IN_LOCAL      0x0000
#define ESA_LINE2_DAC_IN_REMOTE     0x0400
#define ESA_LINE2_DAC_IN_MUTE       0x0800
#define ESA_HANDSET_IN_LOCAL        0x0000
#define ESA_HANDSET_IN_REMOTE       0x1000
#define ESA_HANDSET_IN_MUTE         0x2000
#define ESA_IO_STATUS_IN_LOCAL      0x0000
#define ESA_IO_STATUS_IN_REMOTE     0x4000

#define ESA_SPDIF_IN_CTRL           0x3E
#define ESA_SPDIF_IN_ENABLE         0x0001

#define ESA_GPIO_DATA               0x60
#define ESA_GPIO_DATA_MASK          0x0FFF
#define ESA_GPIO_HV_STATUS          0x3000
#define ESA_GPIO_PME_STATUS         0x4000

#define ESA_GPIO_MASK               0x64
#define ESA_GPIO_DIRECTION          0x68
#define ESA_GPO_PRIMARY_AC97        0x0001
#define ESA_GPI_LINEOUT_SENSE       0x0004
#define ESA_GPO_SECONDARY_AC97      0x0008
#define ESA_GPI_VOL_DOWN            0x0010
#define ESA_GPI_VOL_UP              0x0020
#define ESA_GPI_IIS_CLK             0x0040
#define ESA_GPI_IIS_LRCLK           0x0080
#define ESA_GPI_IIS_DATA            0x0100
#define ESA_GPI_DOCKING_STATUS      0x0100
#define ESA_GPI_HEADPHONE_SENSE     0x0200
#define ESA_GPO_EXT_AMP_SHUTDOWN    0x1000

/* M3 */
#define ESA_GPO_M3_EXT_AMP_SHUTDN   0x0002

#define ESA_ASSP_INDEX_PORT         0x80
#define ESA_ASSP_MEMORY_PORT        0x82
#define ESA_ASSP_DATA_PORT          0x84

#define ESA_MPU401_DATA_PORT        0x98
#define ESA_MPU401_STATUS_PORT      0x99

#define ESA_CLK_MULT_DATA_PORT      0x9C

#define ESA_ASSP_CONTROL_A          0xA2
#define ESA_ASSP_0_WS_ENABLE        0x01
#define ESA_ASSP_CTRL_A_RESERVED1   0x02
#define ESA_ASSP_CTRL_A_RESERVED2   0x04
#define ESA_ASSP_CLK_49MHZ_SELECT   0x08
#define ESA_FAST_PLU_ENABLE         0x10
#define ESA_ASSP_CTRL_A_RESERVED3   0x20
#define ESA_DSP_CLK_36MHZ_SELECT    0x40

#define ESA_ASSP_CONTROL_B          0xA4
#define ESA_RESET_ASSP              0x00
#define ESA_RUN_ASSP                0x01
#define ESA_ENABLE_ASSP_CLOCK       0x00
#define ESA_STOP_ASSP_CLOCK         0x10
#define ESA_RESET_TOGGLE            0x40

#define ESA_ASSP_CONTROL_C          0xA6
#define ESA_ASSP_HOST_INT_ENABLE    0x01
#define ESA_FM_ADDR_REMAP_DISABLE   0x02
#define ESA_HOST_WRITE_PORT_ENABLE  0x08

#define ESA_ASSP_HOST_INT_STATUS    0xAC
#define ESA_DSP2HOST_REQ_PIORECORD  0x01
#define ESA_DSP2HOST_REQ_I2SRATE    0x02
#define ESA_DSP2HOST_REQ_TIMER      0x04

/*
 * ASSP control regs
 */
#define ESA_DSP_PORT_TIMER_COUNT    0x06

#define ESA_DSP_PORT_MEMORY_INDEX   0x80

#define ESA_DSP_PORT_MEMORY_TYPE    0x82
#define ESA_MEMTYPE_INTERNAL_CODE   0x0002
#define ESA_MEMTYPE_INTERNAL_DATA   0x0003
#define ESA_MEMTYPE_MASK            0x0003

#define ESA_DSP_PORT_MEMORY_DATA    0x84

#define ESA_DSP_PORT_CONTROL_REG_A  0xA2
#define ESA_DSP_PORT_CONTROL_REG_B  0xA4
#define ESA_DSP_PORT_CONTROL_REG_C  0xA6

#define ESA_REV_A_CODE_MEMORY_BEGIN         0x0000
#define ESA_REV_A_CODE_MEMORY_END           0x0FFF
#define ESA_REV_A_CODE_MEMORY_UNIT_LENGTH   0x0040
#define ESA_REV_A_CODE_MEMORY_LENGTH        (ESA_REV_A_CODE_MEMORY_END - ESA_REV_A_CODE_MEMORY_BEGIN + 1)

#define ESA_REV_B_CODE_MEMORY_BEGIN         0x0000
#define ESA_REV_B_CODE_MEMORY_END           0x0BFF
#define ESA_REV_B_CODE_MEMORY_UNIT_LENGTH   0x0040
#define ESA_REV_B_CODE_MEMORY_LENGTH        (ESA_REV_B_CODE_MEMORY_END - ESA_REV_B_CODE_MEMORY_BEGIN + 1)

#define ESA_REV_A_DATA_MEMORY_BEGIN         0x1000
#define ESA_REV_A_DATA_MEMORY_END           0x2FFF
#define ESA_REV_A_DATA_MEMORY_UNIT_LENGTH   0x0080
#define ESA_REV_A_DATA_MEMORY_LENGTH        (ESA_REV_A_DATA_MEMORY_END - ESA_REV_A_DATA_MEMORY_BEGIN + 1)

#define ESA_REV_B_DATA_MEMORY_BEGIN         0x1000
#define ESA_REV_B_DATA_MEMORY_END           0x2BFF
#define ESA_REV_B_DATA_MEMORY_UNIT_LENGTH   0x0080
#define ESA_REV_B_DATA_MEMORY_LENGTH        (ESA_REV_B_DATA_MEMORY_END - ESA_REV_B_DATA_MEMORY_BEGIN + 1)


#define ESA_NUM_UNITS_KERNEL_CODE          16
#define ESA_NUM_UNITS_KERNEL_DATA           2

#define ESA_NUM_UNITS_KERNEL_CODE_WITH_HSP 16
#define ESA_NUM_UNITS_KERNEL_DATA_WITH_HSP  5

/*
 * Kernel data layout
 */

#define ESA_DP_SHIFT_COUNT                  7

#define ESA_KDATA_BASE_ADDR                 0x1000
#define ESA_KDATA_BASE_ADDR2                0x1080

#define ESA_KDATA_TASK0                     (ESA_KDATA_BASE_ADDR + 0x0000)
#define ESA_KDATA_TASK1                     (ESA_KDATA_BASE_ADDR + 0x0001)
#define ESA_KDATA_TASK2                     (ESA_KDATA_BASE_ADDR + 0x0002)
#define ESA_KDATA_TASK3                     (ESA_KDATA_BASE_ADDR + 0x0003)
#define ESA_KDATA_TASK4                     (ESA_KDATA_BASE_ADDR + 0x0004)
#define ESA_KDATA_TASK5                     (ESA_KDATA_BASE_ADDR + 0x0005)
#define ESA_KDATA_TASK6                     (ESA_KDATA_BASE_ADDR + 0x0006)
#define ESA_KDATA_TASK7                     (ESA_KDATA_BASE_ADDR + 0x0007)
#define ESA_KDATA_TASK_ENDMARK              (ESA_KDATA_BASE_ADDR + 0x0008)

#define ESA_KDATA_CURRENT_TASK              (ESA_KDATA_BASE_ADDR + 0x0009)
#define ESA_KDATA_TASK_SWITCH               (ESA_KDATA_BASE_ADDR + 0x000A)

#define ESA_KDATA_INSTANCE0_POS3D           (ESA_KDATA_BASE_ADDR + 0x000B)
#define ESA_KDATA_INSTANCE1_POS3D           (ESA_KDATA_BASE_ADDR + 0x000C)
#define ESA_KDATA_INSTANCE2_POS3D           (ESA_KDATA_BASE_ADDR + 0x000D)
#define ESA_KDATA_INSTANCE3_POS3D           (ESA_KDATA_BASE_ADDR + 0x000E)
#define ESA_KDATA_INSTANCE4_POS3D           (ESA_KDATA_BASE_ADDR + 0x000F)
#define ESA_KDATA_INSTANCE5_POS3D           (ESA_KDATA_BASE_ADDR + 0x0010)
#define ESA_KDATA_INSTANCE6_POS3D           (ESA_KDATA_BASE_ADDR + 0x0011)
#define ESA_KDATA_INSTANCE7_POS3D           (ESA_KDATA_BASE_ADDR + 0x0012)
#define ESA_KDATA_INSTANCE8_POS3D           (ESA_KDATA_BASE_ADDR + 0x0013)
#define ESA_KDATA_INSTANCE_POS3D_ENDMARK    (ESA_KDATA_BASE_ADDR + 0x0014)

#define ESA_KDATA_INSTANCE0_SPKVIRT         (ESA_KDATA_BASE_ADDR + 0x0015)
#define ESA_KDATA_INSTANCE_SPKVIRT_ENDMARK  (ESA_KDATA_BASE_ADDR + 0x0016)

#define ESA_KDATA_INSTANCE0_SPDIF           (ESA_KDATA_BASE_ADDR + 0x0017)
#define ESA_KDATA_INSTANCE_SPDIF_ENDMARK    (ESA_KDATA_BASE_ADDR + 0x0018)

#define ESA_KDATA_INSTANCE0_MODEM           (ESA_KDATA_BASE_ADDR + 0x0019)
#define ESA_KDATA_INSTANCE_MODEM_ENDMARK    (ESA_KDATA_BASE_ADDR + 0x001A)

#define ESA_KDATA_INSTANCE0_SRC             (ESA_KDATA_BASE_ADDR + 0x001B)
#define ESA_KDATA_INSTANCE1_SRC             (ESA_KDATA_BASE_ADDR + 0x001C)
#define ESA_KDATA_INSTANCE_SRC_ENDMARK      (ESA_KDATA_BASE_ADDR + 0x001D)

#define ESA_KDATA_INSTANCE0_MINISRC         (ESA_KDATA_BASE_ADDR + 0x001E)
#define ESA_KDATA_INSTANCE1_MINISRC         (ESA_KDATA_BASE_ADDR + 0x001F)
#define ESA_KDATA_INSTANCE2_MINISRC         (ESA_KDATA_BASE_ADDR + 0x0020)
#define ESA_KDATA_INSTANCE3_MINISRC         (ESA_KDATA_BASE_ADDR + 0x0021)
#define ESA_KDATA_INSTANCE_MINISRC_ENDMARK  (ESA_KDATA_BASE_ADDR + 0x0022)

#define ESA_KDATA_INSTANCE0_CPYTHRU         (ESA_KDATA_BASE_ADDR + 0x0023)
#define ESA_KDATA_INSTANCE1_CPYTHRU         (ESA_KDATA_BASE_ADDR + 0x0024)
#define ESA_KDATA_INSTANCE_CPYTHRU_ENDMARK  (ESA_KDATA_BASE_ADDR + 0x0025)

#define ESA_KDATA_CURRENT_DMA               (ESA_KDATA_BASE_ADDR + 0x0026)
#define ESA_KDATA_DMA_SWITCH                (ESA_KDATA_BASE_ADDR + 0x0027)
#define ESA_KDATA_DMA_ACTIVE                (ESA_KDATA_BASE_ADDR + 0x0028)

#define ESA_KDATA_DMA_XFER0                 (ESA_KDATA_BASE_ADDR + 0x0029)
#define ESA_KDATA_DMA_XFER1                 (ESA_KDATA_BASE_ADDR + 0x002A)
#define ESA_KDATA_DMA_XFER2                 (ESA_KDATA_BASE_ADDR + 0x002B)
#define ESA_KDATA_DMA_XFER3                 (ESA_KDATA_BASE_ADDR + 0x002C)
#define ESA_KDATA_DMA_XFER4                 (ESA_KDATA_BASE_ADDR + 0x002D)
#define ESA_KDATA_DMA_XFER5                 (ESA_KDATA_BASE_ADDR + 0x002E)
#define ESA_KDATA_DMA_XFER6                 (ESA_KDATA_BASE_ADDR + 0x002F)
#define ESA_KDATA_DMA_XFER7                 (ESA_KDATA_BASE_ADDR + 0x0030)
#define ESA_KDATA_DMA_XFER8                 (ESA_KDATA_BASE_ADDR + 0x0031)
#define ESA_KDATA_DMA_XFER_ENDMARK          (ESA_KDATA_BASE_ADDR + 0x0032)

#define ESA_KDATA_I2S_SAMPLE_COUNT          (ESA_KDATA_BASE_ADDR + 0x0033)
#define ESA_KDATA_I2S_INT_METER             (ESA_KDATA_BASE_ADDR + 0x0034)
#define ESA_KDATA_I2S_ACTIVE                (ESA_KDATA_BASE_ADDR + 0x0035)

#define ESA_KDATA_TIMER_COUNT_RELOAD        (ESA_KDATA_BASE_ADDR + 0x0036)
#define ESA_KDATA_TIMER_COUNT_CURRENT       (ESA_KDATA_BASE_ADDR + 0x0037)

#define ESA_KDATA_HALT_SYNCH_CLIENT         (ESA_KDATA_BASE_ADDR + 0x0038)
#define ESA_KDATA_HALT_SYNCH_DMA            (ESA_KDATA_BASE_ADDR + 0x0039)
#define ESA_KDATA_HALT_ACKNOWLEDGE          (ESA_KDATA_BASE_ADDR + 0x003A)

#define ESA_KDATA_ADC1_XFER0                (ESA_KDATA_BASE_ADDR + 0x003B)
#define ESA_KDATA_ADC1_XFER_ENDMARK         (ESA_KDATA_BASE_ADDR + 0x003C)
#define ESA_KDATA_ADC1_LEFT_VOLUME	    (ESA_KDATA_BASE_ADDR + 0x003D)
#define ESA_KDATA_ADC1_RIGHT_VOLUME  	    (ESA_KDATA_BASE_ADDR + 0x003E)
#define ESA_KDATA_ADC1_LEFT_SUR_VOL	    (ESA_KDATA_BASE_ADDR + 0x003F)
#define ESA_KDATA_ADC1_RIGHT_SUR_VOL	    (ESA_KDATA_BASE_ADDR + 0x0040)

#define ESA_KDATA_ADC2_XFER0                (ESA_KDATA_BASE_ADDR + 0x0041)
#define ESA_KDATA_ADC2_XFER_ENDMARK         (ESA_KDATA_BASE_ADDR + 0x0042)
#define ESA_KDATA_ADC2_LEFT_VOLUME	    (ESA_KDATA_BASE_ADDR + 0x0043)
#define ESA_KDATA_ADC2_RIGHT_VOLUME	    (ESA_KDATA_BASE_ADDR + 0x0044)
#define ESA_KDATA_ADC2_LEFT_SUR_VOL	    (ESA_KDATA_BASE_ADDR + 0x0045)
#define ESA_KDATA_ADC2_RIGHT_SUR_VOL	    (ESA_KDATA_BASE_ADDR + 0x0046)

#define ESA_KDATA_CD_XFER0		    (ESA_KDATA_BASE_ADDR + 0x0047)					
#define ESA_KDATA_CD_XFER_ENDMARK	    (ESA_KDATA_BASE_ADDR + 0x0048)
#define ESA_KDATA_CD_LEFT_VOLUME	    (ESA_KDATA_BASE_ADDR + 0x0049)
#define ESA_KDATA_CD_RIGHT_VOLUME	    (ESA_KDATA_BASE_ADDR + 0x004A)
#define ESA_KDATA_CD_LEFT_SUR_VOL	    (ESA_KDATA_BASE_ADDR + 0x004B)
#define ESA_KDATA_CD_RIGHT_SUR_VOL	    (ESA_KDATA_BASE_ADDR + 0x004C)

#define ESA_KDATA_MIC_XFER0		    (ESA_KDATA_BASE_ADDR + 0x004D)
#define ESA_KDATA_MIC_XFER_ENDMARK	    (ESA_KDATA_BASE_ADDR + 0x004E)
#define ESA_KDATA_MIC_VOLUME		    (ESA_KDATA_BASE_ADDR + 0x004F)
#define ESA_KDATA_MIC_SUR_VOL		    (ESA_KDATA_BASE_ADDR + 0x0050)

#define ESA_KDATA_I2S_XFER0                 (ESA_KDATA_BASE_ADDR + 0x0051)
#define ESA_KDATA_I2S_XFER_ENDMARK          (ESA_KDATA_BASE_ADDR + 0x0052)

#define ESA_KDATA_CHI_XFER0                 (ESA_KDATA_BASE_ADDR + 0x0053)
#define ESA_KDATA_CHI_XFER_ENDMARK          (ESA_KDATA_BASE_ADDR + 0x0054)

#define ESA_KDATA_SPDIF_XFER                (ESA_KDATA_BASE_ADDR + 0x0055)
#define ESA_KDATA_SPDIF_CURRENT_FRAME       (ESA_KDATA_BASE_ADDR + 0x0056)
#define ESA_KDATA_SPDIF_FRAME0              (ESA_KDATA_BASE_ADDR + 0x0057)
#define ESA_KDATA_SPDIF_FRAME1              (ESA_KDATA_BASE_ADDR + 0x0058)
#define ESA_KDATA_SPDIF_FRAME2              (ESA_KDATA_BASE_ADDR + 0x0059)

#define ESA_KDATA_SPDIF_REQUEST             (ESA_KDATA_BASE_ADDR + 0x005A)
#define ESA_KDATA_SPDIF_TEMP                (ESA_KDATA_BASE_ADDR + 0x005B)

#define ESA_KDATA_SPDIFIN_XFER0             (ESA_KDATA_BASE_ADDR + 0x005C)
#define ESA_KDATA_SPDIFIN_XFER_ENDMARK      (ESA_KDATA_BASE_ADDR + 0x005D)
#define ESA_KDATA_SPDIFIN_INT_METER         (ESA_KDATA_BASE_ADDR + 0x005E)

#define ESA_KDATA_DSP_RESET_COUNT           (ESA_KDATA_BASE_ADDR + 0x005F)
#define ESA_KDATA_DEBUG_OUTPUT              (ESA_KDATA_BASE_ADDR + 0x0060)

#define ESA_KDATA_KERNEL_ISR_LIST           (ESA_KDATA_BASE_ADDR + 0x0061)

#define ESA_KDATA_KERNEL_ISR_CBSR1          (ESA_KDATA_BASE_ADDR + 0x0062)
#define ESA_KDATA_KERNEL_ISR_CBER1          (ESA_KDATA_BASE_ADDR + 0x0063)
#define ESA_KDATA_KERNEL_ISR_CBCR           (ESA_KDATA_BASE_ADDR + 0x0064)
#define ESA_KDATA_KERNEL_ISR_AR0            (ESA_KDATA_BASE_ADDR + 0x0065)
#define ESA_KDATA_KERNEL_ISR_AR1            (ESA_KDATA_BASE_ADDR + 0x0066)
#define ESA_KDATA_KERNEL_ISR_AR2            (ESA_KDATA_BASE_ADDR + 0x0067)
#define ESA_KDATA_KERNEL_ISR_AR3            (ESA_KDATA_BASE_ADDR + 0x0068)
#define ESA_KDATA_KERNEL_ISR_AR4            (ESA_KDATA_BASE_ADDR + 0x0069)
#define ESA_KDATA_KERNEL_ISR_AR5            (ESA_KDATA_BASE_ADDR + 0x006A)
#define ESA_KDATA_KERNEL_ISR_BRCR           (ESA_KDATA_BASE_ADDR + 0x006B)
#define ESA_KDATA_KERNEL_ISR_PASR           (ESA_KDATA_BASE_ADDR + 0x006C)
#define ESA_KDATA_KERNEL_ISR_PAER           (ESA_KDATA_BASE_ADDR + 0x006D)

#define ESA_KDATA_CLIENT_SCRATCH0           (ESA_KDATA_BASE_ADDR + 0x006E)
#define ESA_KDATA_CLIENT_SCRATCH1           (ESA_KDATA_BASE_ADDR + 0x006F)
#define ESA_KDATA_KERNEL_SCRATCH            (ESA_KDATA_BASE_ADDR + 0x0070)
#define ESA_KDATA_KERNEL_ISR_SCRATCH        (ESA_KDATA_BASE_ADDR + 0x0071)

#define ESA_KDATA_OUEUE_LEFT                (ESA_KDATA_BASE_ADDR + 0x0072)
#define ESA_KDATA_QUEUE_RIGHT               (ESA_KDATA_BASE_ADDR + 0x0073)

#define ESA_KDATA_ADC1_REQUEST              (ESA_KDATA_BASE_ADDR + 0x0074)
#define ESA_KDATA_ADC2_REQUEST              (ESA_KDATA_BASE_ADDR + 0x0075)
#define ESA_KDATA_CD_REQUEST		    (ESA_KDATA_BASE_ADDR + 0x0076)
#define ESA_KDATA_MIC_REQUEST		    (ESA_KDATA_BASE_ADDR + 0x0077)

#define ESA_KDATA_ADC1_MIXER_REQUEST        (ESA_KDATA_BASE_ADDR + 0x0078)
#define ESA_KDATA_ADC2_MIXER_REQUEST        (ESA_KDATA_BASE_ADDR + 0x0079)
#define ESA_KDATA_CD_MIXER_REQUEST	    (ESA_KDATA_BASE_ADDR + 0x007A)
#define ESA_KDATA_MIC_MIXER_REQUEST	    (ESA_KDATA_BASE_ADDR + 0x007B)
#define ESA_KDATA_MIC_SYNC_COUNTER	    (ESA_KDATA_BASE_ADDR + 0x007C)

/*
 * second 'segment' (?) reserved for mixer
 * buffers..
 */

#define ESA_KDATA_MIXER_WORD0               (ESA_KDATA_BASE_ADDR2 + 0x0000)
#define ESA_KDATA_MIXER_WORD1               (ESA_KDATA_BASE_ADDR2 + 0x0001)
#define ESA_KDATA_MIXER_WORD2               (ESA_KDATA_BASE_ADDR2 + 0x0002)
#define ESA_KDATA_MIXER_WORD3               (ESA_KDATA_BASE_ADDR2 + 0x0003)
#define ESA_KDATA_MIXER_WORD4               (ESA_KDATA_BASE_ADDR2 + 0x0004)
#define ESA_KDATA_MIXER_WORD5               (ESA_KDATA_BASE_ADDR2 + 0x0005)
#define ESA_KDATA_MIXER_WORD6               (ESA_KDATA_BASE_ADDR2 + 0x0006)
#define ESA_KDATA_MIXER_WORD7               (ESA_KDATA_BASE_ADDR2 + 0x0007)
#define ESA_KDATA_MIXER_WORD8               (ESA_KDATA_BASE_ADDR2 + 0x0008)
#define ESA_KDATA_MIXER_WORD9               (ESA_KDATA_BASE_ADDR2 + 0x0009)
#define ESA_KDATA_MIXER_WORDA               (ESA_KDATA_BASE_ADDR2 + 0x000A)
#define ESA_KDATA_MIXER_WORDB               (ESA_KDATA_BASE_ADDR2 + 0x000B)
#define ESA_KDATA_MIXER_WORDC               (ESA_KDATA_BASE_ADDR2 + 0x000C)
#define ESA_KDATA_MIXER_WORDD               (ESA_KDATA_BASE_ADDR2 + 0x000D)
#define ESA_KDATA_MIXER_WORDE               (ESA_KDATA_BASE_ADDR2 + 0x000E)
#define ESA_KDATA_MIXER_WORDF               (ESA_KDATA_BASE_ADDR2 + 0x000F)

#define ESA_KDATA_MIXER_XFER0               (ESA_KDATA_BASE_ADDR2 + 0x0010)
#define ESA_KDATA_MIXER_XFER1               (ESA_KDATA_BASE_ADDR2 + 0x0011)
#define ESA_KDATA_MIXER_XFER2               (ESA_KDATA_BASE_ADDR2 + 0x0012)
#define ESA_KDATA_MIXER_XFER3               (ESA_KDATA_BASE_ADDR2 + 0x0013)
#define ESA_KDATA_MIXER_XFER4               (ESA_KDATA_BASE_ADDR2 + 0x0014)
#define ESA_KDATA_MIXER_XFER5               (ESA_KDATA_BASE_ADDR2 + 0x0015)
#define ESA_KDATA_MIXER_XFER6               (ESA_KDATA_BASE_ADDR2 + 0x0016)
#define ESA_KDATA_MIXER_XFER7               (ESA_KDATA_BASE_ADDR2 + 0x0017)
#define ESA_KDATA_MIXER_XFER8               (ESA_KDATA_BASE_ADDR2 + 0x0018)
#define ESA_KDATA_MIXER_XFER9               (ESA_KDATA_BASE_ADDR2 + 0x0019)
#define ESA_KDATA_MIXER_XFER_ENDMARK        (ESA_KDATA_BASE_ADDR2 + 0x001A)

#define ESA_KDATA_MIXER_TASK_NUMBER         (ESA_KDATA_BASE_ADDR2 + 0x001B)
#define ESA_KDATA_CURRENT_MIXER             (ESA_KDATA_BASE_ADDR2 + 0x001C)
#define ESA_KDATA_MIXER_ACTIVE              (ESA_KDATA_BASE_ADDR2 + 0x001D)
#define ESA_KDATA_MIXER_BANK_STATUS         (ESA_KDATA_BASE_ADDR2 + 0x001E)
#define ESA_KDATA_DAC_LEFT_VOLUME	        (ESA_KDATA_BASE_ADDR2 + 0x001F)
#define ESA_KDATA_DAC_RIGHT_VOLUME          (ESA_KDATA_BASE_ADDR2 + 0x0020)

#define ESA_MAX_INSTANCE_MINISRC            (ESA_KDATA_INSTANCE_MINISRC_ENDMARK - ESA_KDATA_INSTANCE0_MINISRC)
#define ESA_MAX_VIRTUAL_DMA_CHANNELS        (ESA_KDATA_DMA_XFER_ENDMARK - ESA_KDATA_DMA_XFER0)
#define ESA_MAX_VIRTUAL_MIXER_CHANNELS      (ESA_KDATA_MIXER_XFER_ENDMARK - ESA_KDATA_MIXER_XFER0)
#define ESA_MAX_VIRTUAL_ADC1_CHANNELS       (ESA_KDATA_ADC1_XFER_ENDMARK - ESA_KDATA_ADC1_XFER0)

/*
 * client data area offsets
 */
#define ESA_CDATA_INSTANCE_READY            0x00

#define ESA_CDATA_HOST_SRC_ADDRL            0x01
#define ESA_CDATA_HOST_SRC_ADDRH            0x02
#define ESA_CDATA_HOST_SRC_END_PLUS_1L      0x03
#define ESA_CDATA_HOST_SRC_END_PLUS_1H      0x04
#define ESA_CDATA_HOST_SRC_CURRENTL         0x05
#define ESA_CDATA_HOST_SRC_CURRENTH         0x06

#define ESA_CDATA_IN_BUF_CONNECT            0x07
#define ESA_CDATA_OUT_BUF_CONNECT           0x08

#define ESA_CDATA_IN_BUF_BEGIN              0x09
#define ESA_CDATA_IN_BUF_END_PLUS_1         0x0A
#define ESA_CDATA_IN_BUF_HEAD               0x0B
#define ESA_CDATA_IN_BUF_TAIL               0x0C
#define ESA_CDATA_OUT_BUF_BEGIN             0x0D
#define ESA_CDATA_OUT_BUF_END_PLUS_1        0x0E
#define ESA_CDATA_OUT_BUF_HEAD              0x0F
#define ESA_CDATA_OUT_BUF_TAIL              0x10

#define ESA_CDATA_DMA_CONTROL               0x11
#define ESA_CDATA_RESERVED                  0x12

#define ESA_CDATA_FREQUENCY                 0x13
#define ESA_CDATA_LEFT_VOLUME               0x14
#define ESA_CDATA_RIGHT_VOLUME              0x15
#define ESA_CDATA_LEFT_SUR_VOL              0x16
#define ESA_CDATA_RIGHT_SUR_VOL             0x17

#define ESA_CDATA_HEADER_LEN                0x18

#define ESA_SRC3_DIRECTION_OFFSET           ESA_CDATA_HEADER_LEN
#define ESA_SRC3_MODE_OFFSET                (ESA_CDATA_HEADER_LEN + 1)
#define ESA_SRC3_WORD_LENGTH_OFFSET         (ESA_CDATA_HEADER_LEN + 2)
#define ESA_SRC3_PARAMETER_OFFSET           (ESA_CDATA_HEADER_LEN + 3)
#define ESA_SRC3_COEFF_ADDR_OFFSET          (ESA_CDATA_HEADER_LEN + 8)
#define ESA_SRC3_FILTAP_ADDR_OFFSET         (ESA_CDATA_HEADER_LEN + 10)
#define ESA_SRC3_TEMP_INBUF_ADDR_OFFSET     (ESA_CDATA_HEADER_LEN + 16)
#define ESA_SRC3_TEMP_OUTBUF_ADDR_OFFSET    (ESA_CDATA_HEADER_LEN + 17)

#define ESA_MINISRC_IN_BUFFER_SIZE   (0x50 * 2)
#define ESA_MINISRC_OUT_BUFFER_SIZE  (0x50 * 2 * 2)
#define ESA_MINISRC_OUT_BUFFER_SIZE  (0x50 * 2 * 2)
#define ESA_MINISRC_TMP_BUFFER_SIZE  (112 + (ESA_MINISRC_BIQUAD_STAGE * 3 + 4) * 2 * 2)
#define ESA_MINISRC_BIQUAD_STAGE	2
#define ESA_MINISRC_COEF_LOC		0x175

#define ESA_DMACONTROL_BLOCK_MASK           0x000F
#define  ESA_DMAC_BLOCK0_SELECTOR           0x0000
#define  ESA_DMAC_BLOCK1_SELECTOR           0x0001
#define  ESA_DMAC_BLOCK2_SELECTOR           0x0002
#define  ESA_DMAC_BLOCK3_SELECTOR           0x0003
#define  ESA_DMAC_BLOCK4_SELECTOR           0x0004
#define  ESA_DMAC_BLOCK5_SELECTOR           0x0005
#define  ESA_DMAC_BLOCK6_SELECTOR           0x0006
#define  ESA_DMAC_BLOCK7_SELECTOR           0x0007
#define  ESA_DMAC_BLOCK8_SELECTOR           0x0008
#define  ESA_DMAC_BLOCK9_SELECTOR           0x0009
#define  ESA_DMAC_BLOCKA_SELECTOR           0x000A
#define  ESA_DMAC_BLOCKB_SELECTOR           0x000B
#define  ESA_DMAC_BLOCKC_SELECTOR           0x000C
#define  ESA_DMAC_BLOCKD_SELECTOR           0x000D
#define  ESA_DMAC_BLOCKE_SELECTOR           0x000E
#define  ESA_DMAC_BLOCKF_SELECTOR           0x000F
#define ESA_DMACONTROL_PAGE_MASK            0x00F0
#define  ESA_DMAC_PAGE0_SELECTOR            0x0030
#define  ESA_DMAC_PAGE1_SELECTOR            0x0020
#define  ESA_DMAC_PAGE2_SELECTOR            0x0010
#define  ESA_DMAC_PAGE3_SELECTOR            0x0000
#define ESA_DMACONTROL_AUTOREPEAT           0x1000
#define ESA_DMACONTROL_STOPPED              0x2000
#define ESA_DMACONTROL_DIRECTION            0x0100
