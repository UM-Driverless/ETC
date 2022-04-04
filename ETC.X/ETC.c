/* 
 * File:   ETC.h
 * Author: 93Urbano
 *
 * Created on December 24, 2021, 6:15 PM
 * 
 * ET = Electronic Throttle
 * ETC = Electronic Throttle Control
 * ETB = Electronic Throttle Body
 * 
 */

#include "mcc_generated_files/DAC3.h"
#include "mcc_generated_files/drivers/i2c_simple_master.h"
#include "MESSAGES.h"
#include "mcc_generated_files/DAC3_example.h"
#include "ETC.h"
#include "GPIO.h"
#include "PARAMETERS.h"
#include "ANALOG.h"

// VARIABLES
unsigned int uiAPPS1min; // int -> 16 bit in MPLAB, PIC18. short = int here.
unsigned int uiAPPS1max;
unsigned int uiAPPS2min;
unsigned int uiAPPS2max;
unsigned int uiTPS1_default;
unsigned int uiTPS1_opened;
unsigned int uiTPS2_default;
unsigned int uiTPS2_opened;
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


// FUNCIONES
void APPSSend(unsigned char ucPercent)
{
    float voltage;
    uint16_t dacAPPS1, dacAPPS2;
    // Funcion de transferencia de porcentaje a valor APPS
    dacAPPS1 = (4096*ucPercent)/5;
    dacAPPS2 = (4096*ucPercent)/5;
    //APPS1 I2C ADRRS = 0x60
    i2c_write2ByteRegister(0x60,(dacAPPS1>>8),dacAPPS1);
    //APPS2 I2C ADRRS = 0x61
    i2c_write2ByteRegister(0x61,(dacAPPS2>>8),dacAPPS2);
    DAC3_example();
}


// Estableciendo los valores limite se puede establecer una relación lineal para
// conseguir calcular la nueva función de transferencia cada vez que se tare el sistema
void APPSReadmin(void)
{
    //ANALOGRead(); // Commented because you only need to read APPS, not TPS.
    // Lo leido en sensores APPS se queda guardado como valor minimo
    uiAPPS1min = uiAPPS1+APPSMARGEN;
    uiAPPS2min = uiAPPS2-APPSMARGEN;
}

void APPSReadmax(void)
{
    //Lo leido en sensores APPS se queda guardado como valor maximo
    uiAPPS1max = APPS1max;
    uiAPPS2max = APPS2max;
}

