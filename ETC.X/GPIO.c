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
     * PWM CONTROL
     * https://youtu.be/u_snBrZ2ijs
     * The PWM has to be linked to a timer. It resets at each PWM cycle and switches the PWM output
     * after reaching a threshold value.
     * Use an even timer: TMR2, TMR4, TMR6...
     * Use FOSC/4 as clock source, prescaler of 1:4 (postscaler 1:1, doesn't matter),
     * Timer Period = 1ms, to create PWM freq of 1kHz.
     * Then link a PWM peripheral to the timer.
     * 
     * DATA: EXTOSC XT 1MHz external crystal oscillator, clock divider = 1.
     * PWM uses FOSC as clock. Prescaler = 100.
     * PWM_CLOCK_freq = clock_freq / prescaler = 1MHz / 100 = 10kHz // Editable in MCC. For ASB: 10MHz/255 = 39215
     * uiTotalPeriod = (PWM_period / PWM_CLOCK_period) - 1 = (PWM_CLOCK_freq / PWM_freq) - 1
     * 
     * 
     * Some sources say a frequency between 3000 and 6000 Hz is the most quiet. TODO try it: 100Hz, 1kHz, 2kHz, 3kHz, 30kHz.
     * https://electrosome.com/pwm-pic-microcontroller-hi-tech-c/
     * https://ww1.microchip.com/downloads/en/DeviceDoc/dsPIC33-PIC24-FRM-High-Resolution-PWM-with-Fine-Edge-Placement-70005320b.pdf
     * 
     * TODO try with 3kHz, 6kHz, 30kHz.
     */
    
    // Protection of inputs
    if (uiDutyCyclePerc > 100) uiDutyCyclePerc = 100;
    if (uiFreq > GPIO_PWM2_CLOCK_FREQ) uiFreq = GPIO_PWM2_CLOCK_FREQ;
    
    // Calculations
    unsigned int uiTotalPeriod; // static for speed.
    unsigned int uiActivePeriod; 
    
    uiTotalPeriod = (GPIO_PWM2_CLOCK_FREQ / uiFreq) - 1;
    uiActivePeriod = (uiTotalPeriod * uiDutyCyclePerc) / 100;
    
    // Configure the total PWM2_16BIT period
    PWM2_16BIT_WritePeriodRegister(uiTotalPeriod);
    
    // Configure the active period of the PWM - Duty Cycle
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