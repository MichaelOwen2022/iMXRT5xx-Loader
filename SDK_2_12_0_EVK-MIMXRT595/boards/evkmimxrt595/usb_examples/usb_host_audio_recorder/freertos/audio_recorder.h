/*
 * Copyright (c) 2015 -2016, Freescale Semiconductor, Inc.
 * Copyright 2016, 2018, 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __AUDIO_RECORDER_H__
#define __AUDIO_RECORDER_H__

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define MOUSE_MAX_COUNT  2
#define HOST_VOLUME_STEP 1
#define HOST_MIN_VOLUME  0
#define HOST_MAX_VOLUME  10

#define EP_TYPE_MASK (0x03)

/*! @brief host app run status */
typedef enum __host_audio_app_runState
{
    kUSB_HostAudioRunIdle = 0,                /*!< idle */
    kUSB_HostAudioRunSetControlInterface,     /*!< execute set control interface code */
    kUSB_HostAudioRunSetControlInterfaceDone, /*!<  set control interface done */
    kUSB_HostAudioRunWaitSetControlInterface, /*!< wait control interface done */
    kUSB_HostAudioRunWaitSetStreamInterface,  /*!< wait steam interface done */
    kUSB_HostAudioRunSetInterfaceDone,        /*!< set interface done */
    kUSB_HostAudioRunWaitGetStreamDescriptor, /*!< wait get stream descriotor */
    kUSB_HostAudioRunGetStreamDescriptorDone, /*!< get stream descriotor done */
    /*audio 1.0 only state machine start*/
    kUSB_HostAudioRunWaitAudioGetMinVolume, /*!< wait get minimum volume command done */
    kUSB_HostAudioRunAudioGetMaxVolume,     /*!< execute get maximum volume command done */
    kUSB_HostAudioRunWaitAudioGetMaxVolume, /*!< wait get maximum volume command done */
    kUSB_HostAudioRunAudioGetResVolume,     /*!< execute get res volume command done */
    kUSB_HostAudioRunWaitAudioGetResVolume, /*!< wait get res volume command done */
    /*audio 1.0 state machine end*/
    kUSB_HostAudioRunAudioConfigChannel,          /*!< execute config audio channel */
    kUSB_HostAudioRunWaitAudioConfigChannel,      /*!< wait config audio channel */
    kUSB_HostAudioRunAudioConfigChannel1Vol,      /*!< execute config audio channel 1*/
    kUSB_HostAudioRunWaitAudioConfigChannel1Vol,  /*!< wait config audio channel 1*/
    kUSB_HostAudioRunAudioConfigChannel2Vol,      /*!< execute config audio channel 2*/
    kUSB_HostAudioRunWaitAudioConfigChannel2Vol,  /*!< wait config audio channel 2*/
    kUSB_HostAudioRunWaitAudioSetCurSamplingFreq, /*!< wait config audio set sampling freq*/
    kUSB_HostAudioRunAudioSetCurSamplingFreq,     /*!< execute audio set current sampling freq*/
    kUSB_HostAudioRunAudioConfigMute,             /*!< execute audio config mute*/
    kUSB_HostAudioRunWaitAudioConfigMute,         /*!< wait audio config mute*/
    kUSB_HostAudioRunAudioDone,
    kUSB_HostAudioRunAudioWaitCommand,
    kUSB_HostAudioRecorderStartWriteSdcard, /*!< audio recorder sdcard start */
    /*the followign state is used in audio2.0 only*/
    kUSB_HostAudioRunWaitAudioGetVolumeRang,              /*!< wait get volume rang command done */
    kUSB_HostAudioRunAudioGetSampleFrequencyRange,        /*!< get sample frequency range */
    kUSB_HostAudioRunWaitAudioGetSampleFrequencyRange,    /*!< wait get sample frequency range */
    kUSB_HostAudioRunAudioGetAllSampleFrequencyRange,     /*!< get all sample frequency range */
    kUSB_HostAudioRunWaitAudioGetAllSampleFrequencyRange, /*!< wait get all sample frequency range */
    kUSB_HostAudioRunAudioGetCurrentVolume,               /*!< get current volume*/
    kUSB_HostAudioRunWaitAudioGetCurrentVolumeDone,       /*!< wait get current volume */
} host_audio_app_runState_t;
/*! @brief USB host audio instance structure */
typedef struct _audio_recorder_instance
{
    usb_device_handle deviceHandle;              /*!< the audio recorder device handle */
    usb_host_class_handle classHandle;           /*!< the audio recorder class handle */
    usb_host_interface_handle controlIntfHandle; /*!< the audio recorder control interface handle */
    usb_host_interface_handle streamIntfHandle;  /*!< the audio recorder stream interface handle */
    uint16_t deviceAudioVersion;                 /*!< device's current Audio version, 16bit to aligned with Spec*/
    uint8_t devState;                            /*!< device attach/detach status */
    uint8_t prevState;                           /*!< device attach/detach previous status */
    uint8_t runState;                            /*!< audio recorder application run status */
    uint8_t runWaitState;                        /*!< audio recorder application run status */
    uint16_t sendPacketSize;                     /*!< iso send packet size */
    uint8_t dataReceived;                        /*!< data recived  */
    uint8_t deviceIsUsed;                        /*!< device is used or not */
    volatile uint8_t audioRecorderStreamUsbBufferIndex;
    volatile uint32_t audioRecorderStreamPutBufferIndex;
    volatile uint32_t audioRecorderStreamGetBufferIndex;
    volatile uint32_t recorderUsedSpace;
    volatile uint16_t audioFileIndex;
    uint8_t audioRecorderWriteSdcardFlag;
    uint16_t audioRecorderIsoMaxPacketSize;
    volatile uint8_t audioRecorderRunFlag;
} audio_recorder_instance_t;
typedef struct _usb_audio_2_0_layout3_struct
{
    int32_t wMIN;
    int32_t wMAX;
    uint32_t wRES;
} usb_audio_2_0_layout3_struct_t;

/*!
 * @brief host audio sdcard initialization function.
 *
 * This function implements the sdcard detect and initialization
 *
 */
extern usb_status_t USB_HostAudioAppSDcardInit(void);
#endif /* __AUDIO_RECORDER_H__ */
