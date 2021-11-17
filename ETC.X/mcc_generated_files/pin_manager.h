/**
  @Generated Pin Manager Header File

  @Company:
    Microchip Technology Inc.

  @File Name:
    pin_manager.h

  @Summary:
    This is the Pin Manager file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This header file provides APIs for driver for .
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.7
        Device            :  PIC18F26Q84
        Driver Version    :  2.11
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.31 and above
        MPLAB 	          :  MPLAB X 5.45	
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

#ifndef PIN_MANAGER_H
#define PIN_MANAGER_H

/**
  Section: Included Files
*/

#include <xc.h>

#define INPUT   1
#define OUTPUT  0

#define HIGH    1
#define LOW     0

#define ANALOG      1
#define DIGITAL     0

#define PULL_UP_ENABLED      1
#define PULL_UP_DISABLED     0

// get/set LED aliases
#define LED_TRIS                 TRISAbits.TRISA0
#define LED_LAT                  LATAbits.LATA0
#define LED_PORT                 PORTAbits.RA0
#define LED_WPU                  WPUAbits.WPUA0
#define LED_OD                   ODCONAbits.ODCA0
#define LED_ANS                  ANSELAbits.ANSELA0
#define LED_SetHigh()            do { LATAbits.LATA0 = 1; } while(0)
#define LED_SetLow()             do { LATAbits.LATA0 = 0; } while(0)
#define LED_Toggle()             do { LATAbits.LATA0 = ~LATAbits.LATA0; } while(0)
#define LED_GetValue()           PORTAbits.RA0
#define LED_SetDigitalInput()    do { TRISAbits.TRISA0 = 1; } while(0)
#define LED_SetDigitalOutput()   do { TRISAbits.TRISA0 = 0; } while(0)
#define LED_SetPullup()          do { WPUAbits.WPUA0 = 1; } while(0)
#define LED_ResetPullup()        do { WPUAbits.WPUA0 = 0; } while(0)
#define LED_SetPushPull()        do { ODCONAbits.ODCA0 = 0; } while(0)
#define LED_SetOpenDrain()       do { ODCONAbits.ODCA0 = 1; } while(0)
#define LED_SetAnalogMode()      do { ANSELAbits.ANSELA0 = 1; } while(0)
#define LED_SetDigitalMode()     do { ANSELAbits.ANSELA0 = 0; } while(0)

// get/set FinCarrera2 aliases
#define FinCarrera2_TRIS                 TRISAbits.TRISA1
#define FinCarrera2_LAT                  LATAbits.LATA1
#define FinCarrera2_PORT                 PORTAbits.RA1
#define FinCarrera2_WPU                  WPUAbits.WPUA1
#define FinCarrera2_OD                   ODCONAbits.ODCA1
#define FinCarrera2_ANS                  ANSELAbits.ANSELA1
#define FinCarrera2_SetHigh()            do { LATAbits.LATA1 = 1; } while(0)
#define FinCarrera2_SetLow()             do { LATAbits.LATA1 = 0; } while(0)
#define FinCarrera2_Toggle()             do { LATAbits.LATA1 = ~LATAbits.LATA1; } while(0)
#define FinCarrera2_GetValue()           PORTAbits.RA1
#define FinCarrera2_SetDigitalInput()    do { TRISAbits.TRISA1 = 1; } while(0)
#define FinCarrera2_SetDigitalOutput()   do { TRISAbits.TRISA1 = 0; } while(0)
#define FinCarrera2_SetPullup()          do { WPUAbits.WPUA1 = 1; } while(0)
#define FinCarrera2_ResetPullup()        do { WPUAbits.WPUA1 = 0; } while(0)
#define FinCarrera2_SetPushPull()        do { ODCONAbits.ODCA1 = 0; } while(0)
#define FinCarrera2_SetOpenDrain()       do { ODCONAbits.ODCA1 = 1; } while(0)
#define FinCarrera2_SetAnalogMode()      do { ANSELAbits.ANSELA1 = 1; } while(0)
#define FinCarrera2_SetDigitalMode()     do { ANSELAbits.ANSELA1 = 0; } while(0)

