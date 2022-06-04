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
#include "ANALOG.h"

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
signed long ulTPS1calc;
signed long ulTPS2calc; 
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
    uiAPPS1min = uiAPPS1 + APPSMARGEN;
    uiAPPS2min = uiAPPS2 - APPSMARGEN;
}

void APPSReadmax (void)
{
    //Lo leido en sensores APPS se queda guardado como valor maximo
    uiAPPS1max = APPS1max - APPSMARGEN;
    uiAPPS2max = APPS2max + APPSMARGEN;
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
                GPIO_PWM2_Control(uiETCDuty + 34, 600); //lo muevo sin comprobar nada
            }
            else if ( ucASMode == ManualMode )
            {
                //hay que ver como meter aqui la conexion con TPS y APPS
                GPIO_PWM2_Control(uiETCDuty, 600); //lo muevo sin comprobar nada
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

void ETC_PID(signed long slTargetMove, unsigned char ucMode) 
{
    /*
     * Conditional logic to get target position out of this function, along with checks that it's ok to run it. PID just does PID.
     * 
     * sl_target_perc is the target position of the throttle, in 0-100% = default-fully opened?
     * ucMode is the driving mode input to the function. It will be checked with the global variable 
     */
    
    // TODO REMOVE AND CALIBRATE VALUES, use PARAMETERS.h #define constants
    // Static local variables to change values at run time for calibration.
    // Test with sl_target_perc ~= 80%, first K_I until it works with delay, then K_P to cancel those oscillations, then K_D to cancel the K_P oscillations.
    static signed long sl_K = 50000; // Just to compensate the spring, which has approximately constant force, start with 50% of PWM duty cycle. The intake throttle is designed to go with the same force in both directions.
    static signed long sl_K_P = 800; // 1000 2.1 starts to be unstable. 1.0 barely moves it. 1.5 is the right value. Multiply by divider value.
    static signed long sl_K_I = 3; // 0.003 -> Small because it integrates.
    static signed long sl_K_D = 0; // 20.0 to 200.0. If the error increases, you need to add power.
    static signed long slIntegral = 0; // Usually in the range of 1e5 - 1e6
    signed long slDerivative;
    signed long slMotorPwmDuty = 0;
    static signed long  slErrorPos = 0; // Start as 0, then update every cycle.
//    static signed long slLastErrorPos = 0;
    static signed long slLastTPSPerc = 0;
    
    // Standard constants: K = 50 (%), K_P = 1.5, K_D = 20, K_I = 3
    
    // Position error:
    slErrorPos = slTargetMove - ucTPS;  // in % or what units? TODO RJM ( (signed long)(uiTPS1_mv) - 1212 )*100 / (3126-1212)
    
    // Integral Error. Cap the values of the integral to avoid windup: https://en.wikipedia.org/wiki/Integral_windup, https://www.mstarlabs.com/apeng/techniques/pidsoftw.html
    if ((slIntegral > -1e4) && (slIntegral < 1e4))
    {
        slIntegral += slErrorPos;
    }
    
    // Derivative Error. Only make it work when descending? I don't feel like it does anything.
    if (ucTPS < slLastTPSPerc)
    {
        slDerivative = slLastTPSPerc - ucTPS; // Behaves like friction. Ignores the input.
    }
    slLastTPSPerc = ucTPS; // For next iteration
    
    slMotorPwmDuty = sl_K + sl_K_P * slErrorPos + sl_K_I * slIntegral + sl_K_D * slDerivative;
    Nop();
    slMotorPwmDuty /= 1024; // 1024 ~= 1000. Do it because the constants are long, not floats

    // Crop between 0 and 100. It's a duty cycle.
    if (slMotorPwmDuty < 0) 
    {
        slMotorPwmDuty = 0;
    }
    else if ( slMotorPwmDuty > 100 ) 
    {
        slMotorPwmDuty = 100;
    }
    
    Nop();
    
    // APPLY THE DUTY CYCLE TO THE MOTOR - Variable checks.
    // Only let the motor move if CAN beat flag is OK
    // TODO Consider doing this externally, and giving a value to PID function depending on state. PID just moves to target position. Otherwise we need several target positions into the function.
    if ( ucETCFlagSupervisor == TRUE ) // OK no errors...
    { 
        if ( ucMode == ucASMode ) // The mode sent to the PID function and the global variable agree
        { 
            if (ucASMode == ASMode)
            {
                // Drive with autonomous value
                GPIO_PWM2_Control(slMotorPwmDuty, 600);
            } 
            else if (ucASMode == ManualMode)
            {
                GPIO_PWM2_Control(slMotorPwmDuty, 600); // Usually this
            } 
            else 
            {
                // ERROR. Wrong value of ucASMode
            }
        } 
        else 
        {
            // ERROR. Movement prevented by driving mode
        } 
    } 
    else 
    {
        GPIO_PWM2_Control(0, 600);
    }
}

void ETCCalibrate(void) {
    /*
     * Test the movement of the ETB (Electronic Throttle Body) at startup, 
     * and Calibrate the minimum and maximum values of TPS1 and TPS2 (Throttle Position Sensor)
     * of the car intake
    */
    
    // Intake motor OFF - Barely Open - Engine Idle
    GPIO_PWM2_Control(0, 600); // Motor OFF. 0% PWM at 600Hz 
    // Let it move
    __delay_ms(200);
    // Read TPS sensor at default position.
    ANALOGRead(); // Calls ANALOG_GetVoltage(), assigns values to uiAPPS, uiTPS
    // Let it read
    __delay_ms(100);
    uiTPS1min = uiTPS1 + TPSMARGEN; // Smallest value
    uiTPS2min = uiTPS2 - TPSMARGEN; // Biggest value
    
    Nop();
    
    // Intake motor 100%
    GPIO_PWM2_Control(100, 600); // 100% PWM at 600Hz, Motor to max power.
    // Let it move
    __delay_ms(500);
    // Read TPS sensors at max opened position.
    ANALOGRead(); // Calls ANALOG_GetVoltage(), assigns values to uiAPPS, uiTPS
    // Let it read
    __delay_ms(100);
    uiTPS1max = uiTPS1 + TPSMARGEN; // Biggest value
    uiTPS2max = uiTPS2 - TPSMARGEN; // Smallest  value
    Nop();
    
    // Calibration sound
    GPIO_PWM2_Control(20, 400);
    __delay_ms(200);
    GPIO_PWM2_Control(20, 600);
    __delay_ms(200);
    
    // Turn off after calibration
    GPIO_PWM2_Control(0, 300); // 0% PWM at 600Hz, Motor OFF.
}

void TPSAnalysis(void)
{
    //Analisis TPS1
    /*if ( uiTPS1min < uiTPS1max )    //TPS1 voltaje no invertido
    {
        ucTPS1Perc = ETCPercentCalc (uiTPS1, uiTPS1max, uiTPS1min);
        ucTPS_Volts_STATE = TPS1_NO_INVERTED;
    }
    else    //TPS1 voltaje invertido
    {
        ucTPS1Perc = 100-ETCPercentCalc (uiTPS1, uiTPS1min, uiTPS1max);
        ucTPS_Volts_STATE = TPS1_INVERTED;
    }
    
    if ( uiTPS2min < uiTPS2max )    //TPS2 voltaje no invertido
    {
        ucTPS2Perc = ETCPercentCalc (uiTPS2, uiTPS2max, uiTPS2min);
        ucTPS_Volts_STATE = TPS2_NO_INVERTED;
    }
    else    //TPS2 voltaje invertido
    {
        ucTPS2Perc = 100-ETCPercentCalc (uiTPS2, uiTPS2max, uiTPS2min);
        ucTPS_Volts_STATE = TPS2_INVERTED;
    }*/
    
    
    ucTPS1Perc = ETCPercentCalc (uiTPS1, uiTPS1min, uiTPS1max);
    ucTPS2Perc = ETCPercentCalc (uiTPS2, uiTPS2min, uiTPS2max);
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
    /*if ( uiAPPS1min < uiAPPS1max )    //APPS1 voltaje no invertido
    {
        ucAPPS1Perc = ETCPercentCalc (uiAPPS1, uiAPPS1min, uiAPPS1max);
    }
    else    //APPS1 voltaje invertido
    {
        ucAPPS1Perc = 100-ETCPercentCalc (uiAPPS1, uiAPPS1max, uiAPPS1min);
    }
    //Analisis APPS2
    if ( uiAPPS2min < uiAPPS2max )    //APPS2 voltaje no invertido
    {
        ucAPPS2Perc = 100-ETCPercentCalc (uiAPPS2, uiAPPS2max, uiAPPS2min);
    }
    else    //APPS2 voltaje invertido
    {
        ucAPPS2Perc = ETCPercentCalc (uiAPPS2, uiAPPS2min, uiAPPS2max);
    }*/
    
    ucAPPS1Perc = ETCPercentCalc (uiAPPS1, uiAPPS1min, uiAPPS1max);
    ucAPPS2Perc = ETCPercentCalc (uiAPPS2, uiAPPS2min, uiAPPS2max);
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
    else if ( ucASMode == ManualMode )
    {
        ucETCFlagSupervisor = TRUE; //PERMITO MOVIMIENTO
    }
    
}


void ETCManual (unsigned char ucTargetManual)
{
    if ( ucASMode == ManualMode )
    {
        ETCMove(ucTargetManual, ManualMode);
        //ETC_PID(ucTargetManual, ManualMode);
    }
}

//Declaracion funciones
unsigned char ETCPercentCalc(signed long val, signed long min, signed long max) 
{
    /*Returns the percentage of val between min and max. Result bounded between 0 and 100.
     * If val is out of the range [min,max], then it will be capped at 0% or 100%.
     * If min is greater than max, the closest val is to min, the closest the result will be to 0%, and viceversa.
     * If val exceeds the range going through min, it will be 0%. It if goes through max, it will be 100%, even if min > max.
     * A higher value will be 0% in this case.
     */
    val = 100*(val - min)/(max - min);
    if (val < 0)
    {
        val = 0;
    } 
    else if (val > 100 )
    {
        val = 100;
    } 
    // val is now contained in the interval [0,100] -> ok to assign to unsigned char
    return val;
}