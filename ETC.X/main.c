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

#include "mcc_generated_files/mcc.h"
#include "MESSAGES.h"
#include "CLUTCH.h"
#include "GPIO.h"
#include "ETC.h"
#include "ANALOG.h"
#include "PARAMETERS.h"
/*
                         Main application
 */
void main(void)
{
    // Initialize the device - MCC Pins and configurations, still interrupts not Enabled.
    SYSTEM_Initialize();

    // If using interrupts in PIC18 High/Low Priority Mode you need to enable the Global High and Low Interrupts
    // If using interrupts in PIC Mid-Range Compatibility Mode you need to enable the Global Interrupts
    
    GPIOInit(); // Set digital inputs and outputs.
    ETCInit();
    
    /// APPS CALIBRATE
    ANALOGRead(); // Read APPS and TPS sensor values.
    //Tarar sensores TPS y APPS a minimos
    APPSReadmin();
    APPSReadmax();
    
    INTERRUPT_GlobalInterruptEnable(); // Now the functions in TEMPORIZATIONS.c start working.
    CANDisableErrorInterrupt(DISABLE);
    
    // To disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();
    
    CLUTCH_Init();

    //CLUTCHInitMove();
   
    
    while (1)
    {
        // Add your application code
        //CANWriteMessage(0, DataLength_1, 10, 0, 0, 0, 0, 0, 0, 0);
        //LED_Toggle();
        ANALOGRead();
        TPSAnalysis();
        APPSAnalysis();
        ETCRulesSensorsSupervision(); //justo despues de analizar porcentajes debe ir la supervision
        //ETC_PID(ucAPPS, ManualMode);
        
        if (ucASMode == ManualMode)
        {
            ETCMove(ucAPPS,ManualMode);
            //GPIO_PWM2_Control(PIDController_Update(&pid, (float)(ucAPPS), (float)(ucTPS)), 600);
        }
        else if (ucASMode == ASMode)
        {
            ETCMove(ucTargetAccelerator,ASMode);
            //GPIO_PWM2_Control(PIDController_Update(&pid, (float)(ucTargetAccelerator), (float)(ucTPS)), 600);
            ucCLUTCHState = CLUTCH_NONE;
            GPIO_PWM1_Control(ucTargetClutch, 300);
        }
        
        
        /*ANALOGRead();
        TPSAnalysis();
        APPSAnalysis();*/
    }
}
/**
 End of File
*/

