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
unsigned char PercOf(signed long val, signed long min, signed long max);

#ifdef	__cplusplus
}
#endif

#endif	/* ETC_H */

