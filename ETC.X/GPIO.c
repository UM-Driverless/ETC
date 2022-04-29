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
#include "global.h" // It declares here the global variables from global.c

// FUNCTIONS
void GPIOInit(void)
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
// Servos go a 50Hz DC:2-12% GPIO_PWM1_Control(12, 50);
//uiFreq Hz
void GPIO_PWM1_Control (unsigned int uiDutyCycle, unsigned int uiFreq)
{
    unsigned int uiConvertedPeriod;
    unsigned int uiConvertedDC;
    
    // Conversions
    uiConvertedPeriod = ( 39241/uiFreq );
    uiConvertedPeriod = ( uiConvertedPeriod - 1.1508 );
    uiConvertedDC = ( uiDutyCycle * 4 );
    uiConvertedDC = ( uiConvertedDC * 100 );
    uiConvertedDC = ( uiConvertedDC / uiFreq );
    // Functions
    PWM1_16BIT_SetSlice1Output1DutyCycleRegister(uiConvertedDC);
    PWM1_16BIT_WritePeriodRegister(uiConvertedPeriod);
    PWM1_16BIT_LoadBufferRegisters();
}

/****GPIO_PWM2_Control****/
//uiDutyCycle 0 - 100% 
// Servos go a 50Hz DC:2-12% GPIO_PWM1_Control(12, 50);
//uiFreq Hz
void GPIO_PWM2_Control (unsigned int uiDutyCycle, unsigned int uiFreq)
{
    unsigned int uiConvertedPeriod;
    unsigned int uiConvertedDC; // Max 65536, want freq of 30000
    
    // Conversions
    uiConvertedDC = 400 / (unsigned long)(uiFreq) * (unsigned long)(uiDutyCycle);
    PWM2_16BIT_SetSlice1Output1DutyCycleRegister(uiConvertedDC);
    
    uiConvertedPeriod = 39241/(unsigned long)(uiFreq) - 1.1508;
    PWM2_16BIT_WritePeriodRegister(uiConvertedPeriod);
    
    // Functions
    PWM2_16BIT_LoadBufferRegisters();
}


// NO EJECUTAR
void GPIO_INT2_desembragar(void)
{
    //ANALIZAR SI REALMENTE ESTA HIGH
    //SI ESTÁ SELECCIONAR EL VALOR DE DUTY QUE LE HEMOS APLICADO COMO EL MÁXIMO
    if ( FinCarrera2_GetValue() == TRUE )
    {
        uiCLUTCHlmax = uiCLUTCHDuty;
        uiCLUTCHState = CLUTCH_DISENGAGE;
    }
}