/*
 * This file is part of libfreespace.
 *
 * Copyright (c) 2009-2012 Hillcrest Laboratories, Inc.
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

#ifndef FREESPACE_H_
#define FREESPACE_H_

#include "freespace_codecs.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Freespace(r) C API
 */

#define FREESPACE_MAX_INPUT_MESSAGE_SIZE 96
#define FREESPACE_MAX_OUTPUT_MESSAGE_SIZE 96
#define FREESPACE_MAXIMUM_DEVICE_COUNT 128
#define FREESPACE_RESERVED_ADDRESS 4

/**
 * @defgroup initialization Initialization
 *
 * This page documents the libfreespace function calls
 * for initializing and cleaning up the library.
 */

/**
 * @defgroup discovery Discovery API
 * This page documents the functions to discover the attached
 * Freespace(r) devices.
 */

/**
 * @defgroup device Device API
 *
 * This page documents the functions to access
 * Freespace(r) devices.
 */

/**
 * @defgroup synchronous Synchronous API
 *
 * This page describes the synchronous API for communicating
 * with Freespace(r) devices.
 */

/**
 * @defgroup async Asynchronous API
 *
 * This page describes the asynchronous API for communicating
 * with Freespace(r) devices.
 */

/**
 * Handle to a Freespace device.
 */
typedef int FreespaceDeviceId;

struct FreespaceDeviceInfo {
    /** The user-meaningful name for the device. */
    const char* name;

    /** Device vendor ID code */
    uint16_t vendor;

    /** Product ID */
    uint16_t product;

	/** HID message protocol verson */
	int hVer;
};

/** @ingroup discovery
 * Enumeration for the type of hotplug event.
 */
enum freespace_hotplugEvent {
    /** The specified Freespace device was inserted. */
    FREESPACE_HOTPLUG_INSERTION,
    /** The specified Freespace device was removed. */
    FREESPACE_HOTPLUG_REMOVAL
};

/** @ingroup discovery
 * Callback for getting notified when a Freespace devices are
 * added or removed from the system.
 *
 * @param event whether a device was inserted or removed
 * @param id the device that changed status
 * @param cookie the data passed to freespace_setDeviceHotplugCallback().
 */
typedef void (*freespace_hotplugCallback)(enum freespace_hotplugEvent event,
                                          FreespaceDeviceId id,
                                          void* cookie);

/** @ingroup async
 * Callback for getting notified when a USB packet has been sent.
 *
 * @param id the device that sent the message
 * @param cookie the data passed to freespace_sendAsync().
 * @param result FREESPACE_SUCCESS if the packet was sent; else error code
 */
typedef void (*freespace_sendCallback)(FreespaceDeviceId id, void* cookie, int result);

/** @ingroup async
 * Callback for received Freespace events in byte stream form.
 * Deprecated for external use.  For use with other language bindings, such as
 * Python and Java, only.
 *
 * @param id The device that generated the message
 * @param message the raw HID report
 * @param length its length
 * @param cookie the data passed to freespace_setReceiveCallback().
 * @param result FREESPACE_SUCCESS if a packet was received; else error code
 */
typedef void (*freespace_receiveCallback)(FreespaceDeviceId id,
                                          const uint8_t* message,
                                          int length,
                                          void* cookie,
                                          int result);

/** @ingroup async
 * Callback for received Freespace events in message form.
 *
 * @param id The device that generated the message
 * @param message the decoded HID message
 * @param cookie the data passed to freespace_setReceiveCallback().
 * @param result FREESPACE_SUCCESS if a packet was received; else error code
 */
typedef void (*freespace_receiveMessageCallback)(FreespaceDeviceId id,
                                                 struct freespace_message* message,
                                                 void* cookie,
                                                 int result);

/** @ingroup async
 * Callback for when file descriptors should be added to the
 * poll or select fd sets
 *
 * @param fd the file descriptor
 * @param events flag such as POLLIN or POLLOUT from poll.h
 */
typedef void (*freespace_pollfdAddedCallback)(FreespaceFileHandleType fd, short events);

/** @ingroup async
 *
 * Callback for when file descriptors should be removed from the poll
 * or select fd sets
 *
 * @param fd the file descriptor
 */
typedef void (*freespace_pollfdRemovedCallback)(FreespaceFileHandleType fd);

/** @ingroup initialization
 *
 * Initialize the Freespace library.
 *
 * @return FREESPACE_SUCCESS on success
 */
LIBFREESPACE_API int freespace_init();

/** @ingroup initialization
 *
 * Return a human readable string with the version of libfreespace
 * being used.
 *
 * @return version string
 */
