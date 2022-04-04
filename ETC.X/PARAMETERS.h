/* 
 * File:   PARAMETERS.h
 * Author: 93Urbano
 *
 * Created on December 4, 2021, 2:24 PM
 */

#ifndef PARAMETERS_H
#define	PARAMETERS_H

#ifdef	__cplusplus
extern "C" {
#endif

#define FALSE   0x00
#define TRUE    0x01
    
// PARAMETROS CONSTANTES    
#define TPSMARGEN 20
#define APPSMARGEN 100
#define APPS1max 160 //0.001v
#define APPS2max 1990 //0.001v

// ETC PID CONTROL CONSTANTS
#define ETC_KP 0.9
#define ETC_KI 0.1 // 5 goes crazy, 1 does nothing
#define ETC_KD 0

#ifdef	__cplusplus
}
#endif

#endif	/* PARAMETERS_H */

