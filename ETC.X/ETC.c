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
unsigned int uiAPPS1min;
unsigned int uiAPPS1max;    
unsigned int uiAPPS2min;
unsigned int uiAPPS2max;  
unsigned int uiTPS1min;
unsigned int uiTPS1max;    
unsigned int uiTPS2min;
unsigned int uiTPS2max; 
unsigned int uiAPPS1;
unsigned int uiAPPS2;   
unsigned char ucAPPS_STATE; 
unsigned long ulAPPS1calc;
unsigned long ulAPPS2calc; 
unsigned char ucAPPS1Perc;
unsigned char ucAPPS2Perc; 
unsigned char ucAPPS; 
unsigned int uiTPS1;
unsigned int uiTPS2; 
unsigned long ulTPS1calc;
unsigned long ulTPS2calc; 
unsigned char ucTPS1Perc;
unsigned char ucTPS2Perc; 
unsigned char ucTPS; 
unsigned char ucTPS_STATE; 
unsigned char ucTPS1_STATE; 
unsigned char ucTPS2_STATE; 
unsigned char ucTPS_Volts_STATE; 
unsigned int uiETCDuty;
unsigned char ucETB_STATE; 
unsigned char ucETCBeatSupervisor = FALSE; 
unsigned char ucETCFlagSupervisor = FALSE; 
unsigned char ucAPPSManual;
signed char scLastErrorPos;
signed char scErrorPos;

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
    uiAPPS1min = uiAPPS1+APPSMARGEN;
    uiAPPS2min = uiAPPS2-APPSMARGEN;
}

void APPSReadmax (void)
{
    //Lo leido en sensores APPS se queda guardado como valor maximo
    uiAPPS1max = APPS1max;
    uiAPPS2max = APPS2max;
}

void TPSReadmin (void)
{
    //Lo leido en sensores TPS se queda guardado como valor minimo
    uiTPS1min = uiTPS1;
    uiTPS2min = uiTPS2;
}

void TPSReadmax (void)
{
    //Lo leido en sensores TPS se queda guardado como valor maximo
    uiTPS1max = uiTPS1;
    uiTPS2max = uiTPS2;
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

    
}

//Funcion para mover directamente el servo con PWM
void ETCMove(unsigned char ucTargetMove, unsigned char ucMode)
{
    //Depender de beat constante en CAN
    if ( ucETCFlagSupervisor == TRUE )
    {
        //HACER CONVERSION DE 0-100% A 2-12 DUTY
        uiETCDuty = ucTargetMove;
        //nos tenemos que asegurar antes de mover que aceptamos ordenes de manual o autonomo
        if ( ucMode == ucASMode )
        {
            if ( ucASMode == ASMode ) 
            {
                //aumentar un 10% para asegurar un ralenti siempre, quiza ajustarlo con rpm
                GPIO_PWM2_Control(uiETCDuty + 34, 300); //lo muevo sin comprobar nada
            }
            else if ( ucASMode == ManualMode )
            {
                //hay que ver como meter aqui la conexion con TPS y APPS
                GPIO_PWM2_Control(uiETCDuty, 300); //lo muevo sin comprobar nada
            }
            else
            {
                //la variable ucMode esta corrupto
            }

        }
        else
        {
            //generar error movimiento impedido por modo de conduccion
        }
    } 
    else 
    {
        GPIO_PWM2_Control(0, 600); //lo muevo sin comprobar nada
    }
}


//Probar movimiento de ETB en arranques
void ETCInitMove(void)
{
    //Analizar aqui valores minimos de APPS
    TPSReadmin();
    Nop();
    GPIO_PWM2_Control(0, 600); //lo muevo sin comprobar nada
    __delay_ms(200);
    GPIO_PWM2_Control(100, 600); //lo muevo sin comprobar nada
    __delay_ms(1000);
    TPSReadmax();
    __delay_ms(1000);
    Nop();
    __delay_ms(200);
    GPIO_PWM2_Control(0, 600); //lo muevo sin comprobar nada
    Nop();
}

