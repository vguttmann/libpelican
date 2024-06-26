//----------------------------------------------------------------------------------------------------------------------
// A CAN driver for MCP2517FD (CANFD mode)
// by Pierre Molinaro
// https://github.com/pierremolinaro/CAN2518FD
//
//----------------------------------------------------------------------------------------------------------------------

#pragma once

//----------------------------------------------------------------------------------------------------------------------

#include "CANFDDataBitrateFactor.h"

//----------------------------------------------------------------------------------------------------------------------
//  CAN2518FDSettings class
//----------------------------------------------------------------------------------------------------------------------

class CAN2518FDSettings {

    //······················································································································
    //   ENUMERATED TYPES
    //······················································································································

    typedef enum {
        OSC_4MHz,
        OSC_4MHz_DIVIDED_BY_2,
        OSC_4MHz10xPLL,
        OSC_4MHz10xPLL_DIVIDED_BY_2,
        OSC_20MHz,
        OSC_20MHz_DIVIDED_BY_2,
        OSC_40MHz,
        OSC_40MHz_DIVIDED_BY_2
    } Oscillator;

    typedef enum {
        CLKO_DIVIDED_BY_1,
        CLKO_DIVIDED_BY_2,
        CLKO_DIVIDED_BY_4,
        CLKO_DIVIDED_BY_10,
        SOF
    } CLKOpin;

    typedef enum {
        NormalFD = 0,
        Sleep = 1,
        InternalLoopBack = 2,
        ListenOnly = 3,
        Configuration = 4,
        ExternalLoopBack = 5,
        Normal20B = 6,
        RestrictedOperation = 7
    } OperationMode;

    typedef enum { Disabled, ThreeAttempts, UnlimitedNumber } RetransmissionAttempts;

    typedef enum {
        PAYLOAD_8 = 0,
        PAYLOAD_12 = 1,
        PAYLOAD_16 = 2,
        PAYLOAD_20 = 3,
        PAYLOAD_24 = 4,
        PAYLOAD_32 = 5,
        PAYLOAD_48 = 6,
        PAYLOAD_64 = 7
    } PayloadSize;

    //······················································································································
    //   Deprecated enumeration (now use DataBitrateFactor declared in ACANFD_DataBitrateFactor.h)
    //······················································································································

    typedef enum {
        DATA_BITRATE_x1 = 1,
        DATA_BITRATE_x2 = 2,
        DATA_BITRATE_x3 = 3,
        DATA_BITRATE_x4 = 4,
        DATA_BITRATE_x5 = 5,
        DATA_BITRATE_x6 = 6,
        DATA_BITRATE_x7 = 7,
        DATA_BITRATE_x8 = 8,
        DATA_BITRATE_x9 = 9,
        DATA_BITRATE_x10 = 10
    } DataBitrateFactor_Deprecated;

    //······················································································································
    //   CONSTRUCTOR
    //······················································································································

    CAN2518FDSettings(const Oscillator inOscillator,
        const uint32_t inDesiredArbitrationBitRate,
        const DataBitrateFactor inDataBitrateFactor,
        const uint32_t inTolerancePPM = 1000);

    //······················································································································
    //   DEPRECATED CONSTRUCTOR (for compatibility with version < 2.1.0)
    //······················································································································

    CAN2518FDSettings(const Oscillator inOscillator,
        const uint32_t inDesiredArbitrationBitRate,
        const DataBitrateFactor_Deprecated inDataBitrateFactor,
        const uint32_t inTolerancePPM = 1000) :
        CAN2518FDSettings(inOscillator, inDesiredArbitrationBitRate, DataBitrateFactor(inDataBitrateFactor), inTolerancePPM) {
    }

    //······················································································································
    //   CAN BIT TIMING
    //······················································································································