// Ejecutar cada vez que llegue el mensaje de modo autonomo
void ETCModeSelect(unsigned char ucModeSelect)
{
    switch (ucModeSelect)
    {
        case ASMode:
            // Iniciar con 0%
            //APPSSend(0x00); // da error el I2C generando inf interrupciones
            APPSMODE_SetHigh();
            break;
        case ManualMode:
            APPSMODE_SetLow();
            break;
        default:    // poner modo manual porsi
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



void ETCInitMove(void) {
    /*
     * Test the movement of the ETB (Electronic Throttle Body) at startup, 
     * and Calibrate the minimum and maximum values of TPS1 and TPS2 (Throttle Position Sensor)
     * of the car intake
    */
    
    // ETB motor OFF - Barely Open - Engine Idle
    GPIO_PWM2_Control(0, 600); // Motor OFF. 0% PWM at 600Hz 
    __delay_ms(200); // Let it move
    tps_read_default(); // Read TPS sensor at default position
    __delay_ms(100); // Let it read
    Nop();
    
    // ETB motor 100%
    GPIO_PWM2_Control(100, 600); // 100% PWM at 600Hz, Motor to max power.
    __delay_ms(500); // Let it move
    tps_read_opened(); // Read TPS sensor at max opened position
    __delay_ms(100); // Let it read
    Nop();
    
    // Turn off after calibration
    GPIO_PWM2_Control(0, 600); // 0% PWM at 600Hz, Motor OFF.

    // Calibration sound
    GPIO_PWM2_Control(50, 400);
    __delay_ms(200);
    GPIO_PWM2_Control(50, 600);
    __delay_ms(200);
}

void tps_read_default(void)
{
    // Read TPS1 and TPS2 values
    ANALOGRead(); // Read APPS1 APPS2 TPS1 TPS2
    
    // Define the values for default position
    uiTPS1_default = uiTPS1; // uiTPS1 // ANALOG_GetVoltage(ENT_TPS1)
    uiTPS2_default = uiTPS2; // uiTPS2 // ANALOG_GetVoltage(ENT_TPS2)
    
    /// RJM Why not this? Don't call ANALOGRead() and just update the important values. I don't understand why it goes crazy with the same default values.
//    uiTPS1_default = ANALOG_GetVoltage(ENT_TPS1);
//    uiTPS2_default = ANALOG_GetVoltage(ENT_TPS2);
    
}

void tps_read_opened(void)
{
    ANALOGRead(); // Read APPS1 APPS2 TPS1 TPS2
    
    // Define the values for fully opened position
    uiTPS1_opened = uiTPS1;
    uiTPS2_opened = uiTPS2;
}

void TPSAnalysis(void) // Called by TEMPORIZATIONS.c
{
    // Analysis TPS1
    Nop();
    if ( uiTPS1_default < uiTPS1_opened )    //TPS1 voltaje no invertido
    {
        //ulTPS1calc = ( ( uiTPS1_opened - uiTPS1_default ) * uiETCDuty ) + uiTPS1_default;
        Nop();
        /*ulTPS1calc = ( uiTPS1_opened - uiTPS1_default );
        ulTPS1calc = ( ulTPS1calc * uiETCDuty );
        ulTPS1calc = ( ( ulTPS1calc / 100 ) + uiTPS1_default );
         */
        ulTPS1calc = ( uiTPS1 - uiTPS1_default );
        ulTPS1calc *= 100;
        ulTPS1calc = ( ulTPS1calc /  (uiTPS1_opened - uiTPS1_default ));
        ucTPS_Volts_STATE = TPS1_NO_INVERTED;
    }
    else    //TPS1 voltaje invertido
    {
        Nop();
        //ulTPS1calc = ( uiTPS1_default - ( ( uiTPS1_default - uiTPS1_opened ) * uiETCDuty ) );
        ulTPS1calc = ( uiTPS1_default - uiTPS1 );
        ulTPS1calc *= 100;
        ulTPS1calc = ulTPS1calc / (uiTPS1_default - uiTPS1_opened);
        ucTPS_Volts_STATE = TPS1_INVERTED;
    }
    
    // Analysis TPS2
    if ( uiTPS2_default < uiTPS2_opened )    //TPS2 voltaje no invertido
    {
        //ulTPS2calc = ( ( uiTPS2_opened - uiTPS2_default ) * uiETCDuty ) + uiTPS2_default;
        ulTPS2calc = ( uiTPS2_opened - uiTPS2_default );
        ulTPS2calc = ( ulTPS2calc * uiETCDuty );
        ulTPS2calc = ( ( ulTPS2calc / 100 ) + uiTPS2_default );
        ucTPS_Volts_STATE = TPS2_NO_INVERTED;
    }
    else    //TPS2 voltaje invertido
    {
        Nop();
        //ulTPS1calc = ( uiTPS1_default - ( ( uiTPS1_default - uiTPS1_opened ) * uiETCDuty ) );
        ulTPS2calc = ( uiTPS2_default - uiTPS2 );
        ulTPS2calc *= 100;
        ulTPS2calc = ulTPS2calc / (uiTPS2_default - uiTPS2_opened);
        ucTPS_Volts_STATE = TPS1_INVERTED;
    }
    
    ucTPS1Perc = ( ulTPS1calc & 0x00007F );
    ucTPS2Perc = ( ulTPS2calc & 0x00007F );
    ucTPS = ( ( ucTPS1Perc + ucTPS2Perc ) / 2 );
    Nop();
    // Analysis de fallos TPS por salida de márgenes
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
    
    // Analysis by voltage inversion
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
    
    // Analysis by difference between TPS1 and TPS2
    
    
}

void APPSAnalysis(void) // Called by TEMPORIZATIONS.c
{
    // Analysis APPS1
    Nop();
    if ( uiAPPS1min < uiAPPS1max )    //APPS1 voltaje no invertido
    {
        //DEBERIAMOS SACAR ERROR PORQUE EL CALCULO SALE MAL POR LOS LIMITES
        //ulAPPS1calc = ((uiAPPS1-uiAPPS1min)/(uiAPPS1min-uiAPPS1max))*100;
        Nop();
        ulAPPS1calc = (uiAPPS1-uiAPPS1min);
        ulAPPS1calc = ulAPPS1calc*100;
//        ulAPPS1calc = (ulAPPS1calc/(uiAPPS1min-uiAPPS1max));
        ulAPPS1calc = (ulAPPS1calc/(uiAPPS1max - uiAPPS1min));
    }
    else    //APPS1 voltaje invertido
    {
        //ulAPPS1calc = ((uiAPPS1min-uiAPPS1)/(uiAPPS1min-uiAPPS1max))*100;
        ulAPPS1calc = (uiAPPS1min-uiAPPS1);
        ulAPPS1calc = ulAPPS1calc*100;
        ulAPPS1calc = (ulAPPS1calc/(uiAPPS1min-uiAPPS1max));
    }
    // Analysis APPS2
    if ( uiAPPS2min < uiAPPS2max )    //APPS2 voltaje no invertido
    {
        //ulAPPS2calc = ((uiAPPS2-uiAPPS2min)/(uiAPPS2max-uiAPPS2min))*100;
        Nop();
        ulAPPS2calc = (uiAPPS2-uiAPPS2min);
        ulAPPS2calc = ulAPPS2calc*100;
        ulAPPS2calc = (ulAPPS2calc/(uiAPPS2max-uiAPPS2min));
    }
    else    //APPS2 voltaje invertido
    {
        //DEBERIAMOS SACAR ERROR PORQUE EL CALCULO SALE MAL POR LOS LIMITES
        //ulAPPS2calc = ((uiAPPS2min-uiAPPS2)/(uiAPPS2min-uiAPPS2max))*100;
        Nop();
        ulAPPS2calc = (uiAPPS2min-uiAPPS2);
        ulAPPS2calc = ulAPPS2calc*100;
        ulAPPS2calc = (ulAPPS2calc/(uiAPPS2min-uiAPPS2max));
    }
    
    ucAPPS1Perc = ( ulAPPS1calc & 0x00007F );
    ucAPPS2Perc = ( ulAPPS2calc & 0x00007F );
    ucAPPS = ( ( ucAPPS1Perc + ucAPPS2Perc ) / 2 );
    Nop();
}


void ETCSupervisor(void)
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
            // poner embrague y ETC a cero
            GPIO_PWM1_Control(0, 300);
            GPIO_PWM2_Control(0, 600);
        }
    }
    else if ( ucASMode == ManualMode) {
        ucETCFlagSupervisor = TRUE; //PERMITO MOVIMIENTO
    }
    
}


