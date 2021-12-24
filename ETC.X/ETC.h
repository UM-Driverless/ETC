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
extern unsigned char ucTPS1;
extern unsigned char ucTPS2; 

//FUNCIONES
void APPSSend (unsigned char ucPercent);
void APPSReadmin (void);
void APPSReadmax (void);
void TPSReadmin (void);
void TPSReadmax (void);


#ifdef	__cplusplus
}
#endif

#endif	/* ETC_H */

