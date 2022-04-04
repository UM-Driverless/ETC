/* 
 * File:   ANALOG.h
 * Author: 93Urbano
 *
 * Created on December 24, 2021, 6:29 PM
 */

#ifndef ANALOG_H
#define	ANALOG_H

#ifdef	__cplusplus
extern "C" {
#endif

//Entradas analógicas     
#define ENT_APPS1       1
#define ENT_APPS2       2
#define ENT_TPS1        3
#define ENT_TPS2        4

//FUNCIONES
unsigned int ANALOG_GetVoltage (unsigned char ucEntradaAnalogica);
void ANALOGRead(void);

#ifdef	__cplusplus
}
#endif

#endif	/* ANALOG_H */

