/* 
 * File:   TEMPORIZATIONS.h
 * Author: 93Urbano
 *
 * Created on December 4, 2021, 2:50 PM
 */

#include "../ETC.X/mcc_generated_files/pin_manager.h"
#include "CLUTCH.h"
#include "TEMPORIZATIONS.h"
#include "ANALOG.h"
#include "MESSAGES.h"
#include "ETC.h"
#include "PARAMETERS.h"

//VARIABLES
unsigned char ucCount500ms;
unsigned char ucCount1s;
unsigned char ucCount10s;
unsigned int uiCount30s;
unsigned int uiCount1min;
unsigned char ucCount50ms;


//FUNCIONES
void TEMPORIZATION_10ms (void)
{
    //CLUTCH_Move(ucSTEER_WH_Clutch, ManualMode);
    if (ucCount50ms++ == 5 )
    {
        TPSAnalysis();
        APPSAnalysis();
        ucCount50ms = 0;
    }
}

void TEMPORIZATION_100ms (void)
{
    /*ANALOGRead();
    //TPSAnalysis();
    //APPSAnalysis();
    ETCManual(ucAPPSManual);*/
    ETC100msSupervisor();
    CANWriteMessage(ETC_SIGNAL, DataLength_6, ucAPPS1Perc, ucAPPS2Perc, ucTPS1Perc, ucTPS2Perc, ucAPPS, ucTPS, 0, 0);    //Falta meter los APPS target
    //CANWriteMessage(ETC_SIGNAL, DataLength_6, (uiTPS1>>8), ( uiTPS1 & 0xFF), (uiTPS2>>8), ( uiTPS2 & 0xFF), ucAPPS, ucTPS, 0, 0);    //Falta meter los APPS target
}

void TEMPORIZATION_500ms (void)
{
    //ANALIZAR IN1 E IN2 PARA ESTADO CLUTCH
    //ANALOGRead();
    CLUTCH_AnalyseState();
    
    ETCXavierSupervisor();
    ucETCBeatSupervisor = FALSE;
    ETC500msSupervisor();
}

void TEMPORIZATION_1s (void)
{
    CANWriteMessage(ETC_STATE, DataLength_4, ucTPS_STATE, ucAPPS_STATE, ucCLUTCHState, ucETB_STATE, 0, 0, 0, 0);  
    LED_Toggle();
}

void TEMPORIZATION_10s (void)
{

}

void TEMPORIZATION_30s (void)
{

}

void TEMPORIZATION_1mins (void)
{
    
}




