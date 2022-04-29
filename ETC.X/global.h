/* 
 * File:   global.h
 * Author: ruben
 *
 * Created on 29 April 2022, 13:40
 * 
 * 
 * #include this file to declare the global variables defined in global.c
 */

#ifndef GLOBAL_H
#define	GLOBAL_H

#ifdef	__cplusplus
extern "C" {
#endif


// APPS Variables - Defaults, real-time values, states
extern unsigned int uiAPPS1_default_mv; // int -> 16 bit in MPLAB, PIC18. short = int here.
extern unsigned int uiAPPS2_default_mv;
extern unsigned int uiAPPS1_pushed_mv;
extern unsigned int uiAPPS2_pushed_mv;

extern unsigned int uiAPPS1_mv; // Sent by CAN
extern unsigned int uiAPPS2_mv; // Sent by CAN
extern unsigned char ucAPPS1_perc;
extern unsigned char ucAPPS2_perc;
extern unsigned char ucAPPS_perc;

extern unsigned char ucAPPS_STATE;
extern unsigned char ucAPPSManual;
    
    
// TPS Variables - Defaults, real-time values, states
extern unsigned int uiTPS1_default_mv; // Called by TEMPORIZATIONS.c, TODO move to main and pass to etc_calibrate();
extern unsigned int uiTPS1_opened_mv; // Called by TEMPORIZATIONS.c, TODO move to main and pass to etc_calibrate();
extern unsigned int uiTPS2_default_mv; // Called by TEMPORIZATIONS.c, TODO move to main and pass to etc_calibrate();
extern unsigned int uiTPS2_opened_mv; // Called by TEMPORIZATIONS.c, TODO move to main and pass to etc_calibrate();

extern unsigned int uiTPS1_mv; // Sent by CAN, used by ANALOG.c for ANALOG_GetVoltage, called by interruption in TEMPORIZATIONS.c
extern unsigned int uiTPS2_mv; // Sent by CAN
extern unsigned char ucTPS1_perc;
extern unsigned char ucTPS2_perc;
extern unsigned char ucTPSPerc;

extern unsigned int uiETCDuty;
extern unsigned char ucTPS_STATE;
extern unsigned char ucTPS1_STATE;
extern unsigned char ucTPS2_STATE;
extern unsigned char ucTPS_Volts_STATE;
extern unsigned char ucETB_STATE;
extern unsigned char ucETCBeatSupervisor;
extern unsigned char ucETCFlagSupervisor;

// MESSAGES
extern unsigned char CANDATAdata[8];

// TRAJECTORY_ACT
extern unsigned char ucTargetAccelerator;
extern unsigned char ucTargetClutch;
extern unsigned char ucTargetBrake;
extern unsigned char ucTargetDirection;
extern unsigned char ucTargetGear;

// DV_SYSTEM_STATUS
extern unsigned char ucAS_state;
extern unsigned char ucEBS_state;
extern unsigned char ucAMI_state;
extern unsigned char ucSteering_state;
extern unsigned char ucService_brake;
extern unsigned char ucLap_counter;
extern unsigned char ucCones_count_actual;
extern unsigned char ucCones_count_all;

// DV_DRIVING_DYNAMICS_1
extern unsigned char ucSpeed_actual;
extern unsigned char ucSpeed_target;
extern unsigned char ucSteering_angle_actual;
extern unsigned char ucSteering_angle_target;
extern unsigned char ucBrake_hydr_actual;
extern unsigned char ucBrake_hydr_target;
extern unsigned char ucMotor_moment_actual;
extern unsigned char ucMotor_moment_target;

// DV_DRIVING_DYNAMICS_2
extern unsigned int uiAcc_longitudinal;
extern unsigned int uiAcc_lateral;
extern unsigned int uiYaw_rate;

// ASB STATE
extern unsigned char ucASBState;
extern unsigned char ucASRequesState;

// PMC STATE
extern unsigned char ucASMode;

// STEERING WHEEL
extern unsigned char ucSTEER_WH_Clutch;

// CLUTCH
extern unsigned int uiCLUTCHlmin;
extern unsigned int uiCLUTCHlmax;
extern unsigned int uiCLUTCHDuty;
extern unsigned int uiCLUTCHState;

// TEMPORIZATIONS
extern unsigned char ucCount500ms;
extern unsigned char ucCount1s;
extern unsigned char ucCount10s;
extern unsigned int uiCount30s;
extern unsigned int uiCount1min;

// GPIO
extern unsigned char ucLEDState;
extern unsigned char ucPedalPos;
extern unsigned char ucPICHDRPRES1min;
extern unsigned char ucPICHDRPRES2min;
extern unsigned int uiCLUTCHlmin;
extern unsigned int uiCLUTCHlmax;
extern unsigned int uiCLUTCHDuty;
extern unsigned int uiCLUTCHState;

// ANALOG
extern unsigned int ucPICHDRPRES1;
extern unsigned int ucPICHDRPRES2;
extern unsigned int ucPICNPRES1;
extern unsigned int ucPICNPRES2;
extern unsigned int ucPICNPRES3;
extern unsigned int ucPICNPRES4;
extern unsigned int ucAN1;
extern unsigned int ucAN2;

// SERVICEBRAKE
extern unsigned int uiDutyServomotor;
extern unsigned char ucServoLmin;
extern unsigned char ucServoLmax;
extern unsigned char ucServoLDif;
extern unsigned char ucASBBeatSupervisor; 
extern unsigned char ucASBFlagSupervisor; 

// PARAMETERS
extern unsigned char HDRPRES_min;
extern unsigned char HDRPRES_max;
extern unsigned char NPRES_min;
extern unsigned char NPRES_max;

#ifdef	__cplusplus
}
#endif

#endif	/* GLOBAL_H */

