//-----------------------------------------------------------------------------
// Generic CANFD Message
// by Pierre Molinaro
//
// https://github.com/pierremolinaro/CAN2518FD
//
//-----------------------------------------------------------------------------

#ifndef GENERIC_CANFD_MESSAGE_DEFINED
#define GENERIC_CANFD_MESSAGE_DEFINED

//-----------------------------------------------------------------------------

#include <CANMessage.h>
#include <furi.h>

//-----------------------------------------------------------------------------
//    CANFDMessage class
//-----------------------------------------------------------------------------
// Note that "len" field contains the actual length, not its encoding in CANFD frames
// Valid values are: 0, 1, ..., 8, 12, 16, 20, 24, 32, 48, 64.
// Having other values is an error that prevents frame to be sent by tryToSend
// You can use the "pad" method for padding with 0 bytes to the next valid length


typedef enum {
    CAN_REMOTE,
    CAN_DATA,
    CANFD_NO_BIT_RATE_SWITCH,
    CANFD_WITH_BIT_RATE_SWITCH
} Type;

typedef struct {
    uint32_t id;  // Frame identifier
    bool ext; // false -> base frame, true -> extended frame
    Type type;
    uint8_t idx;  // This field is used by the driver
    uint8_t len;  // Length of data (0 ... 64)
    union {
        uint64_t data64[8]; // Caution: subject to endianness
        int64_t  data_s64[8]; // Caution: subject to endianness
        uint32_t data32[16]; // Caution: subject to endianness
        int32_t  data_s32[16]; // Caution: subject to endianness
        float    dataFloat[16]; // Caution: subject to endianness
        uint16_t data16[32]; // Caution: subject to endianness
        int16_t  data_s16[32]; // Caution: subject to endianness
        int8_t   data_s8[64];
        uint8_t  data[64];
    };
} CANFDMessage;


CANFDMessage instantiateEmptyMessage(void) :
    id(0),  // Frame identifier
    ext(false), // false -> base frame, true -> extended frame
    type(CANFD_WITH_BIT_RATE_SWITCH),
    idx(0),  // This field is used by the driver
    len(0), // Length of data (0 ... 64)
    data() {
}


CANFDMessage(const CANMessage& inMessage) :
    id(inMessage.id),  // Frame identifier
    ext(inMessage.ext), // false -> base frame, true -> extended frame
    type(inMessage.rtr ? CAN_REMOTE : CAN_DATA),
    idx(inMessage.idx),  // This field is used by the driver
    len(inMessage.len), // Length of data (0 ... 64)
    data() {
    data64[0] = inMessage.data64;
}




void pad(CANFDMessage* message);
bool isValid(CANFDMessage* message);

//-----------------------------------------------------------------------------

typedef void (*ACANFDCallBackRoutine) (const CANFDMessage& inMessage);

//-----------------------------------------------------------------------------

#endif