    Oscillator mOscillator;
    uint32_t mSysClock; // In Hz
    const uint32_t mDesiredArbitrationBitRate; // In kb/s
    const DataBitrateFactor mDataBitrateFactor;
    //--- Data bit rate; if mDataBitrateFactor==1, theses properties are not used for configuring the MCP2517FD.
    uint8_t mDataSJW = 0; // if mDataBitrateFactor > 1: 1...16, else equal to mArbitrationSJW
    uint8_t mDataPhaseSegment1 = 0; // if mDataBitrateFactor > 1: 2...32, else equal to mArbitrationPhaseSegment1
    uint8_t mDataPhaseSegment2 = 0; // if mDataBitrateFactor > 1: 1...16, else equal to mArbitrationPhaseSegment2
    //--- Bit rate prescaler is common to arbitration and data bit rates
    uint16_t mBitRatePrescaler = 0; // 1...256
    //--- Arbitration bit rate
    uint16_t mArbitrationPhaseSegment1 = 0; // 2...256
    uint8_t mArbitrationPhaseSegment2 = 0; // 1...128
    uint8_t mArbitrationSJW = 0; // 1...128
    bool mArbitrationBitRateClosedToDesiredRate = false; // The above configuration is not correct
    //--- Transmitter Delay Compensation Offset
    int8_t mTDCO = 0; // -64 ... +63

    //······················································································································
    //    MCP2517FD TXCAN pin is Open Drain ?
    //······················································································································

    bool mTXCANIsOpenDrain = false; // false --> Push/Pull Output, true --> Open Drain Output

    //······················································································································
    //    MCP2517FD INT pin is Open Drain ?
    //······················································································································

    bool mINTIsOpenDrain = false; // false --> Push/Pull Output, true --> Open Drain Output

    //······················································································································
    //    ISO CRC Enable
    //······················································································································

    // false --> Do NOT include Stuff Bit Count in CRC Field and use CRC Initialization Vector with all zeros
    // true --> Include Stuff Bit Count in CRC Field and use Non-Zero CRC Initialization Vector according to ISO 11898-1:2015
    bool mISOCRCEnabled = true;

    //······················································································································
    //    CLKO pin function (default value is MCP2517FD power on setting)
    //······················································································································

    CLKOpin mCLKOPin = CLKO_DIVIDED_BY_10;

    //······················································································································
    //    Requested mode
    //······················································································································

    OperationMode mRequestedMode = NormalFD;

    //······················································································································
    //   TRANSMIT FIFO
    //······················································································································

    //--- Driver transmit buffer size
    uint16_t mDriverTransmitFIFOSize = 16; // >= 0

    //--- Controller transmit FIFO size
    uint8_t mControllerTransmitFIFOSize = 1; // 1 ... 32

    //--- Payload controller transmit FIFO size
    PayloadSize mControllerTransmitFIFOPayload = PAYLOAD_64;

    //--- Controller transmit FIFO priority (0 --> lowest, 31 --> highest)
    uint8_t mControllerTransmitFIFOPriority = 0; // 0 ... 31

    //--- Controller transmit FIFO retransmission attempts
    RetransmissionAttempts mControllerTransmitFIFORetransmissionAttempts = UnlimitedNumber;

    //······················································································································
    //   TXQ BUFFER
    //······················································································································

    //--- TXQ buffer size (0 --> TXQ disabled)
    uint8_t mControllerTXQSize = 0; // 0 ... 32

    //--- Payload controller TXQ buffer size
    PayloadSize mControllerTXQBufferPayload = PAYLOAD_64;

    //--- TXQ buffer priority (0 --> lowest, 31 --> highest)
    uint8_t mControllerTXQBufferPriority = 31; // 0 ... 31

    //--- Controller TXQ buffer retransmission attempts
    RetransmissionAttempts mControllerTXQBufferRetransmissionAttempts = UnlimitedNumber;


    //······················································································································
    //   RECEIVE FIFO
    //······················································································································

    //--- Driver receive buffer size
    uint16_t mDriverReceiveFIFOSize = 32; // > 0

    //--- Payload receive FIFO size
    PayloadSize mControllerReceiveFIFOPayload = PAYLOAD_64;

    //--- Controller receive FIFO size
    uint8_t mControllerReceiveFIFOSize = 27; // 1 ... 32

    //······················································································································
    //    SYSCLOCK frequency computation
    //······················································································································

    uint32_t sysClock(const Oscillator inOscillator);

    //······················································································································
    //    Accessors
    //······················································································································

