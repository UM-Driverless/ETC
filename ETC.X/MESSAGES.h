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
    
//INCLUDES
#include "mcc_generated_files/mcc.h"
    
    
//DEFINICION DE CONSTANTES    
#define CAN_TX_BUFF  {0,0,0,0,0,0,0,0}
 
CAN_MSG_OBJ msgTransmit;
CAN_MSG_OBJ msgReceipt;

//VARIABLES
extern unsigned char CANDATAdata[8];
//TRAJECTORY_ACT
extern unsigned char ucTargetAccelerator;
extern unsigned char ucTargetClutch;
extern unsigned char ucTargetBrake;
extern unsigned char ucTargetDirection;
extern unsigned char ucTargetGear;
//DV_SYSTEM_STATUS
extern unsigned char ucAS_state;
extern unsigned char ucEBS_state;
extern unsigned char ucAMI_state;
extern unsigned char ucSteering_state;
extern unsigned char ucService_brake;
extern unsigned char ucLap_counter;
extern unsigned char ucCones_count_actual;
extern unsigned char ucCones_count_all;
//DV_DRIVING_DYNAMICS_1
extern unsigned char ucSpeed_actual;
extern unsigned char ucSpeed_target;
extern unsigned char ucSteering_angle_actual;
extern unsigned char ucSteering_angle_target;
extern unsigned char ucBrake_hydr_actual;
extern unsigned char ucBrake_hydr_target;
extern unsigned char ucMotor_moment_actual;
extern unsigned char ucMotor_moment_target;
//DV_DRIVING_DYNAMICS_2
extern unsigned int uiAcc_longitudinal;
extern unsigned int uiAcc_lateral;
extern unsigned int uiYaw_rate;
//ASB_STATE
extern unsigned char ucASBState;
extern unsigned char ucASRequesState;

//DEFINICIONES
#define DataLength_0        			0
#define DataLength_1        			1
#define DataLength_2        			2
#define DataLength_3        			3
#define DataLength_4        			4
#define DataLength_5        			5
#define DataLength_6        			6
#define DataLength_7        			7
#define DataLength_8        			8

//Modos conduccion
#define ManualMode  0
#define ASMode      1

//CAN MESSAGES
#define DV_SYSTEM_STATUS            0x500
#define DV_DRIVING_DYNAMICS_1       0x501
#define DV_DRIVING_DYNAMICS_2       0x502
#define TRAJECTORY_ACT              0x320
#define ASB_ANALOG                  0x360
#define ASB_SIGNALS                 0x361
#define ASB_STATE                   0x362
#define PMC_STATE                   0x345
    
//DEFINICION DE FUNCIONES    
void CANWriteMessage(unsigned long id, unsigned char dataLength, unsigned char data1, unsigned char data2, unsigned char data3, unsigned char data4, unsigned char data5, unsigned char data6, unsigned char data7, unsigned char data8);
void CANReadMessage (void);


#ifdef	__cplusplus
}
#endif

#endif	/* MESSAGE_H */