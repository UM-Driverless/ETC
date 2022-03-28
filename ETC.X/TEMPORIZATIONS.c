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


//FUNCIONES
void TEMPORIZATION_10ms (void)
{
    //CLUTCH_Move(ucSTEER_WH_Clutch, ManualMode);
}

void TEMPORIZATION_100ms (void)
{
    //ANALOGRead();
    //ETCManual(ucAPPSManual);
}

void TEMPORIZATION_500ms (void)
{
    //CANWriteMessage(ETC_SIGNAL, DataLength_6, ucAPPS1, ucAPPS2, ucTPS1, ucTPS2, 0, 0, 0, 0);    //Falta meter los APPS target
    //ANALIZAR IN1 E IN2 PARA ESTADO CLUTCH
    ANALOGRead();
    CLUTCH_AnalyseState();
    
    ETCSupervisor();
    ucETCBeatSupervisor = FALSE;
    
}

void TEMPORIZATION_1s (void)
{
    //LED_Toggle();
    //CANWriteMessage(ETC_STATE, DataLength_4, ucTPS_STATE, ucAPPS_STATE, ucCLUTCHState, ucETB_STATE, 0, 0, 0, 0);  
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




