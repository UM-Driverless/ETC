/*
 * Global variables.
 * Here they are defined (reserved memory for them), and elsewhere in the files they shall be declared to be used locally with the keyword "extern".
 * To do that, #include the file "global.h". In that way, when we change the type or name of a global variable, 
 * we won't have to edit every single file where it is used. Just global.c and global.h. It's less prone to errors.
*/

#include "PARAMETERS.h" // To use FALSE, TRUE, etc.
#include "MESSAGES.h" // to include #define ? should not be needed

// APPS Variables - Defaults, real-time values, states
unsigned int uiAPPS1_default_mv; // int -> 16 bit in MPLAB, PIC18. short = int here.
unsigned int uiAPPS2_default_mv;
unsigned int uiAPPS1_pushed_mv;
unsigned int uiAPPS2_pushed_mv;

unsigned int uiAPPS1_mv; // Sent by CAN
unsigned int uiAPPS2_mv; // Sent by CAN
unsigned char ucAPPS1_perc;
unsigned char ucAPPS2_perc;
unsigned char ucAPPS_perc;

unsigned char ucAPPS_STATE;
unsigned char ucAPPSManual;

// TPS Variables - Defaults, real-time values, states
unsigned int uiTPS1_default_mv;
unsigned int uiTPS2_default_mv;
unsigned int uiTPS1_opened_mv;
unsigned int uiTPS2_opened_mv;

unsigned int uiTPS1_mv; // Global because it depends on interruptions. (TEMPORIZATIONS.c)
unsigned int uiTPS2_mv;
unsigned char ucTPS1_perc;
unsigned char ucTPS2_perc;
unsigned char ucTPSPerc;

unsigned char ucTPS_STATE;
unsigned char ucTPS1_STATE;
unsigned char ucTPS2_STATE;
unsigned char ucTPS_Volts_STATE;
unsigned int uiETCDuty;
unsigned char ucETB_STATE;
unsigned char ucETCBeatSupervisor = FALSE;
unsigned char ucETCFlagSupervisor = FALSE;

// MESSAGES
unsigned char CANDATAdata[8] = CAN_TX_BUFF; //uint8_t CANDATAdata[8];
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
unsigned char ucASMode;

// STEERING WHEEL
unsigned char ucSTEER_WH_Clutch;

// CLUTCH
unsigned int uiCLUTCHlmin;
unsigned int uiCLUTCHlmax;    
unsigned int uiCLUTCHDuty;
unsigned int uiCLUTCHState;

// TEMPORIZATIONS
unsigned char ucCount500ms;
unsigned char ucCount1s;
unsigned char ucCount10s;
unsigned int uiCount30s;
unsigned int uiCount1min;

// GPIO
unsigned char ucLEDState;
unsigned char ucPedalPos;
unsigned char ucPICHDRPRES1min;
unsigned char ucPICHDRPRES2min;
unsigned int uiCLUTCHlmin;
unsigned int uiCLUTCHlmax;
unsigned int uiCLUTCHDuty;
unsigned int uiCLUTCHState;


// ANALOG
unsigned int ucPICHDRPRES1;
unsigned int ucPICHDRPRES2;
unsigned int ucPICNPRES1;
unsigned int ucPICNPRES2;
unsigned int ucPICNPRES3;
unsigned int ucPICNPRES4;
unsigned int ucAN1;
unsigned int ucAN2;

// SERVICEBRAKE
unsigned int uiDutyServomotor;
unsigned char ucServoLmin;
unsigned char ucServoLmax;
unsigned char ucServoLDif;
unsigned char ucASBBeatSupervisor = FALSE; 
unsigned char ucASBFlagSupervisor = FALSE; 

// PARAMETERS
unsigned char HDRPRES_min;
unsigned char HDRPRES_max;
unsigned char NPRES_min;
unsigned char NPRES_max;





// TODO - Explain here what each variable does, with an inline comment, and maybe why is it global