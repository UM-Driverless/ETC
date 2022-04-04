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
    

// VARIABLES    
extern unsigned int uiAPPS1min;
extern unsigned int uiAPPS1;
extern unsigned int uiAPPS1max;

extern unsigned int uiAPPS2min;
extern unsigned int uiAPPS2;
extern unsigned int uiAPPS2max;

extern unsigned int uiTPS1_default;
extern unsigned int uiTPS1_opened;
extern unsigned int uiTPS2_default;
extern unsigned int uiTPS2_opened;

extern unsigned char ucAPPS_STATE;
extern unsigned long ulAPPS1calc;
extern unsigned long ulAPPS2calc;
extern unsigned char ucAPPS1Perc;
extern unsigned char ucAPPS2Perc;
extern unsigned char ucAPPS;
extern unsigned int uiTPS1;
extern unsigned int uiTPS2;
extern unsigned long ulTPS1calc;
extern unsigned long ulTPS2calc;
extern unsigned char ucTPS1Perc;
extern unsigned char ucTPS2Perc;
extern unsigned char ucTPS;
extern unsigned char ucTPS_STATE;
extern unsigned char ucTPS1_STATE;
extern unsigned char ucTPS2_STATE;
extern unsigned char ucTPS_Volts_STATE;
extern unsigned int uiETCDuty;
extern unsigned char ucETB_STATE;
extern unsigned char ucETCBeatSupervisor;
extern unsigned char ucETCFlagSupervisor;
extern unsigned char ucAPPSManual;
extern signed char scLastErrorPos;
extern signed char scErrorPos;

//FUNCIONES
void APPSSend (unsigned char ucPercent);
void APPSReadmin(void);
void APPSReadmax(void);
void tps_read_default(void);
void tps_read_opened(void);
void ETCModeSelect (unsigned char ucModeSelect);
void ETCRulesSupervision(void);
void ETCMove(unsigned char ucTargetMove, unsigned char ucMode);
void ETCInitMove(void);
void TPSAnalysis(void);
void APPSAnalysis(void);
void ETCSupervisor(void);
void ETCManual (unsigned char ucTargetManual);
void ETC_PIDcontroller (unsigned char ucTargetMove, unsigned char ucMode);


#ifdef	__cplusplus
}
#endif

#endif	/* ETC_H */