// get/set RA2 procedures
#define RA2_SetHigh()            do { LATAbits.LATA2 = 1; } while(0)
#define RA2_SetLow()             do { LATAbits.LATA2 = 0; } while(0)
#define RA2_Toggle()             do { LATAbits.LATA2 = ~LATAbits.LATA2; } while(0)
#define RA2_GetValue()              PORTAbits.RA2
#define RA2_SetDigitalInput()    do { TRISAbits.TRISA2 = 1; } while(0)
#define RA2_SetDigitalOutput()   do { TRISAbits.TRISA2 = 0; } while(0)
#define RA2_SetPullup()             do { WPUAbits.WPUA2 = 1; } while(0)
#define RA2_ResetPullup()           do { WPUAbits.WPUA2 = 0; } while(0)
#define RA2_SetAnalogMode()         do { ANSELAbits.ANSELA2 = 1; } while(0)
#define RA2_SetDigitalMode()        do { ANSELAbits.ANSELA2 = 0; } while(0)

// get/set RA3 procedures
#define RA3_SetHigh()            do { LATAbits.LATA3 = 1; } while(0)
#define RA3_SetLow()             do { LATAbits.LATA3 = 0; } while(0)
#define RA3_Toggle()             do { LATAbits.LATA3 = ~LATAbits.LATA3; } while(0)
#define RA3_GetValue()              PORTAbits.RA3
#define RA3_SetDigitalInput()    do { TRISAbits.TRISA3 = 1; } while(0)
#define RA3_SetDigitalOutput()   do { TRISAbits.TRISA3 = 0; } while(0)
#define RA3_SetPullup()             do { WPUAbits.WPUA3 = 1; } while(0)
#define RA3_ResetPullup()           do { WPUAbits.WPUA3 = 0; } while(0)
#define RA3_SetAnalogMode()         do { ANSELAbits.ANSELA3 = 1; } while(0)
#define RA3_SetDigitalMode()        do { ANSELAbits.ANSELA3 = 0; } while(0)

// get/set PICTPS2 aliases
#define PICTPS2_TRIS                 TRISAbits.TRISA4
#define PICTPS2_LAT                  LATAbits.LATA4
#define PICTPS2_PORT                 PORTAbits.RA4
#define PICTPS2_WPU                  WPUAbits.WPUA4
#define PICTPS2_OD                   ODCONAbits.ODCA4
#define PICTPS2_ANS                  ANSELAbits.ANSELA4
#define PICTPS2_SetHigh()            do { LATAbits.LATA4 = 1; } while(0)
#define PICTPS2_SetLow()             do { LATAbits.LATA4 = 0; } while(0)
#define PICTPS2_Toggle()             do { LATAbits.LATA4 = ~LATAbits.LATA4; } while(0)
#define PICTPS2_GetValue()           PORTAbits.RA4
#define PICTPS2_SetDigitalInput()    do { TRISAbits.TRISA4 = 1; } while(0)
#define PICTPS2_SetDigitalOutput()   do { TRISAbits.TRISA4 = 0; } while(0)
#define PICTPS2_SetPullup()          do { WPUAbits.WPUA4 = 1; } while(0)
#define PICTPS2_ResetPullup()        do { WPUAbits.WPUA4 = 0; } while(0)
#define PICTPS2_SetPushPull()        do { ODCONAbits.ODCA4 = 0; } while(0)
#define PICTPS2_SetOpenDrain()       do { ODCONAbits.ODCA4 = 1; } while(0)
#define PICTPS2_SetAnalogMode()      do { ANSELAbits.ANSELA4 = 1; } while(0)
#define PICTPS2_SetDigitalMode()     do { ANSELAbits.ANSELA4 = 0; } while(0)

