//----------------------------------------------------------------------------------------------------------------------
// Generic CAN Message
// by Pierre Molinaro
//
// This file is common to the following libraries
// https://github.com/pierremolinaro/acan
// https://github.com/pierremolinaro/acan2515
// https://github.com/pierremolinaro/CAN2518
// https://github.com/pierremolinaro/CAN2518FD
//
//----------------------------------------------------------------------------------------------------------------------

#ifndef GENERIC_CAN_MESSAGE_DEFINED
#define GENERIC_CAN_MESSAGE_DEFINED

//----------------------------------------------------------------------------------------------------------------------

#include <furi.h>

//----------------------------------------------------------------------------------------------------------------------

typedef struct {
    uint32_t id = 0;  // Frame identifier
    bool ext = false; // false -> standard frame, true -> extended frame
    bool rtr = false; // false -> data frame, true -> remote frame
    uint8_t idx = 0;  // This field is used by the driver
    uint8_t len = 0;  // Length of data (0 ... 8)
    union {
        uint64_t data64; // Caution: subject to endianness
        int64_t  data_s64; // Caution: subject to endianness
        uint32_t data32[2]; // Caution: subject to endianness
        int32_t  data_s32[2]; // Caution: subject to endianness
        float    dataFloat[2]; // Caution: subject to endianness
        uint16_t data16[4]; // Caution: subject to endianness
        int16_t  data_s16[4]; // Caution: subject to endianness
        int8_t   data_s8[8];
        uint8_t  data[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    };
} CANMessage;

//----------------------------------------------------------------------------------------------------------------------

typedef enum { kStandard, kExtended } tFrameFormat;
typedef enum { kData, kRemote } tFrameKind;
typedef void (*ACANCallBackRoutine) (const CANMessage& inMessage);

//----------------------------------------------------------------------------------------------------------------------

#endif
