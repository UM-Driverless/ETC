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


unsigned int counter = 0; // Check while frequency
unsigned int counts = 0;


//FUNCIONES
void TEMPORIZATION_10ms(void)
{
    
}

void TEMPORIZATION_100ms(void)
{
    CANWriteMessage(ETC_SIGNAL, DataLength_6, uiAPPS1, uiAPPS2, uiTPS1, uiTPS2, 0, 0, 0, 0);    //Falta meter los APPS target
    //CANWriteMessage(ETC_STATE, DataLength_4, ucTPS_STATE, ucAPPS_STATE, ucCLUTCHState, ucETB_STATE, 0, 0, 0, 0);  
}

void TEMPORIZATION_500ms(void)
{
    LED_Toggle();
    //ANALIZAR IN1 E IN2 PARA ESTADO CLUTCH
    CLUTCH_AnalyseState();
    
    ETCSupervisor();
    ucETCBeatSupervisor = FALSE;
}

void TEMPORIZATION_1s(void)
{
    
}

void TEMPORIZATION_10s(void)
{
    counts = counter;
    counter = 0;
    Nop();
}

void TEMPORIZATION_30s(void)
{

}

void TEMPORIZATION_1mins(void)
{
    
}




