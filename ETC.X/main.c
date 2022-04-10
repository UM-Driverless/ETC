/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.7
        Device            :  PIC18F26Q84
        Driver Version    :  2.00
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#include "mcc_generated_files/mcc.h" // Initialize functions
#include "MESSAGES.h"
#include "CLUTCH.h"
#include "GPIO.h"
#include "ETC.h"
#include "ANALOG.h"

/*
    Main application
 */
void main(void) {
    // Initialize the device - MCC Pins and configurations, still interrupts not Enabled.
    SYSTEM_Initialize();

    // If using interrupts in PIC18 High/Low Priority Mode you need to enable the Global High and Low Interrupts
    // If using interrupts in PIC Mid-Range Compatibility Mode you need to enable the Global Interrupts
    GPIOInit(); // Set digital inputs and outputs.
    
    etc_calibrate();
    apps_calibrate();
    ClutchCalibrate();
    
    INTERRUPT_GlobalInterruptEnable(); // Now the functions in TEMPORIZATIONS.c start working.

    // To disable the global interrupts:
    // INTERRUPT_GlobalInterruptDisable();
    
    while(1) {
        LED_Toggle();
        
        ETC_PID(ucAPPS_perc, ManualMode);
        
        /// Play sound of some value - uiAPPS1_mv uiAPPS2_mv ucAPPS1_perc ucAPPS2_perc ucAPPS_perc
//        GPIO_PWM2_Control(10, 100*ucAPPS1_perc); // Play as frequency
        
        
        CANWriteMessage(ETC_SIGNAL, DataLength_6, ucAPPS1_perc, ucAPPS2_perc, ucTPS1_perc, ucTPS2_perc, 0, 0, 0, 0); //Falta meter los APPS target
    }
}
/*
 End of File
*/

// TODO - CAN error messages whenever something is wrong.
// TODO - Make local any variables that don't need to be global. Local variables in main such as uiAPPS1_pushed_mv, and pass by reference, instead of global. Easy code flow analysis
// TODO - Description for every single variable
// TODO - FIX Function that creates PWM so it works with any frequency, and test 30kHz frequencies to remove audible noise.

/*
 What are the frequency limits of the ET Motor driver? Note in a comment
 
 
 
 */