void ETCManual(unsigned char ucTargetManual)
{
    if ( ucASMode == ManualMode )
    {
        ETCMove(ucTargetManual, ManualMode);
    }
}

void ETC_PIDcontroller(unsigned char ucTargetMove, unsigned char ucMode) {
    /*
     *
     * 
     * ucTargetMove is the target position of the throttle, in 0-100% = default-fully opened?
     * ucMode is the driving mode input to the function. It will be checked with the global variable 
     */
    
//    unsigned char ucCurrentPos = 0;
    signed char scIntegral = 0;
    signed char scDerivative = 0;
//    signed int scPropPart = 0;
//    signed int scIntPart = 0;
//    signed int scDerPart = 0;
    unsigned int siMotorPWM = 0; // CHANGED!! TODO signed
    Nop();
    
    // Depender de beat constante en CAN
    if ( ucETCFlagSupervisor == TRUE ) {              
//        ucCurrentPos = ucTPS;
//        scErrorPos = ucTargetMove - ucCurrentPos;
//        scIntegral += scErrorPos;
//        scDerivative = scErrorPos - scLastErrorPos;
//        scPropPart = ETC_KP * scErrorPos;
//        scIntPart = ETC_KI * scIntegral;
//        scDerPart = ETC_KD * scDerivative;
//        siMotorPWM = scPropPart + scIntPart + scDerPart;
        
        // calculate the position error, in % or what units? TODO RJM
        scErrorPos = ucTargetMove - ucTPS; // ucTPS is the current position
        scIntegral += scErrorPos;
        scDerivative = scErrorPos - scLastErrorPos;
        
        siMotorPWM = ETC_KP * scErrorPos + ETC_KI * scIntegral + ETC_KD * scDerivative;
//        siMotorPWM /= 100;
        
        Nop();
        
        if ( siMotorPWM <= 0 ) {
            siMotorPWM = 0;
        }
        else if ( ( siMotorPWM > 0 ) && ( siMotorPWM < 100 ) ) {
//            siMotorPWM = siMotorPWM; // RJM ?
        }
        else if ( siMotorPWM > 100 ) {
            siMotorPWM = 100;
        }
        else {
            // no debería entrar nunca, pero si lo hace ojo
        }
        
        // MAIN MOVEMENT - DEPENDS ON ASMode¡
        if ( ucMode == ucASMode ) {
            if ( ucASMode == ASMode ) {
                ETCMove(siMotorPWM, ASMode);
            }
            else if ( ucASMode == ManualMode ) {
                ETCMove(siMotorPWM, ManualMode);
            }
            else {
                // ERROR: Wrong value
            }
        } else {
            // ERROR: Movement prevented by driving mode
        }
        
        scLastErrorPos = scErrorPos;
    } 
    else {
        // ET Motor OFF
        GPIO_PWM2_Control(0, 600);
    }
}