// get/set APPSMODE aliases
#define APPSMODE_TRIS                 TRISAbits.TRISA5
#define APPSMODE_LAT                  LATAbits.LATA5
#define APPSMODE_PORT                 PORTAbits.RA5
#define APPSMODE_WPU                  WPUAbits.WPUA5
#define APPSMODE_OD                   ODCONAbits.ODCA5
#define APPSMODE_ANS                  ANSELAbits.ANSELA5
#define APPSMODE_SetHigh()            do { LATAbits.LATA5 = 1; } while(0)
#define APPSMODE_SetLow()             do { LATAbits.LATA5 = 0; } while(0)
#define APPSMODE_Toggle()             do { LATAbits.LATA5 = ~LATAbits.LATA5; } while(0)
#define APPSMODE_GetValue()           PORTAbits.RA5
#define APPSMODE_SetDigitalInput()    do { TRISAbits.TRISA5 = 1; } while(0)
#define APPSMODE_SetDigitalOutput()   do { TRISAbits.TRISA5 = 0; } while(0)
#define APPSMODE_SetPullup()          do { WPUAbits.WPUA5 = 1; } while(0)
#define APPSMODE_ResetPullup()        do { WPUAbits.WPUA5 = 0; } while(0)
#define APPSMODE_SetPushPull()        do { ODCONAbits.ODCA5 = 0; } while(0)
#define APPSMODE_SetOpenDrain()       do { ODCONAbits.ODCA5 = 1; } while(0)
#define APPSMODE_SetAnalogMode()      do { ANSELAbits.ANSELA5 = 1; } while(0)
#define APPSMODE_SetDigitalMode()     do { ANSELAbits.ANSELA5 = 0; } while(0)

// get/set RB0 procedures
#define RB0_SetHigh()            do { LATBbits.LATB0 = 1; } while(0)
#define RB0_SetLow()             do { LATBbits.LATB0 = 0; } while(0)
#define RB0_Toggle()             do { LATBbits.LATB0 = ~LATBbits.LATB0; } while(0)
#define RB0_GetValue()              PORTBbits.RB0
#define RB0_SetDigitalInput()    do { TRISBbits.TRISB0 = 1; } while(0)
#define RB0_SetDigitalOutput()   do { TRISBbits.TRISB0 = 0; } while(0)
#define RB0_SetPullup()             do { WPUBbits.WPUB0 = 1; } while(0)
#define RB0_ResetPullup()           do { WPUBbits.WPUB0 = 0; } while(0)
#define RB0_SetAnalogMode()         do { ANSELBbits.ANSELB0 = 1; } while(0)
#define RB0_SetDigitalMode()        do { ANSELBbits.ANSELB0 = 0; } while(0)

// get/set RB1 procedures
#define RB1_SetHigh()            do { LATBbits.LATB1 = 1; } while(0)
#define RB1_SetLow()             do { LATBbits.LATB1 = 0; } while(0)
#define RB1_Toggle()             do { LATBbits.LATB1 = ~LATBbits.LATB1; } while(0)
#define RB1_GetValue()              PORTBbits.RB1
#define RB1_SetDigitalInput()    do { TRISBbits.TRISB1 = 1; } while(0)
#define RB1_SetDigitalOutput()   do { TRISBbits.TRISB1 = 0; } while(0)
#define RB1_SetPullup()             do { WPUBbits.WPUB1 = 1; } while(0)
#define RB1_ResetPullup()           do { WPUBbits.WPUB1 = 0; } while(0)
#define RB1_SetAnalogMode()         do { ANSELBbits.ANSELB1 = 1; } while(0)
#define RB1_SetDigitalMode()        do { ANSELBbits.ANSELB1 = 0; } while(0)

// get/set FinCarrera1 aliases
#define FinCarrera1_TRIS                 TRISBbits.TRISB2
#define FinCarrera1_LAT                  LATBbits.LATB2
#define FinCarrera1_PORT                 PORTBbits.RB2
#define FinCarrera1_WPU                  WPUBbits.WPUB2
#define FinCarrera1_OD                   ODCONBbits.ODCB2
#define FinCarrera1_ANS                  ANSELBbits.ANSELB2
#define FinCarrera1_SetHigh()            do { LATBbits.LATB2 = 1; } while(0)
#define FinCarrera1_SetLow()             do { LATBbits.LATB2 = 0; } while(0)
#define FinCarrera1_Toggle()             do { LATBbits.LATB2 = ~LATBbits.LATB2; } while(0)
#define FinCarrera1_GetValue()           PORTBbits.RB2
#define FinCarrera1_SetDigitalInput()    do { TRISBbits.TRISB2 = 1; } while(0)
#define FinCarrera1_SetDigitalOutput()   do { TRISBbits.TRISB2 = 0; } while(0)
#define FinCarrera1_SetPullup()          do { WPUBbits.WPUB2 = 1; } while(0)
#define FinCarrera1_ResetPullup()        do { WPUBbits.WPUB2 = 0; } while(0)
#define FinCarrera1_SetPushPull()        do { ODCONBbits.ODCB2 = 0; } while(0)
#define FinCarrera1_SetOpenDrain()       do { ODCONBbits.ODCB2 = 1; } while(0)
#define FinCarrera1_SetAnalogMode()      do { ANSELBbits.ANSELB2 = 1; } while(0)
#define FinCarrera1_SetDigitalMode()     do { ANSELBbits.ANSELB2 = 0; } while(0)