void TPSAnalysis (void)
{
    //Analisis TPS1
    if ( uiTPS1min < uiTPS1max )    //TPS1 voltaje no invertido
    {
        //ulTPS1calc = ( ( uiTPS1max - uiTPS1min ) * uiETCDuty ) + uiTPS1min;
        ulTPS1calc = ( uiTPS1max - uiTPS1min );
        ulTPS1calc = ( ulTPS1calc * uiETCDuty );
        ulTPS1calc = ( ( ulTPS1calc / 100 ) + uiTPS1min );
        
        ucTPS_Volts_STATE = TPS1_NO_INVERTED;
    }
    else    //TPS1 voltaje invertido
    {
        //ulTPS1calc = ( uiTPS1min - ( ( uiTPS1min - uiTPS1max ) * uiETCDuty ) );
        ulTPS1calc = ( uiTPS1min - uiTPS1max );
        ulTPS1calc = ( ulTPS1calc * uiETCDuty );
        ulTPS1calc = ( ( uiTPS1min * 100 ) - ulTPS1calc );
        
        ucTPS_Volts_STATE = TPS1_INVERTED;
    }
    
    //Analisis TPS2
    if ( uiTPS2min < uiTPS2max )    //TPS2 voltaje no invertido
    {
        //ulTPS2calc = ( ( uiTPS2max - uiTPS2min ) * uiETCDuty ) + uiTPS2min;
        ulTPS2calc = ( uiTPS2max - uiTPS2min );
        ulTPS2calc = ( ulTPS2calc * uiETCDuty );
        ulTPS2calc = ( ( ulTPS2calc / 100 ) + uiTPS2min );
        ucTPS_Volts_STATE = TPS2_NO_INVERTED;
    }
    else    //TPS2 voltaje invertido
    {
        //ulTPS2calc = ( uiTPS2min - ( ( uiTPS2min - uiTPS2max ) * uiETCDuty ) );
        ulTPS2calc = ( uiTPS2min - uiTPS2max );
        ulTPS2calc = ( ulTPS2calc * uiETCDuty );
        ulTPS2calc = ( ( uiTPS2min * 100 ) - ulTPS2calc );
        ucTPS_Volts_STATE = TPS2_INVERTED;
    }
    
    ucTPS1Perc = ( ulTPS1calc & 0x00007F );
    ucTPS2Perc = ( ulTPS2calc & 0x00007F );
    ucTPS = ( ( ucTPS1Perc + ucTPS2Perc ) / 2 );
    Nop();
    
    
    //analisis de fallos TPS por salida de márgenes
    if ( ( ulTPS1calc > uiTPS1 + TPSMARGEN ) || ( ulTPS1calc < uiTPS1 - TPSMARGEN ) )
    {
        //apuntar fallo de TPS1
        ucTPS_STATE |= TPS1_ERROR;
    }
    else
    {
        //quitar error TPS1
        ucTPS_STATE |= QUITAR_ERROR_TPS1;
    }
    
    if ( ( ulTPS2calc > uiTPS2 + TPSMARGEN ) || ( ulTPS2calc < uiTPS2 - TPSMARGEN ) )
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
    //Analisis APPS1
    Nop();
    __delay_ms(50);
    Nop();
    Nop();
    Nop();
    if ( uiAPPS1min < uiAPPS1max )    //APPS1 voltaje no invertido
    {
        //DEBERIAMOS SACAR ERROR PORQUE EL CALCULO SALE MAL POR LOS LIMITES
        //ulAPPS1calc = ((uiAPPS1-uiAPPS1min)/(uiAPPS1min-uiAPPS1max))*100;
        ulAPPS1calc = (uiAPPS1-uiAPPS1min);
        ulAPPS1calc = ulAPPS1calc*100;
        ulAPPS1calc = (ulAPPS1calc/(uiAPPS1min-uiAPPS1max));
    }
    else    //APPS1 voltaje invertido
    {
        //ulAPPS1calc = ((uiAPPS1min-uiAPPS1)/(uiAPPS1min-uiAPPS1max))*100;
        ulAPPS1calc = (uiAPPS1min-uiAPPS1);
        ulAPPS1calc = ulAPPS1calc*100;
        ulAPPS1calc = (ulAPPS1calc/(uiAPPS1min-uiAPPS1max));
    }
    //Analisis APPS2
    if ( uiAPPS2min < uiAPPS2max )    //APPS2 voltaje no invertido
    {
        //ulAPPS2calc = ((uiAPPS2-uiAPPS2min)/(uiAPPS2max-uiAPPS2min))*100;
        ulAPPS2calc = (uiAPPS2-uiAPPS2min);
        ulAPPS2calc = ulAPPS2calc*100;
        ulAPPS2calc = (ulAPPS2calc/(uiAPPS2max-uiAPPS2min));
    }
    else    //APPS2 voltaje invertido
    {
        //DEBERIAMOS SACAR ERROR PORQUE EL CALCULO SALE MAL POR LOS LIMITES
        //ulAPPS2calc = ((uiAPPS2min-uiAPPS2)/(uiAPPS2min-uiAPPS2max))*100;
        ulAPPS2calc = (uiAPPS2min-uiAPPS2);
        ulAPPS2calc = ulAPPS2calc*100;
        ulAPPS2calc = (ulAPPS2calc/(uiAPPS2min-uiAPPS2max));
    }
    
    ucAPPS1Perc = ( ulAPPS1calc & 0x00007F );
    ucAPPS2Perc = ( ulAPPS2calc & 0x00007F );
    ucAPPS = ( ( ucAPPS1Perc + ucAPPS2Perc ) / 2 );
    Nop();
}