LIBFREESPACE_API const char* freespace_version();

/** @ingroup initialization
 *
 * Finalize the Freespace library.
 */
LIBFREESPACE_API void freespace_exit();

/** @ingroup discovery
 *
 * Set a callback for whenever a Freespace device gets added or
 * removed from the system. This is only called when using the
 * asynchronous API functions. Synchronous API users can call
 * freespace_getDeviceList() periodically to discover device
 * insertions or freespace_perform() if they want hotplug event
 * callbacks.
 *
 * Note: The device list is only rescanned on calls to
 * freespace_getDeviceList() and freespace_perform(), so registering a
 * callback between freespace_init() and either of those calls will
 * cause the callback to be called for all existing devices.
 *
 * @param callback the hotplug callback or NULL to stop callbacks.
 * @param cookie user data
 * @return FREESPACE_SUCCESS or an error
 */
LIBFREESPACE_API int freespace_setDeviceHotplugCallback(freespace_hotplugCallback callback,
                                                        void* cookie);

/** @ingroup discovery
 *
 * Get the list of attached devices.
 *
 * @param list where to store the list of inserted devices
 * @param listSize the max number of ids that the list can hold
 * @param listSizeOut the number of ids returned in the list
 * @return FREESPACE_SUCCESS if no errors.
 */
LIBFREESPACE_API int freespace_getDeviceList(FreespaceDeviceId* list,
                                             int listSize,
                                             int* listSizeOut);

/** @ingroup device
 *
 * Return information about the Freespace device.
 *
 * @param id which device
 * @param info where to store the information
 * @return FREESPACE_SUCCESS if no errors.
 */
LIBFREESPACE_API int freespace_getDeviceInfo(FreespaceDeviceId id,
                                             struct FreespaceDeviceInfo* info);

/** @ingroup device
 *
 * Return determine whether the device supports new messages.
 *
 * @param id which device
 * @return FREESPACE_SUCCESS for new devices or FREESPACE_ERROR_NO_DEVICE for old device.
 */
LIBFREESPACE_API int freespace_isNewDevice(FreespaceDeviceId id);

/** @ingroup device
 *
 * Open a Freespace device for use. A device must be in the opened
 * state before most other commands in this API can be called. This
 * internally allocates the resources needed to communicate with the
 * device. freespace_closeDevice frees those resources.
 *
 * @param id The FreespaceDeviceID of an attached device to open
 * @return FREESPACE_SUCCESS if no errors
 */
LIBFREESPACE_API int freespace_openDevice(FreespaceDeviceId id);

/** @ingroup synchronous
 *
 * Send a message to the specified Freespace device synchronously.
 * Deprecated for external use.  For use with other language bindings, such as
 * Python and Java, only.
 *
 * @param id the FreespaceDeviceId of the device to send message to
 * @param message the message to send
 * @param length the length of the message
 * @return FREESPACE_SUCCESS or an error
 */
LIBFREESPACE_API int freespace_private_send(FreespaceDeviceId id,
                                            const uint8_t* message,
                                            int length);

/** @ingroup synchronous
 *
 * Send a message to the specified Freespace device synchronously.
 *
 * @param id the FreespaceDeviceId of the device to send message to
 * @param message the message to send
 * @return FREESPACE_SUCCESS or an error
 */
LIBFREESPACE_API int freespace_sendMessage(FreespaceDeviceId id,
                                           struct freespace_message* message);

/** @ingroup synchronous
 *
 * Read a message from the specified device.  This function blocks
 * until a message is received, there's a timeout or an error.
 * Deprecated for external use.  For use with other language bindings, such as
 * Python and Java, only.
 *
 * @param id the FreespaceDeviceId of the device to read from
 * @param message where to put the received message
 * @param maxLength the max length of the message
 * @param timeoutMs the timeout in milliseconds or 0 to wait forever
 * @param actualLength the number of bytes received
 * @return FREESPACE_SUCCESS or an error
 */
LIBFREESPACE_API int freespace_private_read(FreespaceDeviceId id,
                                            uint8_t* message,
                                            int maxLength,
                                            unsigned int timeoutMs,
                                            int* actualLength);

/** @ingroup synchronous
 *
 * Read a message struct from the specified device.  This function blocks
 * until a message is received, there's a timeout or an error.
 *
 * @param id the FreespaceDeviceId of the device to read from
 * @param message where to put the received message
 * @param timeoutMs the timeout in milliseconds or 0 to wait forever
 * @return FREESPACE_SUCCESS or an error
 */
LIBFREESPACE_API int freespace_readMessage(FreespaceDeviceId id,
                                           struct freespace_message* message,
                                           unsigned int timeoutMs);