// get/set RB3 procedures
#define RB3_SetHigh()            do { LATBbits.LATB3 = 1; } while(0)
#define RB3_SetLow()             do { LATBbits.LATB3 = 0; } while(0)
#define RB3_Toggle()             do { LATBbits.LATB3 = ~LATBbits.LATB3; } while(0)
#define RB3_GetValue()              PORTBbits.RB3
#define RB3_SetDigitalInput()    do { TRISBbits.TRISB3 = 1; } while(0)
#define RB3_SetDigitalOutput()   do { TRISBbits.TRISB3 = 0; } while(0)
#define RB3_SetPullup()             do { WPUBbits.WPUB3 = 1; } while(0)
#define RB3_ResetPullup()           do { WPUBbits.WPUB3 = 0; } while(0)
#define RB3_SetAnalogMode()         do { ANSELBbits.ANSELB3 = 1; } while(0)
#define RB3_SetDigitalMode()        do { ANSELBbits.ANSELB3 = 0; } while(0)

// get/set RB4 procedures
#define RB4_SetHigh()            do { LATBbits.LATB4 = 1; } while(0)
#define RB4_SetLow()             do { LATBbits.LATB4 = 0; } while(0)
#define RB4_Toggle()             do { LATBbits.LATB4 = ~LATBbits.LATB4; } while(0)
#define RB4_GetValue()              PORTBbits.RB4
#define RB4_SetDigitalInput()    do { TRISBbits.TRISB4 = 1; } while(0)
#define RB4_SetDigitalOutput()   do { TRISBbits.TRISB4 = 0; } while(0)
#define RB4_SetPullup()             do { WPUBbits.WPUB4 = 1; } while(0)
#define RB4_ResetPullup()           do { WPUBbits.WPUB4 = 0; } while(0)
#define RB4_SetAnalogMode()         do { ANSELBbits.ANSELB4 = 1; } while(0)
#define RB4_SetDigitalMode()        do { ANSELBbits.ANSELB4 = 0; } while(0)

// get/set STBY aliases
#define STBY_TRIS                 TRISBbits.TRISB5
#define STBY_LAT                  LATBbits.LATB5
#define STBY_PORT                 PORTBbits.RB5
#define STBY_WPU                  WPUBbits.WPUB5
#define STBY_OD                   ODCONBbits.ODCB5
#define STBY_ANS                  ANSELBbits.ANSELB5
#define STBY_SetHigh()            do { LATBbits.LATB5 = 1; } while(0)
#define STBY_SetLow()             do { LATBbits.LATB5 = 0; } while(0)
#define STBY_Toggle()             do { LATBbits.LATB5 = ~LATBbits.LATB5; } while(0)
#define STBY_GetValue()           PORTBbits.RB5
#define STBY_SetDigitalInput()    do { TRISBbits.TRISB5 = 1; } while(0)
#define STBY_SetDigitalOutput()   do { TRISBbits.TRISB5 = 0; } while(0)
#define STBY_SetPullup()          do { WPUBbits.WPUB5 = 1; } while(0)
#define STBY_ResetPullup()        do { WPUBbits.WPUB5 = 0; } while(0)
#define STBY_SetPushPull()        do { ODCONBbits.ODCB5 = 0; } while(0)
#define STBY_SetOpenDrain()       do { ODCONBbits.ODCB5 = 1; } while(0)
#define STBY_SetAnalogMode()      do { ANSELBbits.ANSELB5 = 1; } while(0)
#define STBY_SetDigitalMode()     do { ANSELBbits.ANSELB5 = 0; } while(0)

