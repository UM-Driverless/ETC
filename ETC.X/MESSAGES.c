/* 
 * File:   MESSAGES.c
 * Author: 93Urbano
 *
 * Created on September 20, 2021, 10:50 AM
 */

#include "MESSAGES.h"
#include "mcc_generated_files/mcc.h"
#include "ETC.h"
#include "CLUTCH.h"
#include "PARAMETERS.h"
#include "ETC.h"

// VARIABLES
uint8_t CANDATAdata[8];

// TRAJECTORY_ACT
unsigned char ucTargetAccelerator;
unsigned char ucTargetClutch;
unsigned char ucTargetBrake;
unsigned char ucTargetDirection;
unsigned char ucTargetGear;
// DV_SYSTEM_STATUS
unsigned char ucAS_state;
unsigned char ucEBS_state;
unsigned char ucAMI_state;
unsigned char ucSteering_state;
unsigned char ucService_brake;
unsigned char ucLap_counter;
unsigned char ucCones_count_actual;
unsigned int uiCones_count_all;
// DV_DRIVING_DYNAMICS_1
unsigned char ucSpeed_actual;
unsigned char ucSpeed_target;
unsigned char ucSteering_angle_actual;
unsigned char ucSteering_angle_target;
unsigned char ucBrake_hydr_actual;
unsigned char ucBrake_hydr_target;
unsigned char ucMotor_moment_actual;
unsigned char ucMotor_moment_target;
// DV_DRIVING_DYNAMICS_2
unsigned int uiAcc_longitudinal;
unsigned int uiAcc_lateral;
unsigned int uiYaw_rate;
// ASB_STATE
unsigned char ucASBState;
unsigned char ucASRequesState;
// PMC STATE - Autonomous vs Manual according to the PMC
unsigned char ucASMode; // Declare a local instance of the already-declared header variable, saving space for it. Each file that declares it gets an instance.
// STEERING WHEEL
unsigned char ucSTEER_WH_Clutch;

//FUNCIONES
void CANWriteMessage(unsigned long ul_id, unsigned char uc_dataLength, unsigned char uc_data1, unsigned char uc_data2, unsigned char uc_data3, unsigned char uc_data4, unsigned char uc_data5, unsigned char uc_data6, unsigned char uc_data7, unsigned char uc_data8) {
    CANDATAdata [0] = uc_data1;
    CANDATAdata [1] = uc_data2;
    CANDATAdata [2] = uc_data3;
    CANDATAdata [3] = uc_data4;
    CANDATAdata [4] = uc_data5;
    CANDATAdata [5] = uc_data6;
    CANDATAdata [6] = uc_data7;
    CANDATAdata [7] = uc_data8;
    
    msgTransmit.msgId = ul_id;
    msgTransmit.field.formatType = CAN_2_0_FORMAT;  //CAN 2.0
    msgTransmit.field.brs = CAN_NON_BRS_MODE;   //NO BRS
    msgTransmit.field.frameType = CAN_FRAME_DATA;   //FRAME DATA, NO REMOTE
    msgTransmit.field.idType = CAN_FRAME_STD;   //CAN VERSION STANDARD
    msgTransmit.field.dlc = ( uc_dataLength & 0x0F ); //DATA LENGTH
    msgTransmit.data = CANDATAdata;
    
    if (CAN1_IsBusOff() == TRUE)
    {
        Nop();
    }
    if (CAN1_IsTxErrorPassive() == TRUE)
    {
        Nop();
    }
    if (CAN1_IsTxErrorWarning() == TRUE)
    {
        Nop();
    }
    if (CAN1_IsTxErrorActive() == TRUE)
    {
        Nop();
    }

    if (CAN_TX_FIFO_AVAILABLE == (CAN1_TransmitFIFOStatusGet(CAN1_TX_TXQ) & CAN_TX_FIFO_AVAILABLE))
    {        
        CAN1_Transmit(CAN1_TX_TXQ, &msgTransmit);
        Nop();
    }
}



