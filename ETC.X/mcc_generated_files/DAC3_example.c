/*
    (c) 2016 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

#include "mcc.h"
#include "DAC3.h"
#include "DAC3_example.h"
#include <stdio.h>

void DAC3_example(void)
{
    float voltage;
    float readVoltage, readNonvolatileVoltage;
    
    uint16_t dacVoltage, dacNonvolatileVoltage;
    
    //Set volatile and nonvolatile voltage values to 1V
    voltage = 1;
    //Calculating dac value, see equation 4-1 in datasheet
    dacNonvolatileVoltage = (4096*voltage)/5;
    DAC3_SetNonvolatile(dacNonvolatileVoltage);
    
    printf("Set the non-volatile voltage to 1V \n");
    
    __delay_ms(100);

    //Set volatile voltage value to 2V
    voltage = 2;
    dacVoltage = (4096*voltage)/5;
    DAC3_Set(dacVoltage);

    __delay_ms(100);
    
    printf("Set the volatile voltage to 2V \n");

    dacVoltage = DAC3_Read(&dacNonvolatileVoltage);
    
    //Calculating voltage value, see equation 4-1 in datasheet, 819.2 = 4096/5
    readVoltage = dacVoltage / 819.2;
    readNonvolatileVoltage = dacNonvolatileVoltage / 819.2;
    
    printf("Read: non-volatile voltage value %f, and volatile voltage value %f", readNonvolatileVoltage, readVoltage);
    
    __delay_ms(100);
}

