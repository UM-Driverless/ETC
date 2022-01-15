/* 
 * File:   ETC.h
 * Author: 93Urbano
 *
 * Created on December 24, 2021, 6:15 PM
 */

#include "mcc_generated_files/DAC3.h"
#include "mcc_generated_files/drivers/i2c_simple_master.h"
#include "MESSAGES.h"
#include "mcc_generated_files/DAC3_example.h"

//VARIABLES    
unsigned char ucAPPS1min;
unsigned char ucAPPS1max;    
unsigned char ucAPPS2min;
unsigned char ucAPPS2max;
unsigned char ucTPS1min;
unsigned char ucTPS1max;    
unsigned char ucTPS2min;
unsigned char ucTPS2max; 
unsigned char ucAPPS1;
unsigned char ucAPPS2;    
unsigned char ucTPS1;
unsigned char ucTPS2; 

//FUNCIONES
void APPSSend (unsigned char ucPercent)
{
    float voltage;
    uint16_t dacAPPS1, dacAPPS2;
    
    //Funcion de transferencia de porcentaje a valor APPS
    
    
    dacAPPS1 = (4096*ucPercent)/5;
    dacAPPS2 = (4096*ucPercent)/5;
    //APPS1 I2C ADRRS = 0x60
    i2c_write2ByteRegister(0x60,(dacAPPS1>>8),dacAPPS1);
    //APPS2 I2C ADRRS = 0x61
    i2c_write2ByteRegister(0x61,(dacAPPS2>>8),dacAPPS2);
    DAC3_example();
}


//Estableciendo los valores limite se puede establecer una relación lineal para
//conseguir calcular la nueva función de transferencia cada vez que se tare el sistema
void APPSReadmin (void)
{
    //Lo leido en sensores APPS se queda guardado como valor minimo
    ucAPPS1min = ucAPPS1;
    ucAPPS2min = ucAPPS2;
}

void APPSReadmax (void)
{
    //Lo leido en sensores APPS se queda guardado como valor maximo
    ucAPPS1max = ucAPPS1;
    ucAPPS2max = ucAPPS2;
}

void TPSReadmin (void)
{
    //Lo leido en sensores TPS se queda guardado como valor minimo
    ucTPS1min = ucTPS1;
    ucTPS2min = ucTPS2;
}

void TPSReadmax (void)
{
    //Lo leido en sensores TPS se queda guardado como valor maximo
    ucTPS1max = ucTPS1;
    ucTPS2max = ucTPS2;
}

//Ejecutar cada vez que llegue el mensaje de modo autonomo
void ETCModeSelect (unsigned char ucModeSelect)
{
    switch (ucModeSelect)
    {
        case ASMode:
            //Iniciar con 0%
            APPSSend(0x00);
            APPSMODE_SetHigh();
            break;
        case ManualMode:
            APPSMODE_SetLow();
            break;
        default:    //poner modo manual porsi
            APPSMODE_SetLow();
            break;
    }
}

//Funcion supervision de normativa
void ETCRulesSupervision(void)
{
    
}