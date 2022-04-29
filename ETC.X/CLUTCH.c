/* 
 * File:   CLUTCH.c
 * Author: 93Urbano
 *
 * Created on 4 de diciembre de 2021, 13:37
 */

#include "CLUTCH.h"
#include "mcc_generated_files/pin_manager.h"
#include "PARAMETERS.h"
#include "GPIO.h"
#include "MESSAGES.h"
#include "ETC.h"
#include "global.h" // It declares here the global variables from global.c

void ClutchCalibrate(void){
    /* Move clutch to the initial position, then 
     * 
     */
    
    // Clutch to initial position
    GPIO_PWM1_Control( 0, 50 ); // Position signal to the clutch servo
    uiCLUTCHState = CLUTCH_NONE;
        
    //APPSMODE_SetHigh();
    //APPSSend(0);
    
    CLUTCH_Move(0, ASMode);
    
    CLUTCH_AnalyseState();
    if ( uiCLUTCHState == CLUTCH_ENGAGE ) // CLUTCH_AnalyseState() defines uiCLUTCHState
    {
        CLUTCH_Move(30, ASMode);
        __delay_ms(200);
        CLUTCH_Move(50, ASMode);
        __delay_ms(200);
    }
    
    CLUTCH_AnalyseState();
    if ( uiCLUTCHState == CLUTCH_INTRAVEL )
    {
        //__delay_ms(200);
        CLUTCH_Move(70, ASMode);
        __delay_ms(200);
        CLUTCH_Move(100, ASMode);
        __delay_ms(150);
    }
    
    CLUTCH_AnalyseState();
    if ( uiCLUTCHState == CLUTCH_DISENGAGE )
    {
        //__delay_ms(200);
        CLUTCH_Move(0, ASMode);
    }
}

void CLUTCH_Move (signed long slTargetMove, unsigned char ucMode)
{
    //Depender de beat constante en CAN
    if ( ucETCFlagSupervisor == TRUE )
    {
        //CUANDO TENGAMOS LA IN1, ESTA COMPLETAMENTE EMBRAGADO, METER ESTADOS EMBRAGUE
        //uiCLUTCHState
        //ANALIZAR SI SE PUEDE MOVER HASTA EL RANGO MAX LEIDO POR LA INT2
        //HACER CONVERSION DE 0-100% A 2-12 DUTY
        uiCLUTCHDuty = slTargetMove * 60;
        uiCLUTCHDuty = uiCLUTCHDuty / 100;
        uiCLUTCHDuty = (uiCLUTCHDuty & 0xFF);
        //nos tenemos que asegurar antes de mover que aceptamos ordenes de manual o autonomo
        if ( ucMode == ucASMode ) 
        {
            //proteccion frente a corrupcion de datos
            /*if ( uiCLUTCHDuty <= uiCLUTCHlmax )
            {
                if ( ( uiCLUTCHDuty > 0 ) && ( uiCLUTCHDuty < 12 ) ) //0-180º con 50Hz
                {
                    GPIO_PWM1_Control(uiCLUTCHDuty, 250);
                }
            }
            else
            {
                //generar error de rango
            }*/
            //proteccion frente a posicion fisica erronea del embrague
            if ( uiCLUTCHState < CLUTCH_ERROR )
            {
                GPIO_PWM1_Control(uiCLUTCHDuty, 300);
            }
        }
        else
        {
            //generar error movimiento impedido por modo de conduccion
        }
    }
    else
    {
        GPIO_PWM1_Control(0, 300);
    }
    
}


void CLUTCH_AnalyseState(void)
{
    unsigned char ucFDC1; //embragado
    unsigned char ucFDC2; //desembrgado
    
    ucFDC1 = FinCarrera1_GetValue(); 
    ucFDC2 = FinCarrera2_GetValue();
    
    Nop();
    
    if ( ( ucFDC1 == TRUE ) &&  ( ucFDC2 == FALSE ) ) //EMBRAGADO
    {
        uiCLUTCHState = CLUTCH_ENGAGE;
        uiCLUTCHlmin = uiCLUTCHDuty;
    }
    else if ( ( ucFDC1 == FALSE ) &&  ( ucFDC2 == TRUE ) ) //DESAMBRAGADO
    {
        uiCLUTCHState = CLUTCH_DISENGAGE;
        uiCLUTCHlmax = uiCLUTCHDuty;
    }
    else if ( ( ucFDC1 == FALSE ) &&  ( ucFDC2 == FALSE ) ) //RECORRIDO
    {
        uiCLUTCHState = CLUTCH_INTRAVEL;
    }
    else if ( ( ucFDC1 == TRUE ) &&  ( ucFDC2 == TRUE ) ) //FALSO
    {
        uiCLUTCHState = CLUTCH_ERROR;
    }
    else //CORRUPCION DE VARIABLES
    {
        uiCLUTCHState = CLUTCH_CORRUPT;
    }
}