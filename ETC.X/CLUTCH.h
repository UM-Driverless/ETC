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

    
#define CLUTCH_NONE        0
#define CLUTCH_ENGAGE      1
#define CLUTCH_DISENGAGE   2
#define CLUTCH_INTRAVEL    3
#define CLUTCH_ERROR       4
#define CLUTCH_CORRUPT     5

void ClutchCalibrate(void);
void CLUTCH_Move (signed long slTargetMove, unsigned char ucMode);
void CLUTCH_AnalyseState(void);


#ifdef	__cplusplus
}
#endif

#endif	/* CLUTCH_H */

