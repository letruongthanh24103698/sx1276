/*
 * SCI_SX1276.h
 *
 *  Created on: May 15, 2020
 *      Author: Le Truong Thanh
 *      Comment: DO NOT EDIT THIS FILE
 *      First version for RAK4200
 */
#ifndef SRC_SCI_SX1276_H_
#define SRC_SCI_SX1276_H_

#include "main.h"
#include <stdbool.h>
#include "HCI_SX1276.h"
#include "sx1276Regs-LoRa.h"
#include "sx1276Regs-Fsk.h"
/*****************************************************************************************************************/
/*													MACROS DEFINITION											 */
/*****************************************************************************************************************/
#define SX1276_print 		printf

#define RADIO_INIT_REGISTERS_VALUE                \
{                                                 \
    { MODEM_FSK , REG_LNA                , 0x23 },\
    { MODEM_FSK , REG_RXCONFIG           , 0x1E },\
    { MODEM_FSK , REG_RSSICONFIG         , 0xD2 },\
    { MODEM_FSK , REG_AFCFEI             , 0x01 },\
    { MODEM_FSK , REG_PREAMBLEDETECT     , 0xAA },\
    { MODEM_FSK , REG_OSC                , 0x07 },\
    { MODEM_FSK , REG_SYNCCONFIG         , 0x12 },\
    { MODEM_FSK , REG_SYNCVALUE1         , 0xC1 },\
    { MODEM_FSK , REG_SYNCVALUE2         , 0x94 },\
    { MODEM_FSK , REG_SYNCVALUE3         , 0xC1 },\
    { MODEM_FSK , REG_PACKETCONFIG1      , 0xD8 },\
    { MODEM_FSK , REG_FIFOTHRESH         , 0x8F },\
    { MODEM_FSK , REG_IMAGECAL           , 0x02 },\
    { MODEM_FSK , REG_DIOMAPPING1        , 0x00 },\
    { MODEM_FSK , REG_DIOMAPPING2        , 0x30 },\
    { MODEM_LORA, REG_LR_PAYLOADMAXLENGTH, 0x40 },\
}

#define RADIO_INIT_REGISTERS_VALUE                \
{                                                 \
    { MODEM_FSK , REG_LNA                , 0x23 },\
    { MODEM_FSK , REG_RXCONFIG           , 0x1E },\
    { MODEM_FSK , REG_RSSICONFIG         , 0xD2 },\
    { MODEM_FSK , REG_AFCFEI             , 0x01 },\
    { MODEM_FSK , REG_PREAMBLEDETECT     , 0xAA },\
    { MODEM_FSK , REG_OSC                , 0x07 },\
    { MODEM_FSK , REG_SYNCCONFIG         , 0x12 },\
    { MODEM_FSK , REG_SYNCVALUE1         , 0xC1 },\
    { MODEM_FSK , REG_SYNCVALUE2         , 0x94 },\
    { MODEM_FSK , REG_SYNCVALUE3         , 0xC1 },\
    { MODEM_FSK , REG_PACKETCONFIG1      , 0xD8 },\
    { MODEM_FSK , REG_FIFOTHRESH         , 0x8F },\
    { MODEM_FSK , REG_IMAGECAL           , 0x02 },\
    { MODEM_FSK , REG_DIOMAPPING1        , 0x00 },\
    { MODEM_FSK , REG_DIOMAPPING2        , 0x30 },\
    { MODEM_LORA, REG_LR_PAYLOADMAXLENGTH, 0x40 },\
}

#define XTAL_FREQ                                   32000000
#define FREQ_STEP                                   61.03515625

#define RX_BUFFER_SIZE                              256

/*!
 * Sync word for Private LoRa networks
 */
#define LORA_MAC_PRIVATE_SYNCWORD                   0x12

/*!
 * Sync word for Public LoRa networks
 */
#define LORA_MAC_PUBLIC_SYNCWORD                    0x34

#define RF_MID_BAND_THRESH                          525000000

#define RSSI_OFFSET_LF                              -164
#define RSSI_OFFSET_HF                              -157

#define LORA_MAC_FRMPAYLOAD_OVERHEAD                13
/*****************************************************************************************************************/
/*													   DATA-STRUCTS												 */
/*****************************************************************************************************************/
typedef enum
{
    MODEM_FSK = 0,
    MODEM_LORA,
}RadioModems_t;

typedef struct
{
    RadioModems_t Modem;
    uint8_t       Addr;
    uint8_t       Value;
}RadioRegisters_t;

