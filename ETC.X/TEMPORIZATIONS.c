/* 
 * File:   TEMPORIZATIONS.h
 * Author: 93Urbano
 *
 * Created on December 4, 2021, 2:50 PM
 */

#include "../ETC.X/mcc_generated_files/pin_manager.h"
#include "CLUTCH.h"
#include "TEMPORIZATIONS.h"

//VARIABLES
unsigned char ucCount500ms;
unsigned char ucCount1s;
unsigned char ucCount10s;
unsigned int uiCount30s;
unsigned int uiCount1min;


//FUNCIONES
void TEMPORIZATION_10ms (void)
{

}

void TEMPORIZATION_100ms (void)
{
    //ANALIZAR IN1 E IN2 PARA ESTADO CLUTCH
    CLUTCH_AnalyseState();
}

void TEMPORIZATION_500ms (void)
{
    LED_Toggle();
}

void TEMPORIZATION_1s (void)
{
    
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




