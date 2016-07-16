/*-------------------------------------------------------------------------------------------------------------------*\
 *  @FileName       :: AmbaSensor_OV9750Table.c
 *
 *  @Copyright      :: Copyright (C) 2013 Ambarella Corporation. All rights reserved.
 *
 *                     No part of this file may be reproduced, stored in a retrieval system,
 *                     or transmitted, in any form, or by any means, electronic, mechanical, photocopying,
 *                     recording, or otherwise, without the prior consent of Ambarella Corporation.
 *
 *  @Description    :: Control APIs of OmniVision OV9750 CMOS sensor with MIPI interface
\*-------------------------------------------------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "AmbaDataType.h"
#include "AmbaDSP.h"
#include "AmbaSensor.h"
#include "AmbaSensor_OV9750.h"

/*-----------------------------------------------------------------------------------------------*\
 * Sensor Device Information
\*-----------------------------------------------------------------------------------------------*/
const AMBA_SENSOR_DEVICE_INFO_s OV9750DeviceInfo = {
    .UnitCellWidth          = 3.75,
    .UnitCellHeight         = 3.75,
    .NumTotalPixelCols      = 1280,
    .NumTotalPixelRows      = 960,
    .NumEffectivePixelCols  = 1280,
    .NumEffectivePixelRows  = 960,
    .MinAnalogGainFactor    = 1.0,
    .MaxAnalogGainFactor    = 62.0,
    .MinDigitalGainFactor   = 1.0,
    .MaxDigitalGainFactor   = 1.0,

    .FrameRateCtrlInfo = {
        .CommunicationTime      = AMBA_SENSOR_COMMUNICATION_AT_NON_VBLANK,
        .FirstReflectedFrame    = 2,
        .FirstBadFrame          = 1,
        .NumBadFrames           = 1
    },
    .ShutterSpeedCtrlInfo = {
        .CommunicationTime      = AMBA_SENSOR_COMMUNICATION_AT_NON_VBLANK,
        .FirstReflectedFrame    = 2,
        .FirstBadFrame          = 1,
        .NumBadFrames           = 1
    },
    .AnalogGainCtrlInfo = {
        .CommunicationTime      = AMBA_SENSOR_COMMUNICATION_AT_NON_VBLANK,
        .FirstReflectedFrame    = 1,
        .FirstBadFrame          = 1,
        .NumBadFrames           = 1
    },
    .DigitalGainCtrlInfo = {
        .CommunicationTime      = AMBA_SENSOR_COMMUNICATION_AT_NON_VBLANK,
        .FirstReflectedFrame    = 1,
        .FirstBadFrame          = 1,
        .NumBadFrames           = 1
    },
};