    Oscillator oscillator(void) const { return mOscillator; }
    uint32_t sysClock(void) const { return mSysClock; }
    uint32_t actualArbitrationBitRate(void) const;
    uint32_t actualDataBitRate(void) const;
    bool exactArbitrationBitRate(void) const;
    bool exactDataBitRate(void) const;
    bool dataBitRateIsAMultipleOfArbitrationBitRate(void) const;

    //······················································································································
    //    RAM USAGE
    //······················································································································

    uint32_t ramUsage(void) const;

    uint32_t objectSizeForPayload(const PayloadSize inPayload);

    //······················································································································
    //    Distance between actual bit rate and requested bit rate (in ppm, part-per-million)
    //······················································································································

    uint32_t ppmFromDesiredArbitrationBitRate(void) const;
    uint32_t ppmFromDesiredDataBitRate(void) const;

    //······················································································································
    //    Distance of sample point from bit start (in ppc, part-per-cent, denoted by %)
    //······················································································································

    uint32_t arbitrationSamplePointFromBitStart(void) const;
    uint32_t dataSamplePointFromBitStart(void) const;

    //······················································································································
    //    Bit settings are consistent ? (returns 0 if ok)
    //······················································································································

    uint32_t CANBitSettingConsistency(void) const;

    //······················································································································
    //    Constants returned by CANBitSettingConsistency
    //······················································································································

    const uint32_t kBitRatePrescalerIsZero = ((uint32_t)1) << 0;
    const uint32_t kBitRatePrescalerIsGreaterThan256 = ((uint32_t)1) << 1;
    const uint32_t kArbitrationPhaseSegment1IsLowerThan2 = ((uint32_t)1) << 2;
    const uint32_t kArbitrationPhaseSegment1IsGreaterThan256 = ((uint32_t)1) << 3;
    const uint32_t kArbitrationPhaseSegment2IsZero = ((uint32_t)1) << 4;
    const uint32_t kArbitrationPhaseSegment2IsGreaterThan128 = ((uint32_t)1) << 5;
    const uint32_t kArbitrationSJWIsZero = ((uint32_t)1) << 6;
    const uint32_t kArbitrationSJWIsGreaterThan128 = ((uint32_t)1) << 7;
    const uint32_t kArbitrationSJWIsGreaterThanPhaseSegment1 = ((uint32_t)1) << 8;
    const uint32_t kArbitrationSJWIsGreaterThanPhaseSegment2 = ((uint32_t)1) << 9;
    const uint32_t kArbitrationTQCountNotDivisibleByDataBitrateFactor = ((uint32_t)1) << 10;
    const uint32_t kDataPhaseSegment1IsLowerThan2 = ((uint32_t)1) << 11;
    const uint32_t kDataPhaseSegment1IsGreaterThan32 = ((uint32_t)1) << 12;
    const uint32_t kDataPhaseSegment2IsZero = ((uint32_t)1) << 13;
    const uint32_t kDataPhaseSegment2IsGreaterThan16 = ((uint32_t)1) << 14;
    const uint32_t kDataSJWIsZero = ((uint32_t)1) << 15;
    const uint32_t kDataSJWIsGreaterThan16 = ((uint32_t)1) << 16;
    const uint32_t kDataSJWIsGreaterThanPhaseSegment1 = ((uint32_t)1) << 17;
    const uint32_t kDataSJWIsGreaterThanPhaseSegment2 = ((uint32_t)1) << 18;

    //······················································································································
    // Max values
    //······················································································································

    const uint16_t MAX_BRP = 256;
    const uint16_t MAX_ARBITRATION_PHASE_SEGMENT_1 = 256;
    const uint8_t  MAX_ARBITRATION_PHASE_SEGMENT_2 = 128;
    const uint8_t  MAX_ARBITRATION_SJW = 128;
    const uint16_t MAX_DATA_PHASE_SEGMENT_1 = 32;
    const uint8_t  MAX_DATA_PHASE_SEGMENT_2 = 16;
    const uint8_t  MAX_DATA_SJW = 16;

    //······················································································································

};

//----------------------------------------------------------------------------------------------------------------------