void ETCSupervisor (void)
{
    Nop();
    if ( ucASMode == ASMode )
    {
        if ( ucETCBeatSupervisor == TRUE )
        {
            ucETCFlagSupervisor = TRUE; //PERMITO MOVIMIENTO
        }
        else
        {
            ucETCFlagSupervisor = FALSE; //NO PERMITO MOVER 
            //poner embrague y ETC a cero
            GPIO_PWM1_Control(0, 300);
            GPIO_PWM2_Control(0, 600);
        }
    }
    
}


void ETCManual (unsigned char ucTargetManual)
{
    if ( ucASMode == ManualMode )
    {
        ETCMove(ucTargetManual, ManualMode);
    }
}

void ETC_PIDcontroller (unsigned char ucTargetMove, unsigned char ucMode)
{
    unsigned char ucCurrentPos;
    signed char scIntegral;
    signed char scDerivative;
    signed char scPropPart;
    signed char scIntPart;
    signed char scDerPart;
    signed char scMotorPWM;
    
    //Depender de beat constante en CAN
    if ( ucETCFlagSupervisor == TRUE )
    {              
        ucCurrentPos = ucTPS;
        scErrorPos = ucTargetMove - ucCurrentPos;
        scIntegral += scErrorPos;
        scDerivative = scErrorPos - scLastErrorPos;
        scPropPart = ETC_KP * scErrorPos;
        scIntPart = ETC_KI * scIntegral;
        scDerPart = ETC_KD * scDerivative;
        scMotorPWM = scPropPart + scIntPart + scDerPart;
        
        if ( scMotorPWM <= 0 )
        {
            scMotorPWM = 0;
        }
        else if ( ( scMotorPWM > 0 ) && ( scMotorPWM < 100 ) )
        {
            scMotorPWM = scMotorPWM;
        }
        else if ( scMotorPWM > 100 )
        {
            scMotorPWM = 100;
        }
        else   
        {
            //no debería entrar nunca, pero si lo hace ojo
        }
        
        //nos tenemos que asegurar antes de mover que aceptamos ordenes de manual o autonomo
        if ( ucMode == ucASMode )
        {
            if ( ucASMode == ASMode ) 
            {
               ETCMove(scMotorPWM, ASMode);
            }
            else if ( ucASMode == ManualMode )
            {
               ETCManual(scMotorPWM);
            }
            else
            {
                //la variable ucMode esta corrupto
            }

        }
        else
        {
            //generar error movimiento impedido por modo de conduccion
        }
        
        scLastErrorPos = scErrorPos;
    } 
    else 
    {
        GPIO_PWM2_Control(0, 600); //lo muevo sin comprobar nada
    }
}