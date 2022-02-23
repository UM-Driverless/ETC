/* 
 * File:   ETC.h
 * Author: 93Urbano
 *
 * Created on December 24, 2021, 6:15 PM
 */

#ifndef ETC_H
#define	ETC_H

#ifdef	__cplusplus
extern "C" {
#endif
    
    
//CONSTANTES
#define TPS_OK              0
//errores por salidad de margenes
#define TPS1_ERROR          1
#define TPS2_ERROR          2
#define TPS1_TPS2_ERROR     3
#define TPS_Volt_ERROR      4
#define QUITAR_ERROR_TPS1  0xFE    
#define QUITAR_ERROR_TPS2  0xFD   
//inversion de voltaje
#define TPS1_NO_INVERTED    1  
#define TPS1_INVERTED       2
#define TPS2_NO_INVERTED    4  
#define TPS2_INVERTED       8
#define TPS1_NO_INVERTED_TPS2_NO_INVERTED       5
#define TPS1_NO_INVERTED_TPS2_INVERTED          9  
#define TPS1_INVERTED_TPS2_NO_INVERTED          6  
#define TPS1_INVERTED_TPS2_INVERTED             10  
#define QUITAR_ERROR_VOLTS                      0xFB       
    

//VARIABLES    
extern unsigned char ucAPPS1min;
extern unsigned char ucAPPS1max;    
extern unsigned char ucAPPS2min;
extern unsigned char ucAPPS2max;  
extern unsigned char ucTPS1min;
extern unsigned char ucTPS1max;    
extern unsigned char ucTPS2min;
extern unsigned char ucTPS2max; 
extern unsigned char ucAPPS1;
extern unsigned char ucAPPS2;   
extern unsigned char ucAPPS_STATE; 
extern unsigned char ucTPS1;
extern unsigned char ucTPS2; 
extern unsigned char ucTPS1calc;
extern unsigned char ucTPS2calc; 
extern unsigned char ucTPS_STATE; 
extern unsigned char ucTPS1_STATE; 
extern unsigned char ucTPS2_STATE; 
extern unsigned char ucTPS_Volts_STATE; 
extern unsigned int uiETCDuty;
extern unsigned char ucETB_STATE; 
extern unsigned char ucETCBeatSupervisor; 
extern unsigned char ucETCFlagSupervisor; 

//FUNCIONES
void APPSSend (unsigned char ucPercent);
void APPSReadmin (void);
void APPSReadmax (void);
void TPSReadmin (void);
void TPSReadmax (void);
void ETCModeSelect (unsigned char ucModeSelect);
void ETCRulesSupervision(void);
void ETCMove(unsigned char ucTargetMove, unsigned char ucMode);
void ETCInitMove(void);
void TPSAnalysis (void);
void APPSAnalysis (void);
void ETCSupervisor (void);

#ifdef	__cplusplus
}
#endif

#endif	/* ETC_H */