void CANReadMessage(void) {
    uint32_t id;
    unsigned char idType;
    unsigned char dlc;
    unsigned char uc_data1;
    unsigned char uc_data2;
    unsigned char uc_data3;
    unsigned char uc_data4;
    unsigned char uc_data5;
    unsigned char uc_data6;
    unsigned char uc_data7;
    unsigned char uc_data8;
    
    if (CAN1_ReceivedMessageCountGet() > 0) {
        if (true == CAN1_Receive(&msgReceipt)) {
            Nop();
            id = msgReceipt.msgId;
            idType = msgReceipt.field.idType;
            dlc = msgReceipt.field.dlc;
            uc_data1 = msgReceipt.data[0];
            uc_data2 = msgReceipt.data[1];
            uc_data3 = msgReceipt.data[2];
            uc_data4 = msgReceipt.data[3];
            uc_data5 = msgReceipt.data[4];
            uc_data6 = msgReceipt.data[5];
            uc_data7 = msgReceipt.data[6];
            uc_data8 = msgReceipt.data[7];
            
            switch(id) {
                case TRAJECTORY_ACT:
                    ucTargetAccelerator = uc_data1;
                    ucTargetClutch = uc_data2;
                    ucTargetBrake = uc_data3;
                    ucTargetDirection = uc_data4;
                    ucTargetGear = uc_data5;
                    // Move clutch and APPS
                    if ( ucASMode == ASMode ) {
                        CLUTCH_Move(ucTargetClutch, ASMode);
                        //ETCMove(ucTargetAccelerator, ASMode);
                        ETC_PID( ucTargetAccelerator, ASMode);
                        ucETCBeatSupervisor = TRUE;
                    }
                    //APPSSend(ucTargetAccelerator);
                    break;
                case DV_SYSTEM_STATUS:
                    ucAS_state = ( uc_data1 & 0x07 );
                    ucEBS_state = ( uc_data1 & 0x18 );
                    ucAMI_state = ( uc_data1 & 0xE0 );
                    ucSteering_state = ( uc_data2 & 0x01 );
                    ucService_brake = ( uc_data2 & 0x06 );
                    ucLap_counter = ( uc_data2 & 0x78 );
                    //ucC*ones_count_actual = ( ( ( uc_data3 & 0x7F ) << 1 ) | ( ( uc_data2 & 0x80 ) >> 7 ) );
                    //uiCones_count_all = ( ( ( uc_data5 & 0xFF ) << 8 ) | ( ( uc_data4 & 0xFF ) << 1 ) | ( ( uc_data3 & 0x80 ) >> 7 ) );
                    break;
                case DV_DRIVING_DYNAMICS_1:
                    ucSpeed_actual = uc_data1;
                    ucSpeed_target = uc_data2;
                    ucSteering_angle_actual = uc_data3;
                    ucSteering_angle_target = uc_data4;
                    ucBrake_hydr_actual = uc_data5;
                    ucBrake_hydr_target = uc_data6;
                    ucMotor_moment_actual = uc_data7;
                    ucMotor_moment_target = uc_data8;
                    break;
                case DV_DRIVING_DYNAMICS_2:
                    //uiAcc_longitudinal = ( ( uc_data2 << 8 ) | uc_data1 );
                    //uiAcc_lateral = ( ( uc_data4 << 8 ) | uc_data3 );
                    //uiYaw_rate = ( ( uc_data6 << 8 ) | uc_data5 );;
                    break;
                case PMC_STATE:
                    ucASMode = uc_data1;
                    ETCModeSelect(ucASMode);
                    break;
                case STEER_WH_CONT:
                    ucSTEER_WH_Clutch = uc_data1;
                    if ( ucASMode == ManualMode )
                    {
                        CLUTCH_Move(ucSTEER_WH_Clutch, ManualMode);
                    }
                    break;
                default:
                    Nop();
                    break;
            }
        }
    }
}
