/* 
 * File:   GPIO.h
 * Author: 93Urbano
 *
 * Created on 4 de diciembre de 2021, 13:40
 */

#include "GPIO.h"
#include "mcc_generated_files/pwm1_16bit.h"
#include "mcc_generated_files/pwm2_16bit.h"
#include "mcc_generated_files/pin_manager.h"
#include "../ETC.X/PARAMETERS.h"
#include "CLUTCH.h"


//VARIABLES
unsigned char ucCLUTCHlmin;
unsigned char ucCLUTCHlmax;    
unsigned char ucCLUTCHDuty;
unsigned char ucCLUTCHState;


//FUNCIONES
void GPIOInit (void)
{
    LED_SetDigitalOutput();
    FinCarrera1_SetDigitalInput();
    FinCarrera2_SetDigitalInput();
    APPSMODE_SetDigitalOutput();
    START_SetDigitalOutput();
    SDC_SetDigitalOutput();
    OUTM_SetDigitalOutput();
    STBY_SetDigitalOutput();
    GPIO_PWM1_Control( 0, 300 );
    GPIO_PWM2_Control( 0, 600 );
}


/****GPIO_PWM1_Control****/
//uiDutyCycle 0 - 100% 
//Servomotores van a 50Hz DC:2-12% GPIO_PWM1_Control(12, 50);
//uiFreq Hz
void GPIO_PWM1_Control (unsigned int uiDutyCycle, unsigned int uiFreq)
{
    unsigned int uiConvertedPeriod;
    unsigned int uiConvertedDC;
    
    //Conversiones
    uiConvertedPeriod = ( 39241/uiFreq );
    uiConvertedPeriod = ( uiConvertedPeriod - 1.1508 );
    uiConvertedDC = ( uiDutyCycle * 4 );
    uiConvertedDC = ( uiConvertedDC * 100 );
    uiConvertedDC = ( uiConvertedDC / uiFreq );
    //Funciones
    PWM1_16BIT_SetSlice1Output1DutyCycleRegister(uiConvertedDC); 
    PWM1_16BIT_WritePeriodRegister(uiConvertedPeriod);
    PWM1_16BIT_LoadBufferRegisters();
}

/****GPIO_PWM2_Control****/
//uiDutyCycle 0 - 100% 
//Servomotores van a 50Hz DC:2-12% GPIO_PWM1_Control(12, 50);
//uiFreq Hz
void GPIO_PWM2_Control (unsigned int uiDutyCycle, unsigned int uiFreq)
{
    unsigned int uiConvertedPeriod;
    unsigned int uiConvertedDC;
    
    //Conversiones
    uiConvertedPeriod = ( 39241/uiFreq );
    uiConvertedPeriod = ( uiConvertedPeriod - 1.1508 );
    uiConvertedDC = ( uiDutyCycle * 4 );
    uiConvertedDC = ( uiConvertedDC * 100 );
    uiConvertedDC = ( uiConvertedDC / uiFreq );
    //Funciones
    PWM2_16BIT_SetSlice1Output1DutyCycleRegister(uiConvertedDC); 
    PWM2_16BIT_WritePeriodRegister(uiConvertedPeriod);
    PWM2_16BIT_LoadBufferRegisters();
}


//NO EJECUTAR
void GPIO_INT2_desembragar (void)
{
    //ANALIZAR SI REALMENTE ESTA HIGH
    //SI EST� SELECCIONAR EL VALOR DE DUTY QUE LE HEMOS APLICADO COMO EL M�XIMO
    if ( FinCarrera2_GetValue() == TRUE )
    {
        ucCLUTCHlmax = ucCLUTCHDuty;
        ucCLUTCHState = CLUTCH_DISENGAGE;
    }
}