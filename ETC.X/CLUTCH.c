/* 
 * File:   CLUTCH.c
 * Author: 93Urbano
 *
 * Created on 4 de diciembre de 2021, 13:37
 */

//INCLUDES
#include "CLUTCH.h"
#include "mcc_generated_files/pin_manager.h"
#include "PARAMETERS.h"
#include "GPIO.h"


//VARIABLES
unsigned char ucCLUTCHlmin;
unsigned char ucCLUTCHlmax;
unsigned int uiCLUTCHDuty;
unsigned char ucCLUTCHState;

//FUNCIONES
void CLUTCH_Init (void)
{
    GPIO_PWM1_Control( 0, 50 );
    ucCLUTCHState = CLUTCH_NONE;
}



void CLUTCH_Move (unsigned char ucTargetMove)
{
    //CUANDO TENGAMOS LA IN1, ESTA COMPLETAMENTE EMBRAGADO, METER ESTADOS EMBRAGUE
    //ucCLUTCHState
    //ANALIZAR SI SE PUEDE MOVER HASTA EL RANGO MAX LEIDO POR LA INT2
    //HACER CONVERSION DE 0-100% A 2-12 DUTY
    uiCLUTCHDuty = ucTargetMove * 12;
    uiCLUTCHDuty = uiCLUTCHDuty / 100;
    uiCLUTCHDuty = (uiCLUTCHDuty & 0xFF);
    if ( uiCLUTCHDuty <= ucCLUTCHlmax )
    {
        if ( ( uiCLUTCHDuty > 0 ) && ( uiCLUTCHDuty < 12 ) ) //0-180º con 50Hz
        {
            GPIO_PWM1_Control(uiCLUTCHDuty, 50);
        }
    }
    else
    {
        //generar error de rango
    }
    
}


void CLUTCH_AnalyseState (void)
{
    unsigned char ucFDC1; //embragado
    unsigned char ucFDC2; //desembrgado
    
    ucFDC1 = FinCarrera1_GetValue(); 
    ucFDC2 = FinCarrera2_GetValue();
    
    if ( ( ucFDC1 == TRUE ) &&  ( ucFDC2 == FALSE ) ) //EMBRAGADO
    {
        ucCLUTCHState = CLUTCH_ENGAGE;
    }
    else if ( ( ucFDC1 == FALSE ) &&  ( ucFDC2 == TRUE ) ) //DESAMBRAGADO
    {
        ucCLUTCHState = CLUTCH_DISENGAGE;
    }
    else if ( ( ucFDC1 == FALSE ) &&  ( ucFDC2 == FALSE ) ) //RECORRIDO
    {
        ucCLUTCHState = CLUTCH_INTRAVEL;
    }
    else if ( ( ucFDC1 == TRUE ) &&  ( ucFDC2 == TRUE ) ) //FALSO
    {
        ucCLUTCHState = CLUTCH_ERROR;
    }
    else //CORRUPCION DE VARIABLES
    {
        ucCLUTCHState = CLUTCH_CORRUPT;
    }
}