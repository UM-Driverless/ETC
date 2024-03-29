/* 
 * File:   CLUTCH.h
 * Author: 93Urbano
 *
 * Created on 4 de diciembre de 2021, 13:37
 */

#ifndef CLUTCH_H
#define	CLUTCH_H

#ifdef	__cplusplus
extern "C" {
#endif

//CONSTANTES
#define CLUTCH_NONE        0
#define CLUTCH_ENGAGE      1
#define CLUTCH_DISENGAGE   2
#define CLUTCH_INTRAVEL    3
#define CLUTCH_ERROR       4
#define CLUTCH_CORRUPT     5
    
//CLUTCH ACTIONS
#define NO_MOVE_CLUTCH     0
#define ENGAGE_CLUTCH      1
#define DISENGAGE_CLUTCH   2
#define CLUTCH_TO_CERO     3
    
    
//VARIABLES
extern unsigned char ucCLUTCHlmin;
extern unsigned char ucCLUTCHlmax;    
extern unsigned char ucCLUTCHDuty;
extern unsigned char ucCLUTCHState;


//FUNCIONES
void CLUTCH_Init (void);
void CLUTCH_Move (unsigned char ucTargetMove, unsigned char ucMode);
void CLUTCH_AnalyseState (void);
void CLUTCHInitMove(void);
void CLUTCH_HighLevelMovements (unsigned char ucClutchAction);
void CLUTCHTestMove(unsigned char ucMode);

#ifdef	__cplusplus
}
#endif

#endif	/* CLUTCH_H */

