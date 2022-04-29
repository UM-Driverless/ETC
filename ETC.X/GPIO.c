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
void GPIO_PWM2_Control(unsigned int uiDutyCyclePerc, unsigned int uiFreq) {
    /*
     * Creates a PWM signal according to the input duty cycle as a percentage from 0 to 100, and the input frequency.
     * 
     * It uses the functions declared in pwm2_16bit.h
     * 
     * The PWM frequency is set with interruptions that work on a "base frequency", now set at 10kHz.
     * Therefore, the PWM frequeny can be between almost 0 and 10kHz.
     * 
     * Mode: Left aligned, Freq: 10kHz, 7 bits of resolution, no prescale, 
     * 
     * Some sources say a frequency between 3000 and 6000 Hz is the most quiet. TODO try it: 100Hz, 1kHz, 2kHz, 3kHz, 30kHz.
     * https://electrosome.com/pwm-pic-microcontroller-hi-tech-c/
     * https://ww1.microchip.com/downloads/en/DeviceDoc/dsPIC33-PIC24-FRM-High-Resolution-PWM-with-Fine-Edge-Placement-70005320b.pdf
     * 
     * 
     */
    
    //Protection of inputs
    if (uiDutyCyclePerc > 100) uiDutyCyclePerc = 100;
    if (uiFreq > 10000) uiFreq = 10000;
    
    // Calculations
    unsigned int uiTotalPeriod; // static for speed.
    unsigned int uiActivePeriod; 
    
    uiTotalPeriod = 10000 / (uiFreq); // PWM2PR = crystal_freq / (desired_pwm_freq * prescale) = wanted_period / crystal_period = how many crystal periods I want for a PWM period
    uiActivePeriod = (uiTotalPeriod * uiDutyCyclePerc) / 100;
    
    // Configure the total PWM2_16BIT period
    PWM2_16BIT_WritePeriodRegister(uiTotalPeriod);
    
    // Configure the active period of the PWM, the duty cycle
    PWM2_16BIT_SetSlice1Output1DutyCycleRegister(uiActivePeriod);
    
    // Load period or duty cycle registers on the next period event
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