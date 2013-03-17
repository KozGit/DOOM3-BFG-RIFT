/*
 * This file is part of libfreespace.
 *
 * Copyright (c) 2010-2012 Hillcrest Laboratories, Inc.
 *
 * libfreespace is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */
 
#ifndef FREESPACE_DEVICE_TABLE_H_
#define FREESPACE_DEVICE_TABLE_H_


#ifdef _WIN32
#include "win32_stdint.h"
#include <windows.h>

#if _MSC_VER >= 1600
// With VC2010 and the WinDDK 7600, setting the include path to be have the 
// WinDDK inc/api directory causes build failure.  
// The workaround is to only have the /inc directory in the path, and add 
// the api prefix to hidsdi.h.  
#include <api/hidsdi.h>
#else
#include <hidsdi.h>
#endif


#else
#include <stdint.h>
#define USAGE uint16_t
#endif

#include "freespace/freespace.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
#endif

/**
 * Define the maximum number of handles (interfaces) per device that
 * can be joined together as a single virtual device.
 * Changing this value requires changing instances of freespace_deviceAPI.
 * This may be Windows-specific.
 */
#define FREESPACE_HANDLE_COUNT_MAX 2

/**
 * @ingroup device
 * A specific usage available for a given freespace_deviceAPI.
 */
struct FreespaceDeviceUsageAPI {
    USAGE    usage_;
    USAGE    usagePage_;
};

/**
 * @ingroup device
 * Figure out which API to use depending on the reported Freespace version.
 */
struct FreespaceDeviceAPI {
    // Set of devices with this version.
    uint16_t    idVendor_;
    uint16_t    idProduct_;
    int         controlInterfaceNumber_;
    int         usageCount_;
	int         hVer_; // HID messaging protocol used for a particular product ID
    struct      FreespaceDeviceUsageAPI usages_[FREESPACE_HANDLE_COUNT_MAX];
    const char* name_;
};

/**
 * @ingroup device
 * Stores information for all Freespace devices.
 */
extern const struct FreespaceDeviceAPI freespace_deviceAPITable[];

/**
 * Defines the number of devices in the API table.  This can't be done as a sizeof the array
 * because the externed definition of the array is not dimensioned.
 */
extern const int freespace_deviceAPITableNum;

/**
 * @ingroup device
 * Stores product IDs for all Freespace devices that support the new API.
 */
extern const struct FreespaceDeviceInfo freespace_newDeviceAPITable[];

/**
 * Defines the number of devices in the API table.  This can't be done as a sizeof the array
 * because the externed definition of the array is not dimensioned.
 */
extern const int freespace_newDeviceAPITableNum;

#ifdef __cplusplus
}
#endif

#endif // FREESPACE_DEVICE_TABLE_H_

