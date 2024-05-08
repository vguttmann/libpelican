#include <furi.h>


bool isValid(CANFDMessage* message) {
    if ((message->type == CAN_REMOTE) || (message->type == CAN_DATA)) { // Remote frame
        return message->len <= 8;
    }
    else { // Data frame
        return
            (message->len <= 8) || (message->len == 12) || (message->len == 16) || (message->len == 20)
            ||
            (message->len == 24) || (message->len == 32) || (message->len == 48) || (message->len == 64)
            ;
    }
}

void pad(CANFDMessage* message) {
    uint8_t paddedLength = len;
    if ((message->len > 8) && (message->len < 12)) {
        paddedLength = 12;
    }
    else if ((message->len > 12) && (message->len < 16)) {
        paddedLength = 16;
    }
    else if ((message->len > 16) && (message->len < 20)) {
        paddedLength = 20;
    }
    else if ((message->len > 20) && (message->len < 24)) {
        paddedLength = 24;
    }
    else if ((message->len > 24) && (message->len < 32)) {
        paddedLength = 32;
    }
    else if ((message->len > 32) && (message->len < 48)) {
        paddedLength = 48;
    }
    else if ((message->len > 48) && (message->len < 64)) {
        paddedLength = 64;
    }
    while (message->len < paddedLength) {
        data[message->len] = 0;
        message->len += 1;
    }
}
