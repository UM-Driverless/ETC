/* 
 * File:   TEMPORIZATIONS.h
 * Author: 93Urbano
 *
 * Created on December 4, 2021, 2:50 PM
 */

#ifndef TEMPORIZATIONS_H
#define	TEMPORIZATIONS_H

#ifdef	__cplusplus
extern "C" {
#endif
    
//DEFINES
#define t500ms 5       
#define t1s    10        
#define t10s   100       
#define t30s   300       
#define t1min  600 

//FUNCIONES
void TEMPORIZATION_10ms(void);
void TEMPORIZATION_100ms(void);
void TEMPORIZATION_500ms(void);
void TEMPORIZATION_1s(void);
void TEMPORIZATION_10s(void);
void TEMPORIZATION_30s(void);
void TEMPORIZATION_1mins(void);

#ifdef	__cplusplus
}
#endif

#endif	/* TEMPORIZATIONS_H */

