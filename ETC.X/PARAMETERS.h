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
    
//PARAMETROS CONSTANTES    
#define TPSMARGEN 100
#define APPSMARGEN 100
#define APPS1max 1990 //0.001v
#define APPS2max 160 //0.001v

extern signed long sl_K_P;
extern signed long sl_K_I;
extern signed long sl_K_D;

#ifdef	__cplusplus
}
#endif

#endif	/* PARAMETERS_H */