// get/set RB7 procedures
#define RB7_SetHigh()            do { LATBbits.LATB7 = 1; } while(0)
#define RB7_SetLow()             do { LATBbits.LATB7 = 0; } while(0)
#define RB7_Toggle()             do { LATBbits.LATB7 = ~LATBbits.LATB7; } while(0)
#define RB7_GetValue()              PORTBbits.RB7
#define RB7_SetDigitalInput()    do { TRISBbits.TRISB7 = 1; } while(0)
#define RB7_SetDigitalOutput()   do { TRISBbits.TRISB7 = 0; } while(0)
#define RB7_SetPullup()             do { WPUBbits.WPUB7 = 1; } while(0)
#define RB7_ResetPullup()           do { WPUBbits.WPUB7 = 0; } while(0)
#define RB7_SetAnalogMode()         do { ANSELBbits.ANSELB7 = 1; } while(0)
#define RB7_SetDigitalMode()        do { ANSELBbits.ANSELB7 = 0; } while(0)

// get/set PICTPS1 aliases
#define PICTPS1_TRIS                 TRISCbits.TRISC0
#define PICTPS1_LAT                  LATCbits.LATC0
#define PICTPS1_PORT                 PORTCbits.RC0
#define PICTPS1_WPU                  WPUCbits.WPUC0
#define PICTPS1_OD                   ODCONCbits.ODCC0
#define PICTPS1_ANS                  ANSELCbits.ANSELC0
#define PICTPS1_SetHigh()            do { LATCbits.LATC0 = 1; } while(0)
#define PICTPS1_SetLow()             do { LATCbits.LATC0 = 0; } while(0)
#define PICTPS1_Toggle()             do { LATCbits.LATC0 = ~LATCbits.LATC0; } while(0)
#define PICTPS1_GetValue()           PORTCbits.RC0
#define PICTPS1_SetDigitalInput()    do { TRISCbits.TRISC0 = 1; } while(0)
#define PICTPS1_SetDigitalOutput()   do { TRISCbits.TRISC0 = 0; } while(0)
#define PICTPS1_SetPullup()          do { WPUCbits.WPUC0 = 1; } while(0)
#define PICTPS1_ResetPullup()        do { WPUCbits.WPUC0 = 0; } while(0)
#define PICTPS1_SetPushPull()        do { ODCONCbits.ODCC0 = 0; } while(0)
#define PICTPS1_SetOpenDrain()       do { ODCONCbits.ODCC0 = 1; } while(0)
#define PICTPS1_SetAnalogMode()      do { ANSELCbits.ANSELC0 = 1; } while(0)
#define PICTPS1_SetDigitalMode()     do { ANSELCbits.ANSELC0 = 0; } while(0)

// get/set PICAPPS2 aliases
#define PICAPPS2_TRIS                 TRISCbits.TRISC1
#define PICAPPS2_LAT                  LATCbits.LATC1
#define PICAPPS2_PORT                 PORTCbits.RC1
#define PICAPPS2_WPU                  WPUCbits.WPUC1
#define PICAPPS2_OD                   ODCONCbits.ODCC1
#define PICAPPS2_ANS                  ANSELCbits.ANSELC1
#define PICAPPS2_SetHigh()            do { LATCbits.LATC1 = 1; } while(0)
#define PICAPPS2_SetLow()             do { LATCbits.LATC1 = 0; } while(0)
#define PICAPPS2_Toggle()             do { LATCbits.LATC1 = ~LATCbits.LATC1; } while(0)
#define PICAPPS2_GetValue()           PORTCbits.RC1
#define PICAPPS2_SetDigitalInput()    do { TRISCbits.TRISC1 = 1; } while(0)
#define PICAPPS2_SetDigitalOutput()   do { TRISCbits.TRISC1 = 0; } while(0)
#define PICAPPS2_SetPullup()          do { WPUCbits.WPUC1 = 1; } while(0)
#define PICAPPS2_ResetPullup()        do { WPUCbits.WPUC1 = 0; } while(0)
#define PICAPPS2_SetPushPull()        do { ODCONCbits.ODCC1 = 0; } while(0)
#define PICAPPS2_SetOpenDrain()       do { ODCONCbits.ODCC1 = 1; } while(0)
#define PICAPPS2_SetAnalogMode()      do { ANSELCbits.ANSELC1 = 1; } while(0)
#define PICAPPS2_SetDigitalMode()     do { ANSELCbits.ANSELC1 = 0; } while(0)