typedef enum
{
	SX1276_NORMAL_RUNNING=0,
	SX1276_TX_RUNNING,
	SX1276_RX_RUNNING
}SX1276_State_e;

typedef struct
{
    int8_t SnrValue;
    int16_t RssiValue;
    uint8_t Size;
}RadioLoRaPacketHandler_t;

typedef struct{
	uint32_t Frequency;
	RadioModems_t Modem_Type;
	int8_t TxPower;
	uint32_t fDev;
	uint32_t Bandwidth;
	uint32_t Datarate;
	uint8_t Coderate;
	uint16_t PreambleLen;
	bool FixLen;
	bool CrcOn;
	bool FreqHopOn;
	uint8_t HopPeriod;
	bool IqInverted;
	uint32_t Timeout;
}TxConfig_t;

typedef struct{
	uint32_t Frequency;
	RadioModems_t Modem_Type;
	uint32_t Bandwidth;
	uint32_t Datarate;
	uint8_t Coderate;
	uint32_t BandwidthAfc;
	uint16_t PreambleLen;
	uint16_t SymbTimeout;
	bool FixLen;
	uint8_t PayloadLen;
	bool CrcOn;
	bool FreqHopOn;
	uint8_t HopPeriod;
	bool IqInverted;
	bool RxContinuous;
	uint32_t MaxRxWindow1;
	uint32_t MaxRxWindow2;
	uint16_t MaxPayload;
}RxConfig_t;
/*****************************************************************************************************************/
/*											    Software Command Interface     									 */
/*****************************************************************************************************************/
typedef struct{
	void (*OnTxDone)(void);

	void (*OnTxTimeout)(void);

	void (*OnRxDone)(uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr);

	void (*OnRxWindow1Timeout)(void);

	void (*OnRxWindow2Timeout)(void);

	void (*DIO0_IRQ)(void);

	void (*DIO1_IRQ)(void);

	void (*DIO2_IRQ)(void);

	void (*DIO3_IRQ)(void);
}SX1276_UserFunction_t;

typedef struct{
	bool InitDone;

	RadioModems_t Modem;

	uint8_t length;

	uint8_t buffer[RX_BUFFER_SIZE];

	uint32_t channel;

	SX1276_State_e State;

	RadioLoRaPacketHandler_t PacketHandler;

	const void (*Reset)(void);

	const void (*RxChainCalibration)(void);

	const void (*SetOpMode)(uint8_t OpMode);

	const void (*SetModem)(RadioModems_t modem);

	const void (*SetPublicNetwork)(bool enable);

	const uint8_t (*Read)(uint8_t adr);

	const void (*ReadBuffer)(uint8_t adr,uint8_t *buffer,uint8_t length);

	const void (*Write)(uint8_t adr, uint8_t data);

	const void (*WriteBuffer)(uint8_t adr, uint8_t *buffer, uint8_t length);

	const uint8_t (*InOut)(uint8_t outData);

	const void (*Init)(void);

	const void (*Sleep)(void);

	const void (*Send)(TxConfig_t TxConfig);

	const void (*Prepareframe)(uint8_t *Data, uint16_t size);

	const void (*SetChannel)();

	const void (*SetMaxPayloadLength)(RadioModems_t Modem, uint8_t length);

	const void (*SetTxConfig)(TxConfig_t TxConfig);/*( RadioModems_t modem, int8_t power, uint32_t fdev,
            uint32_t bandwidth, uint32_t datarate,
            uint8_t coderate, uint16_t preambleLen,
            bool fixLen, bool crcOn, bool freqHopOn,
            uint8_t hopPeriod, bool iqInverted, uint32_t timeout );*/

	const void (*SetStby)(void);

	const void (*DIO0_IRQ)(void);

	const void (*DIO1_IRQ)(void);

	const void (*DIO2_IRQ)(void);

	const void (*DIO3_IRQ)(void);

	SX1276_UserFunction_t UserFunction;

	const void (*print)(const char *__restrict, ...);

	const void (*SetUserFunction)(SX1276_UserFunction_t UserFunction);

	const uint32_t (*Random)(void);

	const bool (*RxWindowSetup)(RxConfig_t RxConfig);

	const void (*SetRxConfig)(RxConfig_t RxConfig);

	const void (*SetRx)(RxConfig_t RxConfig);

}SCI_SX1276_t;

typedef struct{
	const HCI_SX1276_t *HCI;

	const SCI_SX1276_t *SCI;
}SX1276_t;

extern SX1276_t SX1276;
#endif
