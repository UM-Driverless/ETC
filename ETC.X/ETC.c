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
#include "ETC.h"
#include "GPIO.h"
#include "PARAMETERS.h"

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
unsigned char ucTPS1calc;
unsigned char ucTPS2calc; 
unsigned char ucTPS_STATE; 
unsigned char ucTPS1_STATE; 
unsigned char ucTPS2_STATE; 
unsigned char ucTPS_Volts_STATE; 
unsigned int uiETCDuty;

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
            //APPSSend(0x00); //da error el I2C generando inf interrupciones
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
    TPSAnalysis();
    
}

//Funcion para mover directamente el servo con PWM
void ETCMove(unsigned char ucTargetMove, unsigned char ucMode)
{
    //HACER CONVERSION DE 0-100% A 2-12 DUTY
    uiETCDuty = ucTargetMove;
    //nos tenemos que asegurar antes de mover que aceptamos ordenes de manual o autonomo
    if ( ucMode == ucASMode )
    {
        if ( ucMode == ASMode ) 
        {
            GPIO_PWM2_Control(uiETCDuty, 300); //lo muevo sin comprobar nada
        }
        else
        {
            //hay que ver como meter aqui la conexion con TPS y APPS
        }
        
    }
    else
    {
        //generar error movimiento impedido por modo de conduccion
    }
    
}


//Probar movimiento de ETB en arranques
void ETCInitMove(void)
{
    //Analizar aqui valores minimos de APPS
    TPSReadmin();
    GPIO_PWM2_Control(0, 300); //lo muevo sin comprobar nada
    __delay_ms(200);
    GPIO_PWM2_Control(100, 300); //lo muevo sin comprobar nada
    __delay_ms(1000);
    TPSReadmax();
    __delay_ms(200);
    GPIO_PWM2_Control(0, 300); //lo muevo sin comprobar nada
}

void TPSAnalysis (void)
{
    //Analisis TPS1
    if ( ucTPS1min < ucTPS1max )    //TPS1 voltaje no invertido
    {
        ucTPS1calc = ( ( ucTPS1max - ucTPS1min ) * uiETCDuty ) + ucTPS1min;
        ucTPS_Volts_STATE = TPS1_NO_INVERTED;
    }
    else    //TPS1 voltaje invertido
    {
        ucTPS1calc = ( ucTPS1min - ( ( ucTPS1min - ucTPS1max ) * uiETCDuty ) );
        ucTPS_Volts_STATE = TPS1_INVERTED;
    }
    
    //Analisis TPS2
    if ( ucTPS2min < ucTPS2max )    //TPS2 voltaje no invertido
    {
        ucTPS2calc = ( ( ucTPS2max - ucTPS2min ) * uiETCDuty ) + ucTPS2min;
        ucTPS_Volts_STATE = TPS2_NO_INVERTED;
    }
    else    //TPS2 voltaje invertido
    {
        ucTPS2calc = ( ucTPS2min - ( ( ucTPS2min - ucTPS2max ) * uiETCDuty ) );
        ucTPS_Volts_STATE = TPS2_INVERTED;
    }
    
    //analisis de fallos TPS por salida de márgenes
    if ( ( ucTPS1calc > ucTPS1 + TPSMARGEN ) || ( ucTPS1calc < ucTPS1 - TPSMARGEN ) )
    {
        //apuntar fallo de TPS1
        ucTPS_STATE |= TPS1_ERROR;
    }
    else
    {
        //quitar error TPS1
        ucTPS_STATE |= QUITAR_ERROR_TPS1;
    }
    
    if ( ( ucTPS2calc > ucTPS2 + TPSMARGEN ) || ( ucTPS2calc < ucTPS2 - TPSMARGEN ) )
    {
        //apuntar fallo de TPS2
        ucTPS_STATE |= TPS2_ERROR;
    }
    else
    {
        //quitar error TPS1
        ucTPS_STATE |= QUITAR_ERROR_TPS2;
    }
    
    //analisis por inversiones de voltaje 
    if ( ucTPS_Volts_STATE == TPS1_NO_INVERTED_TPS2_NO_INVERTED )
    {
        ucTPS_STATE |= TPS_Volt_ERROR;
    }
    else if ( ucTPS_Volts_STATE == TPS1_NO_INVERTED_TPS2_INVERTED )
    {
        //todo OK, eliminar erro de estado
        ucTPS_STATE &= QUITAR_ERROR_VOLTS;
    }
    else if ( ucTPS_Volts_STATE == TPS1_INVERTED_TPS2_NO_INVERTED )
    {
        //todo OK, eliminar erro de estado
        ucTPS_STATE &= QUITAR_ERROR_VOLTS;
    }
    else if ( ucTPS_Volts_STATE == TPS1_INVERTED_TPS2_INVERTED )
    {
        ucTPS_STATE |= TPS_Volt_ERROR;
    }
    else
    {
        ucTPS_STATE |= TPS_Volt_ERROR;
    }
    
    //analisis por diferencias entre TPS1 y TPS2
    
    
}



void APPSAnalysis (void)
{
    
}