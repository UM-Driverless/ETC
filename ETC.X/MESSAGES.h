/* 
 * File:   MESSAGE.c
 * Author: 93Urbano
 *
 * Created on September 20, 2021, 10:50 AM
 */

#ifndef MESSAGE_H
#define	MESSAGE_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "mcc_generated_files/mcc.h"
#define CAN_TX_BUFF  "BUFFERCAN"

CAN_MSG_OBJ msgTransmit;
CAN_MSG_OBJ msgReceipt;
uint8_t CANDATAdata[8];

// DEFINITIONS
#define DataLength_0        			0
#define DataLength_1        			1
#define DataLength_2        			2
#define DataLength_3        			3
#define DataLength_4        			4
#define DataLength_5        			5
#define DataLength_6        			6
#define DataLength_7        			7
#define DataLength_8        			8

// Driving Modes
#define ManualMode  0
#define ASMode      1

// CAN MESSAGES
#define DV_SYSTEM_STATUS            0x500
#define DV_DRIVING_DYNAMICS_1       0x501
#define DV_DRIVING_DYNAMICS_2       0x502
#define TRAJECTORY_ACT              0x320
// ASB
#define ASB_ANALOG                  0x360
#define ASB_SIGNALS                 0x361
#define ASB_STATE                   0x362
// PMC
#define PMC_STATE                   0x347
// STEERING WHEEL
#define STEER_WH_CONT               0x412
// ETC
#define ETC_SIGNAL                  0x330
#define ETC_STATE                   0x331

void CANWriteMessage(unsigned long ul_id, unsigned char uc_dataLength, unsigned char uc_data1, unsigned char uc_data2, unsigned char uc_data3, unsigned char uc_data4, unsigned char uc_data5, unsigned char uc_data6, unsigned char uc_data7, unsigned char uc_data8);
void CANReadMessage(void);
void CANSetErrorInterrupt(unsigned char ucEnable);

#ifdef	__cplusplus
}
#endif

#endif	/* MESSAGE_H */