/*-----------------------------------------------------------------------------------------------*\
 * Sensor register settings of each readout modes
\*-----------------------------------------------------------------------------------------------*/
OV9750_REG_s OV9750RegTable[OV9750_NUM_READOUT_MODE_REG] = {
    {0x0100, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {0x0300, {0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04}},
    {0x0302, {0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64}},
    {0x0303, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03}},
    {0x0304, {0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03}},
    {0x0305, {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01}},
    {0x0306, {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01}},
    {0x030a, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {0x030b, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {0x030d, {0x1e, 0x1e, 0x1e, 0x1e, 0x1e, 0x1e, 0x1e, 0x1e, 0x1e, 0x1e}},
    {0x030e, {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01}},
    {0x030f, {0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04}},
    {0x0312, {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01}},
    {0x031e, {0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04}},
    {0x3000, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {0x3001, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {0x3002, {0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21}},
    {0x3005, {0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0}},
    {0x3011, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {0x3016, {0x53, 0x53, 0x53, 0x53, 0x53, 0x53, 0x53, 0x53, 0x53, 0x53}},
    {0x3018, {0x12, 0x32, 0x12, 0x32, 0x12, 0x32, 0x12, 0x32, 0x32, 0x32}},
    {0x301a, {0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0}},
    {0x301b, {0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0}},
    {0x301c, {0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0}},
    {0x301d, {0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0}},
    {0x301e, {0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0}},
    {0x3022, {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01}},
    {0x3031, {0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a}},
    {0x3032, {0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80}},
    {0x303c, {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff}},
    {0x303e, {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff}},
    {0x3040, {0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0}},
    {0x3041, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {0x3042, {0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0}},
    {0x3104, {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01}},
    {0x3106, {0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15}},
    {0x3107, {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01}},
    {0x3500, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {0x3501, {0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d}},
    {0x3502, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {0x3503, {0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78}}, // gain & shutter reflection timing (default is 0x8)
    {0x3504, {0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03}},
    {0x3505, {0x83, 0x83, 0x83, 0x83, 0x83, 0x83, 0x83, 0x83, 0x83, 0x83}},
    {0x3508, {0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02}},
    {0x3509, {0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80}},
    {0x3600, {0x65, 0x65, 0x65, 0x65, 0x65, 0x65, 0x65, 0x65, 0x65, 0x65}},
    {0x3601, {0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60}},
    {0x3602, {0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22}},
    {0x3610, {0xe8, 0xe8, 0xe8, 0xe8, 0xe8, 0xe8, 0xe8, 0xe8, 0xe8, 0xe8}},
    {0x3611, {0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56}},
    {0x3612, {0x48, 0x48, 0x48, 0x48, 0x48, 0x48, 0x48, 0x48, 0x18, 0x18}},
    {0x3613, {0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a}},
    {0x3614, {0x91, 0x91, 0x91, 0x91, 0x91, 0x91, 0x91, 0x91, 0x91, 0x91}},
    {0x3615, {0x79, 0x79, 0x79, 0x79, 0x79, 0x79, 0x79, 0x79, 0x79, 0x79}},
    {0x3617, {0x57, 0x57, 0x57, 0x57, 0x57, 0x57, 0x57, 0x57, 0x57, 0x57}},
    {0x3621, {0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90}},
    {0x3622, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {0x3623, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {0x3633, {0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10}},
    {0x3634, {0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10}},
    {0x3635, {0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14}},
    {0x3636, {0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13}},
    {0x3650, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {0x3652, {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff}},
    {0x3654, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {0x3653, {0x34, 0x34, 0x34, 0x34, 0x34, 0x34, 0x34, 0x34, 0x34, 0x34}},
    {0x3655, {0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20}},
    {0x3656, {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff}},
    {0x3657, {0xc4, 0xc4, 0xc4, 0xc4, 0xc4, 0xc4, 0xc4, 0xc4, 0xc4, 0xc4}},
    {0x365a, {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff}},
    {0x365b, {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff}},
    {0x365e, {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff}},
    {0x365f, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {0x3668, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {0x366a, {0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07}},
    {0x366d, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {0x366e, {0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10}},
    {0x3702, {0x1d, 0x1d, 0x1d, 0x1d, 0x1d, 0x1d, 0x1d, 0x1d, 0x1d, 0x1d}},
    {0x3703, {0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10}},
    {0x3704, {0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14}},
    {0x3705, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {0x3706, {0x27, 0x27, 0x27, 0x27, 0x27, 0x27, 0x27, 0x27, 0x27, 0x27}},
    {0x3709, {0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24}},
    {0x370a, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {0x370b, {0x7d, 0x7d, 0x7d, 0x7d, 0x7d, 0x7d, 0x7d, 0x7d, 0x7d, 0x7d}},
    {0x3714, {0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24}},
    {0x371a, {0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e, 0x5e}},
    {0x3730, {0x82, 0x82, 0x82, 0x82, 0x82, 0x82, 0x82, 0x82, 0x82, 0x82}},
    {0x3733, {0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10}},
    {0x373e, {0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18}},
    {0x3755, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {0x3758, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {0x375b, {0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13}},
    {0x3772, {0x23, 0x23, 0x23, 0x23, 0x23, 0x23, 0x23, 0x23, 0x23, 0x23}},
    {0x3773, {0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05}},
    {0x3774, {0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16}},
    {0x3775, {0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12}},
    {0x3776, {0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08}},
    {0x37a8, {0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38}},
    {0x37b5, {0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36}},
    {0x37c2, {0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04}},
    {0x37c5, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {0x37c7, {0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30}},
    {0x37c8, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {0x37d1, {0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13}},
    {0x3800, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {0x3801, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {0x3802, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {0x3803, {0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04}},
    {0x3804, {0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05}},
    {0x3805, {0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f}},
    {0x3806, {0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03}},
    {0x3807, {0xcb, 0xcb, 0xcb, 0xcb, 0xcb, 0xcb, 0xcb, 0xcb, 0xcb, 0xcb}},
    {0x3808, {0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05}},
    {0x3809, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {0x380a, {0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03}},
    {0x380b, {0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0}},
    {0x380c, {0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x06, 0x06}},
    {0x380d, {0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x54, 0x54}},
    {0x380e, {0x03, 0x03, 0x04, 0x04, 0x07, 0x07, 0x09, 0x09, 0x03, 0x04}},
    {0x380f, {0xdc, 0xdc, 0xa0, 0xa0, 0xb8, 0xb8, 0x40, 0x40, 0xdc, 0xa0}},
    {0x3810, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {0x3811, {0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08}},
    {0x3812, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {0x3813, {0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04}},
    {0x3814, {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01}},
    {0x3815, {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01}},
    {0x3816, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {0x3817, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {0x3818, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {0x3819, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {0x3820, {0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80}},
    {0x3821, {0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40}},
    {0x3826, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {0x3827, {0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08}},
    {0x382a, {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01}},
    {0x382b, {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01}},
    {0x3836, {0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02}},
    {0x3838, {0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10}},
    {0x3861, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {0x3862, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {0x3863, {0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02}},
    {0x3b00, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {0x3c00, {0x89, 0x89, 0x89, 0x89, 0x89, 0x89, 0x89, 0x89, 0x89, 0x89}},
    {0x3c01, {0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab, 0xab}},
    {0x3c02, {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01}},
    {0x3c03, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {0x3c04, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {0x3c05, {0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03}},
    {0x3c06, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {0x3c07, {0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05}},
    {0x3c0c, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {0x3c0d, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {0x3c0e, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {0x3c0f, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {0x3c40, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {0x3c41, {0xa3, 0xa3, 0xa3, 0xa3, 0xa3, 0xa3, 0xa3, 0xa3, 0xa3, 0xa3}},
    {0x3c43, {0x7d, 0x7d, 0x7d, 0x7d, 0x7d, 0x7d, 0x7d, 0x7d, 0x7d, 0x7d}},
    {0x3c56, {0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80}},
    {0x3c80, {0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08}},
    {0x3c82, {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01}},
    {0x3c83, {0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61}},
    {0x3d85, {0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17}},
    {0x3f08, {0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08}},
    {0x3f0a, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {0x3f0b, {0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30}},
    {0x4000, {0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd}},
    {0x4003, {0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40}},
    {0x4009, {0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d}},
    {0x4010, {0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0}},
    {0x4011, {0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70}},
    {0x4017, {0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10}},
    {0x4040, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {0x4041, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {0x4303, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {0x4307, {0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30}},
    {0x4500, {0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30}},
    {0x4502, {0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40}},
    {0x4503, {0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06}},
    {0x4508, {0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa}},
    {0x450b, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {0x450c, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {0x4600, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {0x4601, {0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80}},
    {0x4700, {0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04}},
    {0x4704, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {0x4705, {0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04}},
    {0x4837, {0x16, 0x14, 0x16, 0x14, 0x16, 0x14, 0x16, 0x14, 0x50, 0x50}},
    {0x484a, {0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f}},
    {0x5000, {0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10}},
    {0x5001, {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01}},
    {0x5002, {0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28}},
    {0x5004, {0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c}},
    {0x5006, {0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c}},
    {0x5007, {0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0}},
    {0x5008, {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01}},
    {0x5009, {0xb0, 0xb0, 0xb0, 0xb0, 0xb0, 0xb0, 0xb0, 0xb0, 0xb0, 0xb0}},
    {0x502a, {0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18}},
    {0x5901, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {0x5a01, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {0x5a03, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {0x5a04, {0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c}},
    {0x5a05, {0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0}},
    {0x5a06, {0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09}},
    {0x5a07, {0xb0, 0xb0, 0xb0, 0xb0, 0xb0, 0xb0, 0xb0, 0xb0, 0xb0, 0xb0}},
    {0x5a08, {0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06}},
    {0x5e00, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {0x5e10, {0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc}},
    {0x300f, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {0x3733, {0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10}},
    {0x3610, {0xe8, 0xe8, 0xe8, 0xe8, 0xe8, 0xe8, 0xe8, 0xe8, 0xe8, 0xe8}},
    {0x3611, {0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56}},
    {0x3635, {0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14}},
    {0x3636, {0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13}},
    {0x3620, {0x84, 0x84, 0x84, 0x84, 0x84, 0x84, 0x84, 0x84, 0x84, 0x84}},
    {0x3614, {0x96, 0x96, 0x96, 0x96, 0x96, 0x96, 0x96, 0x96, 0x96, 0x96}},
    {0x481f, {0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30}},
    {0x3788, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {0x3789, {0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04}},
    {0x378a, {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01}},
    {0x378b, {0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60}},
    {0x3799, {0x27, 0x27, 0x27, 0x27, 0x27, 0x27, 0x27, 0x27, 0x27, 0x27}},
};

const AMBA_SENSOR_INPUT_INFO_s OV9750InputInfo[] = {
    {{  0,   0, 1280, 960}, { AMBA_SENSOR_SUBSAMPLE_TYPE_NORMAL, 1, 1}, { AMBA_SENSOR_SUBSAMPLE_TYPE_NORMAL, 1, 1}},  /* Input Mode 0 */
};

const AMBA_SENSOR_OUTPUT_INFO_s OV9750OutputInfo[] = {
    { 800000000, 1, 10, AMBA_DSP_BAYER_BG, {AMBA_DSP_PHASE_SHIFT_MODE_0, AMBA_DSP_PHASE_SHIFT_MODE_0}, 1280, 960, {   0,   0, 1280, 960}, {0}},  /* Output Mode 0 */
    { 800000000, 2, 10, AMBA_DSP_BAYER_BG, {AMBA_DSP_PHASE_SHIFT_MODE_0, AMBA_DSP_PHASE_SHIFT_MODE_0}, 1280, 960, {   0,   0, 1280, 960}, {0}},  /* Output Mode 1 */
    { 200000000, 2, 10, AMBA_DSP_BAYER_BG, {AMBA_DSP_PHASE_SHIFT_MODE_0, AMBA_DSP_PHASE_SHIFT_MODE_0}, 1280, 960, {   0,   0, 1280, 960}, {0}},  /* Output Mode 2 */
};

const OV9750_MODE_INFO_s OV9750ModeInfoList[AMBA_SENSOR_OV9750_NUM_MODE] = {
    {     OV9750_S1_10_1280_960_60P_L1, OV9750_INPUT_MODE_0, OV9750_OUTPUT_MODE_0, { 23984416, 1350/*810*/,   988, { .Interlace = 0, .TimeScale =  60000, .NumUnitsInTick = 1001 }},},  /* Mode  0 */
    {     OV9750_S1_10_1280_960_60P_L2, OV9750_INPUT_MODE_0, OV9750_OUTPUT_MODE_1, { 23984416, 2700/*810*/,   988, { .Interlace = 0, .TimeScale =  60000, .NumUnitsInTick = 1001 }},},  /* Mode  1 */
    {     OV9750_S1_10_1280_960_50P_L1, OV9750_INPUT_MODE_0, OV9750_OUTPUT_MODE_0, { 23976000, 1350/*810*/,  1184, { .Interlace = 0, .TimeScale =     50, .NumUnitsInTick =    1 }},},  /* Mode  2 */
    {     OV9750_S1_10_1280_960_50P_L2, OV9750_INPUT_MODE_0, OV9750_OUTPUT_MODE_1, { 23976000, 2700/*810*/,  1184, { .Interlace = 0, .TimeScale =     50, .NumUnitsInTick =    1 }},},  /* Mode  3 */
    { OV9750_S1_10_1280_960_60P_30P_L1, OV9750_INPUT_MODE_0, OV9750_OUTPUT_MODE_0, { 23984416, 1350/*810*/,  1976, { .Interlace = 0, .TimeScale =  30000, .NumUnitsInTick = 1001 }},},  /* Mode  4 */
    { OV9750_S1_10_1280_960_60P_30P_L2, OV9750_INPUT_MODE_0, OV9750_OUTPUT_MODE_1, { 23984416, 2700/*810*/,  1976, { .Interlace = 0, .TimeScale =  30000, .NumUnitsInTick = 1001 }},},  /* Mode  5 */
    { OV9750_S1_10_1280_960_50P_25P_L1, OV9750_INPUT_MODE_0, OV9750_OUTPUT_MODE_0, { 23976000, 1350/*810*/,  2368, { .Interlace = 0, .TimeScale =     25, .NumUnitsInTick =    1 }},},  /* Mode  6 */
    { OV9750_S1_10_1280_960_50P_25P_L2, OV9750_INPUT_MODE_0, OV9750_OUTPUT_MODE_1, { 23976000, 2700/*810*/,  2368, { .Interlace = 0, .TimeScale =     25, .NumUnitsInTick =    1 }},},  /* Mode  7 */
    {     OV9750_S1_10_1280_960_30P_L2, OV9750_INPUT_MODE_0, OV9750_OUTPUT_MODE_2, { 23984416, 1350/*1620*/,  988, { .Interlace = 0, .TimeScale =  30000, .NumUnitsInTick =  1001}},},  /* Mode  8 */
    {     OV9750_S1_10_1280_960_25P_L2, OV9750_INPUT_MODE_0, OV9750_OUTPUT_MODE_2, { 23984416, 1350/*1620*/, 1184, { .Interlace = 0, .TimeScale =     25, .NumUnitsInTick =     1}},},  /* Mode  9 */
};