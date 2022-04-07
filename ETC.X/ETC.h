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
#define TPS1_NO_INVERTED    1  
#define TPS1_INVERTED       2
#define TPS2_NO_INVERTED    4  
#define TPS2_INVERTED       8
#define TPS1_NO_INVERTED_TPS2_NO_INVERTED       5
#define TPS1_NO_INVERTED_TPS2_INVERTED          9  
#define TPS1_INVERTED_TPS2_NO_INVERTED          6  
#define TPS1_INVERTED_TPS2_INVERTED             10  
#define QUITAR_ERROR_VOLTS                      0xFB       
    

/// VARIABLES    
extern unsigned int uiAPPS1min;
extern unsigned int uiAPPS1; // Sent by CAN
extern unsigned int uiAPPS1max;

extern unsigned int uiAPPS2min;
extern unsigned int uiAPPS2; // Sent by CAN
extern unsigned int uiAPPS2max;

// Electronic Throttle (Body, Motor, Control, Sensor...)
extern unsigned int ui_tps1_default; // Called by TEMPORIZATIONS.c, TODO move to main and pass to etc_calibrate();
extern unsigned int uiTPS1_opened; // Called by TEMPORIZATIONS.c, TODO move to main and pass to etc_calibrate();
extern unsigned int ui_tps2_default; // Called by TEMPORIZATIONS.c, TODO move to main and pass to etc_calibrate();
extern unsigned int uiTPS2_opened; // Called by TEMPORIZATIONS.c, TODO move to main and pass to etc_calibrate();


extern unsigned int ui_tps1_mv; // Sent by CAN
extern unsigned int ui_tps2_mv; // Sent by CAN
extern unsigned char uc_tps1_perc;
extern unsigned char uc_tps2_perc;
extern unsigned char uc_tps_perc;
extern unsigned char ucTPS_STATE;
extern unsigned char ucTPS1_STATE;
extern unsigned char ucTPS2_STATE;
extern unsigned char ucTPS_Volts_STATE;
extern unsigned int uiETCDuty;
extern unsigned char ucETB_STATE;
extern unsigned char ucETCBeatSupervisor;
extern unsigned char ucETCFlagSupervisor;

extern unsigned char ucAPPS_STATE;
extern unsigned long ulAPPS1calc;
extern unsigned long ulAPPS2calc;
extern unsigned char ucAPPS1Perc;
extern unsigned char ucAPPS2Perc;
extern unsigned char ucAPPS;


extern unsigned char ucAPPSManual;
extern signed long slErrorPos;

/// Functions
void APPSSend (unsigned char ucPercent);
void apps_calibrate(void);
void ETCModeSelect (unsigned char ucModeSelect);
void ETCRulesSupervision(void);
void ETCMove(unsigned char slTargetMove, unsigned char ucMode);
void etc_calibrate(void);
void TPSAnalysis(void);
void APPSAnalysis(void);
void ETCSupervisor(void);
void ETCManual (unsigned char ucTargetManual);
void ETC_PIDcontroller(signed long slTargetMove, unsigned char ucMode);

/// Testing
void sensor_sound(void);

#ifdef	__cplusplus
}
#endif

#endif	/* ETC_H */