// get/set PICAPPS1 aliases
#define PICAPPS1_TRIS                 TRISCbits.TRISC2
#define PICAPPS1_LAT                  LATCbits.LATC2
#define PICAPPS1_PORT                 PORTCbits.RC2
#define PICAPPS1_WPU                  WPUCbits.WPUC2
#define PICAPPS1_OD                   ODCONCbits.ODCC2
#define PICAPPS1_ANS                  ANSELCbits.ANSELC2
#define PICAPPS1_SetHigh()            do { LATCbits.LATC2 = 1; } while(0)
#define PICAPPS1_SetLow()             do { LATCbits.LATC2 = 0; } while(0)
#define PICAPPS1_Toggle()             do { LATCbits.LATC2 = ~LATCbits.LATC2; } while(0)
#define PICAPPS1_GetValue()           PORTCbits.RC2
#define PICAPPS1_SetDigitalInput()    do { TRISCbits.TRISC2 = 1; } while(0)
#define PICAPPS1_SetDigitalOutput()   do { TRISCbits.TRISC2 = 0; } while(0)
#define PICAPPS1_SetPullup()          do { WPUCbits.WPUC2 = 1; } while(0)
#define PICAPPS1_ResetPullup()        do { WPUCbits.WPUC2 = 0; } while(0)
#define PICAPPS1_SetPushPull()        do { ODCONCbits.ODCC2 = 0; } while(0)
#define PICAPPS1_SetOpenDrain()       do { ODCONCbits.ODCC2 = 1; } while(0)
#define PICAPPS1_SetAnalogMode()      do { ANSELCbits.ANSELC2 = 1; } while(0)
#define PICAPPS1_SetDigitalMode()     do { ANSELCbits.ANSELC2 = 0; } while(0)

// get/set RC3 procedures
#define RC3_SetHigh()            do { LATCbits.LATC3 = 1; } while(0)
#define RC3_SetLow()             do { LATCbits.LATC3 = 0; } while(0)
#define RC3_Toggle()             do { LATCbits.LATC3 = ~LATCbits.LATC3; } while(0)
#define RC3_GetValue()              PORTCbits.RC3
#define RC3_SetDigitalInput()    do { TRISCbits.TRISC3 = 1; } while(0)
#define RC3_SetDigitalOutput()   do { TRISCbits.TRISC3 = 0; } while(0)
#define RC3_SetPullup()             do { WPUCbits.WPUC3 = 1; } while(0)
#define RC3_ResetPullup()           do { WPUCbits.WPUC3 = 0; } while(0)
#define RC3_SetAnalogMode()         do { ANSELCbits.ANSELC3 = 1; } while(0)
#define RC3_SetDigitalMode()        do { ANSELCbits.ANSELC3 = 0; } while(0)

// get/set RC4 procedures
#define RC4_SetHigh()            do { LATCbits.LATC4 = 1; } while(0)
#define RC4_SetLow()             do { LATCbits.LATC4 = 0; } while(0)
#define RC4_Toggle()             do { LATCbits.LATC4 = ~LATCbits.LATC4; } while(0)
#define RC4_GetValue()              PORTCbits.RC4
#define RC4_SetDigitalInput()    do { TRISCbits.TRISC4 = 1; } while(0)
#define RC4_SetDigitalOutput()   do { TRISCbits.TRISC4 = 0; } while(0)
#define RC4_SetPullup()             do { WPUCbits.WPUC4 = 1; } while(0)
#define RC4_ResetPullup()           do { WPUCbits.WPUC4 = 0; } while(0)
#define RC4_SetAnalogMode()         do { ANSELCbits.ANSELC4 = 1; } while(0)
#define RC4_SetDigitalMode()        do { ANSELCbits.ANSELC4 = 0; } while(0)

// get/set START aliases
#define START_TRIS                 TRISCbits.TRISC5
#define START_LAT                  LATCbits.LATC5
#define START_PORT                 PORTCbits.RC5
#define START_WPU                  WPUCbits.WPUC5
#define START_OD                   ODCONCbits.ODCC5
#define START_ANS                  ANSELCbits.ANSELC5
#define START_SetHigh()            do { LATCbits.LATC5 = 1; } while(0)
#define START_SetLow()             do { LATCbits.LATC5 = 0; } while(0)
#define START_Toggle()             do { LATCbits.LATC5 = ~LATCbits.LATC5; } while(0)
#define START_GetValue()           PORTCbits.RC5
#define START_SetDigitalInput()    do { TRISCbits.TRISC5 = 1; } while(0)
#define START_SetDigitalOutput()   do { TRISCbits.TRISC5 = 0; } while(0)
#define START_SetPullup()          do { WPUCbits.WPUC5 = 1; } while(0)
#define START_ResetPullup()        do { WPUCbits.WPUC5 = 0; } while(0)
#define START_SetPushPull()        do { ODCONCbits.ODCC5 = 0; } while(0)
#define START_SetOpenDrain()       do { ODCONCbits.ODCC5 = 1; } while(0)
#define START_SetAnalogMode()      do { ANSELCbits.ANSELC5 = 1; } while(0)
#define START_SetDigitalMode()     do { ANSELCbits.ANSELC5 = 0; } while(0)

