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

#ifndef FREESPACE_PRINTERS_H_
#define FREESPACE_PRINTERS_H_

#include "freespace/freespace_common.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "freespace_codecs.h"
#include <stdio.h>


/**
 * Pretty print a Freespace message to the terminal.
 *
 * @param fp the file pointer to print into
 * @param s the HID message
 */

LIBFREESPACE_API void freespace_printMessage(FILE* fp, struct freespace_message* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printPairingMessageStr(char* dest, int maxlen, const struct freespace_PairingMessage* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printPairingMessage(FILE* fp, const struct freespace_PairingMessage* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printProductIDRequestStr(char* dest, int maxlen, const struct freespace_ProductIDRequest* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printProductIDRequest(FILE* fp, const struct freespace_ProductIDRequest* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printLEDSetRequestStr(char* dest, int maxlen, const struct freespace_LEDSetRequest* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printLEDSetRequest(FILE* fp, const struct freespace_LEDSetRequest* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printLinkQualityRequestStr(char* dest, int maxlen, const struct freespace_LinkQualityRequest* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printLinkQualityRequest(FILE* fp, const struct freespace_LinkQualityRequest* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printAlwaysOnRequestStr(char* dest, int maxlen, const struct freespace_AlwaysOnRequest* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printAlwaysOnRequest(FILE* fp, const struct freespace_AlwaysOnRequest* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printFrequencyFixRequestStr(char* dest, int maxlen, const struct freespace_FrequencyFixRequest* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printFrequencyFixRequest(FILE* fp, const struct freespace_FrequencyFixRequest* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printSoftwareResetMessageStr(char* dest, int maxlen, const struct freespace_SoftwareResetMessage* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printSoftwareResetMessage(FILE* fp, const struct freespace_SoftwareResetMessage* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printDongleRFDisableMessageStr(char* dest, int maxlen, const struct freespace_DongleRFDisableMessage* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printDongleRFDisableMessage(FILE* fp, const struct freespace_DongleRFDisableMessage* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printTxDisableMessageStr(char* dest, int maxlen, const struct freespace_TxDisableMessage* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printTxDisableMessage(FILE* fp, const struct freespace_TxDisableMessage* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printDongleRFSupressHomeFrequencyMessageStr(char* dest, int maxlen, const struct freespace_DongleRFSupressHomeFrequencyMessage* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printDongleRFSupressHomeFrequencyMessage(FILE* fp, const struct freespace_DongleRFSupressHomeFrequencyMessage* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printFRSLoopReadRequestStr(char* dest, int maxlen, const struct freespace_FRSLoopReadRequest* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printFRSLoopReadRequest(FILE* fp, const struct freespace_FRSLoopReadRequest* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printFRSLoopWriteRequestStr(char* dest, int maxlen, const struct freespace_FRSLoopWriteRequest* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printFRSLoopWriteRequest(FILE* fp, const struct freespace_FRSLoopWriteRequest* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printFRSLoopWriteDataStr(char* dest, int maxlen, const struct freespace_FRSLoopWriteData* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printFRSLoopWriteData(FILE* fp, const struct freespace_FRSLoopWriteData* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printFRSDongleReadRequestStr(char* dest, int maxlen, const struct freespace_FRSDongleReadRequest* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printFRSDongleReadRequest(FILE* fp, const struct freespace_FRSDongleReadRequest* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printFRSDongleWriteRequestStr(char* dest, int maxlen, const struct freespace_FRSDongleWriteRequest* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printFRSDongleWriteRequest(FILE* fp, const struct freespace_FRSDongleWriteRequest* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printFRSDongleWriteDataStr(char* dest, int maxlen, const struct freespace_FRSDongleWriteData* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printFRSDongleWriteData(FILE* fp, const struct freespace_FRSDongleWriteData* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printFRSEFlashReadRequestStr(char* dest, int maxlen, const struct freespace_FRSEFlashReadRequest* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printFRSEFlashReadRequest(FILE* fp, const struct freespace_FRSEFlashReadRequest* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printFRSEFlashWriteRequestStr(char* dest, int maxlen, const struct freespace_FRSEFlashWriteRequest* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printFRSEFlashWriteRequest(FILE* fp, const struct freespace_FRSEFlashWriteRequest* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printFRSEFlashWriteDataStr(char* dest, int maxlen, const struct freespace_FRSEFlashWriteData* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printFRSEFlashWriteData(FILE* fp, const struct freespace_FRSEFlashWriteData* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printDongleRFEnableMessageStr(char* dest, int maxlen, const struct freespace_DongleRFEnableMessage* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printDongleRFEnableMessage(FILE* fp, const struct freespace_DongleRFEnableMessage* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printDataModeRequestStr(char* dest, int maxlen, const struct freespace_DataModeRequest* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printDataModeRequest(FILE* fp, const struct freespace_DataModeRequest* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printButtonTestModeRequestStr(char* dest, int maxlen, const struct freespace_ButtonTestModeRequest* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printButtonTestModeRequest(FILE* fp, const struct freespace_ButtonTestModeRequest* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printPairingResponseStr(char* dest, int maxlen, const struct freespace_PairingResponse* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printPairingResponse(FILE* fp, const struct freespace_PairingResponse* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printProductIDResponseStr(char* dest, int maxlen, const struct freespace_ProductIDResponse* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printProductIDResponse(FILE* fp, const struct freespace_ProductIDResponse* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printLinkStatusStr(char* dest, int maxlen, const struct freespace_LinkStatus* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printLinkStatus(FILE* fp, const struct freespace_LinkStatus* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printAlwaysOnResponseStr(char* dest, int maxlen, const struct freespace_AlwaysOnResponse* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printAlwaysOnResponse(FILE* fp, const struct freespace_AlwaysOnResponse* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printFRSLoopReadResponseStr(char* dest, int maxlen, const struct freespace_FRSLoopReadResponse* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printFRSLoopReadResponse(FILE* fp, const struct freespace_FRSLoopReadResponse* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printFRSLoopWriteResponseStr(char* dest, int maxlen, const struct freespace_FRSLoopWriteResponse* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printFRSLoopWriteResponse(FILE* fp, const struct freespace_FRSLoopWriteResponse* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printFRSDongleReadResponseStr(char* dest, int maxlen, const struct freespace_FRSDongleReadResponse* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printFRSDongleReadResponse(FILE* fp, const struct freespace_FRSDongleReadResponse* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printFRSDongleWriteResponseStr(char* dest, int maxlen, const struct freespace_FRSDongleWriteResponse* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printFRSDongleWriteResponse(FILE* fp, const struct freespace_FRSDongleWriteResponse* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printFRSEFlashReadResponseStr(char* dest, int maxlen, const struct freespace_FRSEFlashReadResponse* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printFRSEFlashReadResponse(FILE* fp, const struct freespace_FRSEFlashReadResponse* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printFRSEFlashWriteResponseStr(char* dest, int maxlen, const struct freespace_FRSEFlashWriteResponse* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printFRSEFlashWriteResponse(FILE* fp, const struct freespace_FRSEFlashWriteResponse* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printDataModeResponseStr(char* dest, int maxlen, const struct freespace_DataModeResponse* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printDataModeResponse(FILE* fp, const struct freespace_DataModeResponse* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printButtonTestModeResponseStr(char* dest, int maxlen, const struct freespace_ButtonTestModeResponse* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printButtonTestModeResponse(FILE* fp, const struct freespace_ButtonTestModeResponse* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printBatteryLevelRequestStr(char* dest, int maxlen, const struct freespace_BatteryLevelRequest* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printBatteryLevelRequest(FILE* fp, const struct freespace_BatteryLevelRequest* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printBatteryLevelStr(char* dest, int maxlen, const struct freespace_BatteryLevel* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printBatteryLevel(FILE* fp, const struct freespace_BatteryLevel* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printBodyFrameStr(char* dest, int maxlen, const struct freespace_BodyFrame* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printBodyFrame(FILE* fp, const struct freespace_BodyFrame* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printUserFrameStr(char* dest, int maxlen, const struct freespace_UserFrame* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printUserFrame(FILE* fp, const struct freespace_UserFrame* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printDataMotionControlStr(char* dest, int maxlen, const struct freespace_DataMotionControl* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printDataMotionControl(FILE* fp, const struct freespace_DataMotionControl* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printFRSWriteResponseStr(char* dest, int maxlen, const struct freespace_FRSWriteResponse* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printFRSWriteResponse(FILE* fp, const struct freespace_FRSWriteResponse* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printDataModeControlV2ResponseStr(char* dest, int maxlen, const struct freespace_DataModeControlV2Response* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printDataModeControlV2Response(FILE* fp, const struct freespace_DataModeControlV2Response* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printFRSReadResponseStr(char* dest, int maxlen, const struct freespace_FRSReadResponse* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printFRSReadResponse(FILE* fp, const struct freespace_FRSReadResponse* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printPerResponseStr(char* dest, int maxlen, const struct freespace_PerResponse* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printPerResponse(FILE* fp, const struct freespace_PerResponse* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printFRSWriteRequestStr(char* dest, int maxlen, const struct freespace_FRSWriteRequest* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printFRSWriteRequest(FILE* fp, const struct freespace_FRSWriteRequest* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printFRSWriteDataStr(char* dest, int maxlen, const struct freespace_FRSWriteData* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printFRSWriteData(FILE* fp, const struct freespace_FRSWriteData* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printFRSReadRequestStr(char* dest, int maxlen, const struct freespace_FRSReadRequest* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printFRSReadRequest(FILE* fp, const struct freespace_FRSReadRequest* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printPerRequestStr(char* dest, int maxlen, const struct freespace_PerRequest* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printPerRequest(FILE* fp, const struct freespace_PerRequest* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printActivityClassificationNotificationStr(char* dest, int maxlen, const struct freespace_ActivityClassificationNotification* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printActivityClassificationNotification(FILE* fp, const struct freespace_ActivityClassificationNotification* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printDataModeControlV2RequestStr(char* dest, int maxlen, const struct freespace_DataModeControlV2Request* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printDataModeControlV2Request(FILE* fp, const struct freespace_DataModeControlV2Request* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printReorientationRequestStr(char* dest, int maxlen, const struct freespace_ReorientationRequest* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printReorientationRequest(FILE* fp, const struct freespace_ReorientationRequest* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printBodyUserFrameStr(char* dest, int maxlen, const struct freespace_BodyUserFrame* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printBodyUserFrame(FILE* fp, const struct freespace_BodyUserFrame* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printMotionEngineOutputStr(char* dest, int maxlen, const struct freespace_MotionEngineOutput* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printMotionEngineOutput(FILE* fp, const struct freespace_MotionEngineOutput* s);


/**
 * Print message struct to string dest, with maximum length maxlen.
 * @param dest the destination string
 * @param maxlen the length of the passed in string
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printDceOutV2Str(char* dest, int maxlen, const struct freespace_DceOutV2* s);
/**
 * Print message to a file pointer.
 * @param fp the destination file pointer
 * @param s the struct to print
 * @return the number of characters actually printed, or an error if it tries to print more than maxlen
 */
LIBFREESPACE_API int freespace_printDceOutV2(FILE* fp, const struct freespace_DceOutV2* s);

#ifdef __cplusplus
}
#endif

#endif /* FREESPACE_PRINTERS_H_ */