/** @ingroup synchronous
 *
 * Flush all of the messages out of any receive queues.  libfreespace
 * doesn't have a receive queue, but some of the lower levels do and
 * messages can queue up.  If enough queue up, messages can be
 * dropped.  This is only a problem for the first messages assuming
 * that the application regularly calls freespace_read or has a
 * receive callback.
 *
 * @param id the FreespaceDeviceId of the device whose messages should be flushed
 * @return FREESPACE_SUCCESS or an error
 */
LIBFREESPACE_API int freespace_flush(FreespaceDeviceId id);

/** @ingroup async
 *
 * Register a callback function to handle received HID messages.
 * Deprecated for external use.  For use with other language bindings, such as
 * Python and Java, only.
 *
 * @param id the FreespaceDeviceId of the device
 * @param callback the callback function
 * @param cookie any user data
 * @return FREESPACE_SUCCESS or an error
 */
LIBFREESPACE_API int freespace_private_setReceiveCallback(FreespaceDeviceId id,
                                                          freespace_receiveCallback callback,
                                                          void* cookie);

/** @ingroup async
 *
 * Register a callback function to handle decoded received HID messages.
 *
 * @param id the FreespaceDeviceId of the device
 * @param callback the callback function
 * @param cookie any user data
 * @return FREESPACE_SUCCESS or an error
 */
LIBFREESPACE_API int freespace_setReceiveMessageCallback(FreespaceDeviceId id,
                                                         freespace_receiveMessageCallback callback,
                                                         void* cookie);

/** @ingroup async
 *
 * Send a message to the specified Freespace device, but do not block.
 * Deprecated for external use.  For use with other language bindings, such as
 * Python and Java, only.
 *
 * @param id the FreespaceDeviceId of the device to send message to
 * @param message the HID message to send
 * @param length the length of the message
 * @param timeoutMs the number of milliseconds to wait before timing out
 * @param callback the function to call when the send completes
 * @param cookie data passed to the callback function
 * @return FREESPACE_SUCCESS or an error
 */
LIBFREESPACE_API int freespace_private_sendAsync(FreespaceDeviceId id,
                                                 const uint8_t* message,
                                                 int length,
                                                 unsigned int timeoutMs,
                                                 freespace_sendCallback callback,
                                                 void* cookie);

/** @ingroup async
 *
 * Send a message struct to the specified Freespace device, but do not block.
 *
 * @param id the FreespaceDeviceId of the device to send message to
 * @param message the HID message struct to send
 * @param timeoutMs the number of milliseconds to wait before timing out
 * @param callback the function to call when the send completes
 * @param cookie data passed to the callback function
 * @return FREESPACE_SUCCESS or an error
 */
LIBFREESPACE_API int freespace_sendMessageAsync(FreespaceDeviceId id,
                                                struct freespace_message* message,
                                                unsigned int timeoutMs,
                                                freespace_sendCallback callback,
                                                void* cookie);

/** @ingroup async
 *
 * Get the next timeout for a call to select or poll.
 *
 * @param timeoutMsOut where to store the timeout (<0 is infinite)
 * @return FREESPACE_SUCCESS or an error
 */
LIBFREESPACE_API int freespace_getNextTimeout(int* timeoutMsOut);

/** @ingroup async
 *
 * After select or poll returns, call this to allow libfreespace to
 * service all of its active file descriptors.
 *
 * @return FREESPACE_SUCCESS or an error
 */
LIBFREESPACE_API int freespace_perform();

/** @ingroup async
 *
 * Set callback functions for when file descriptors need to be added
 * or removed from the select/poll list.
 *
 * @param addedCallback the function to call when descriptors are added
 * @param removedCallback the function to call when descriptors are removed
 */
LIBFREESPACE_API void freespace_setFileDescriptorCallbacks(freespace_pollfdAddedCallback addedCallback,
                                                           freespace_pollfdRemovedCallback removedCallback);

/** @ingroup async
 *
 * Call the file descriptor added callback function for each internal
 * file descriptor that is in use. This should be called after
 * freespace_setFileDescriptorCallbacks so that the app can get in
 * sync with the current active set.
 *
 * @return FREESPACE_SUCCESS on success
 */
LIBFREESPACE_API int freespace_syncFileDescriptors();

/** @ingroup device
 *
 * Close a Freespace device.
 *
 * @param id the Freespace device id to close
 */
LIBFREESPACE_API void freespace_closeDevice(FreespaceDeviceId id);

#ifdef __cplusplus
}
#endif

#endif /* FREESPACE_H_ */