// get/set SDC aliases
#define SDC_TRIS                 TRISCbits.TRISC6
#define SDC_LAT                  LATCbits.LATC6
#define SDC_PORT                 PORTCbits.RC6
#define SDC_WPU                  WPUCbits.WPUC6
#define SDC_OD                   ODCONCbits.ODCC6
#define SDC_ANS                  ANSELCbits.ANSELC6
#define SDC_SetHigh()            do { LATCbits.LATC6 = 1; } while(0)
#define SDC_SetLow()             do { LATCbits.LATC6 = 0; } while(0)
#define SDC_Toggle()             do { LATCbits.LATC6 = ~LATCbits.LATC6; } while(0)
#define SDC_GetValue()           PORTCbits.RC6
#define SDC_SetDigitalInput()    do { TRISCbits.TRISC6 = 1; } while(0)
#define SDC_SetDigitalOutput()   do { TRISCbits.TRISC6 = 0; } while(0)
#define SDC_SetPullup()          do { WPUCbits.WPUC6 = 1; } while(0)
#define SDC_ResetPullup()        do { WPUCbits.WPUC6 = 0; } while(0)
#define SDC_SetPushPull()        do { ODCONCbits.ODCC6 = 0; } while(0)
#define SDC_SetOpenDrain()       do { ODCONCbits.ODCC6 = 1; } while(0)
#define SDC_SetAnalogMode()      do { ANSELCbits.ANSELC6 = 1; } while(0)
#define SDC_SetDigitalMode()     do { ANSELCbits.ANSELC6 = 0; } while(0)

// get/set OUTM aliases
#define OUTM_TRIS                 TRISCbits.TRISC7
#define OUTM_LAT                  LATCbits.LATC7
#define OUTM_PORT                 PORTCbits.RC7
#define OUTM_WPU                  WPUCbits.WPUC7
#define OUTM_OD                   ODCONCbits.ODCC7
#define OUTM_ANS                  ANSELCbits.ANSELC7
#define OUTM_SetHigh()            do { LATCbits.LATC7 = 1; } while(0)
#define OUTM_SetLow()             do { LATCbits.LATC7 = 0; } while(0)
#define OUTM_Toggle()             do { LATCbits.LATC7 = ~LATCbits.LATC7; } while(0)
#define OUTM_GetValue()           PORTCbits.RC7
#define OUTM_SetDigitalInput()    do { TRISCbits.TRISC7 = 1; } while(0)
#define OUTM_SetDigitalOutput()   do { TRISCbits.TRISC7 = 0; } while(0)
#define OUTM_SetPullup()          do { WPUCbits.WPUC7 = 1; } while(0)
#define OUTM_ResetPullup()        do { WPUCbits.WPUC7 = 0; } while(0)
#define OUTM_SetPushPull()        do { ODCONCbits.ODCC7 = 0; } while(0)
#define OUTM_SetOpenDrain()       do { ODCONCbits.ODCC7 = 1; } while(0)
#define OUTM_SetAnalogMode()      do { ANSELCbits.ANSELC7 = 1; } while(0)
#define OUTM_SetDigitalMode()     do { ANSELCbits.ANSELC7 = 0; } while(0)

/**
   @Param
    none
   @Returns
    none
   @Description
    GPIO and peripheral I/O initialization
   @Example
    PIN_MANAGER_Initialize();
 */
void PIN_MANAGER_Initialize (void);

/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handling routine
 * @Example
    PIN_MANAGER_IOC();
 */
void PIN_MANAGER_IOC(void);



#endif // PIN_MANAGER_H
/**
 End of File
*/