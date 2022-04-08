/* 
 * File:   ETC.h
 * Author: 93Urbano
 *
 * Created on December 24, 2021, 6:15 PM
 */

#ifndef ETC_H
#define	ETC_H

// If C++ code, this will tell the compiler to interpret the code inside as C.
#ifdef	__cplusplus
extern "C" {
#endif
    
// CONSTANTS
#define TPS_OK              0
// errores por salida de margenes
#define TPS1_ERROR          1
#define TPS2_ERROR          2
#define TPS1_TPS2_ERROR     3
#define TPS_Volt_ERROR      4
#define QUITAR_ERROR_TPS1  0xFE    
#define QUITAR_ERROR_TPS2  0xFD   
// inversion de voltaje
#define QUITAR_ERROR_VOLTS                      0xFB       
    
    
/// Global Variables

// APPS Variables - Defaults, real-time values, states
extern unsigned int uiAPPS1_default_mv;
extern unsigned int uiAPPS2_default_mv;
extern unsigned int uiAPPS1_opened_mv;
extern unsigned int uiAPPS2_opened_mv;

extern unsigned int ucAPPS1_mv;
extern unsigned int ucAPPS2_mv;
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
extern unsigned char ucTPS_perc;

extern unsigned int uiETCDuty;
extern unsigned char ucTPS_STATE;
extern unsigned char ucTPS1_STATE;
extern unsigned char ucTPS2_STATE;
extern unsigned char ucTPS_Volts_STATE;
extern unsigned char ucETB_STATE;
extern unsigned char ucETCBeatSupervisor;
extern unsigned char ucETCFlagSupervisor;


// APPS functions
void APPSSend (unsigned char ucPercent);
void apps_calibrate(void);

// Electronic Throttle functions (ETC)
void ETCModeSelect (unsigned char ucModeSelect);
void ETCRulesSupervision(void);
void ETCMove(signed long slTargetMove, unsigned char ucMode);
void etc_calibrate(void);
void TPSAnalysis(void);
void APPSAnalysis(void);
void ETCSupervisor(void);
void ETCManual (unsigned char ucTargetManual);
void ETC_PID(signed long slTargetMove, unsigned char ucMode);

// Generic functions
unsigned char perc_of(signed long val, signed long min, signed long max);

#ifdef	__cplusplus
}
#endif

#endif	/* ETC_H */

