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

/// Global Variables
unsigned int uiAPPS1min; // int -> 16 bit in MPLAB, PIC18. short = int here.
unsigned int uiAPPS1max;
unsigned int uiAPPS2min;
unsigned int uiAPPS2max;

unsigned int ui_tps1_default; // Called by TEMPORIZATIONS.c
unsigned int uiTPS1_opened; // Called by TEMPORIZATIONS.c
unsigned int ui_tps2_default; // Called by TEMPORIZATIONS.c
unsigned int uiTPS2_opened; // Called by TEMPORIZATIONS.c

unsigned int uiAPPS1; // Sent by CAN
unsigned int uiAPPS2; // Sent by CAN
unsigned char ucAPPS_STATE;
unsigned long ulAPPS1calc;
unsigned long ulAPPS2calc;
unsigned char ucAPPS1Perc;
unsigned char ucAPPS2Perc;
unsigned char ucAPPS;
unsigned int ui_tps1_mv; // Global because it depends on interruptions. (TEMPORIZATIONS.c)
unsigned int ui_tps2_mv;
unsigned char uc_tps1_perc;
unsigned char uc_tps2_perc;
unsigned char uc_tps_perc; //ucTPS
unsigned char ucTPS_STATE;
unsigned char ucTPS1_STATE;
unsigned char ucTPS2_STATE;
unsigned char ucTPS_Volts_STATE;
unsigned int uiETCDuty;
unsigned char ucETB_STATE;
unsigned char ucETCBeatSupervisor = FALSE;
unsigned char ucETCFlagSupervisor = FALSE;
unsigned char ucAPPSManual;
signed long  slErrorPos;


/// APPS
void APPSSend(unsigned char ucPercent) {
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

void apps_calibrate(void){
    /*Estableciendo los valores limite se puede establecer una relación lineal para
     *conseguir calcular la nueva función de transferencia cada vez que se tare el sistema
     *
     *
     */
    
    uiAPPS1min = uiAPPS1+APPSMARGEN;
    uiAPPS2min = uiAPPS2-APPSMARGEN;
    
    // We have to push the accelerator pedal to get these values, so assign constants for now.
    uiAPPS1max = APPS1max;
    uiAPPS2max = APPS2max;
}

void APPSAnalysis(void) { // Called by TEMPORIZATIONS.c
    // Analysis APPS1
    Nop();
    if ( uiAPPS1min < uiAPPS1max ) // Plugged normally
    {
        //DEBERIAMOS SACAR ERROR PORQUE EL CALCULO SALE MAL POR LOS LIMITES
        Nop();
        ulAPPS1calc = (uiAPPS1-uiAPPS1min)/(uiAPPS1max - uiAPPS1min)*100;
    }
    else // Plugged inverted
    {
        //ulAPPS1calc = ((uiAPPS1min-uiAPPS1)/(uiAPPS1min-uiAPPS1max))*100;
        ulAPPS1calc = (uiAPPS1min-uiAPPS1)/(uiAPPS1min-uiAPPS1max)*100;
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

/// Electronic Throttle - Intake.
// Run every time the autonomous mode message is received.
void ETCModeSelect(unsigned char ucModeSelect) {
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

// Normative supervision function - TODO
void ETCRulesSupervision(void)
{

    
}

//Funcion para mover directamente el servo con PWM
void ETCMove(signed long slTargetMove, unsigned char ucMode) {
    //Depender de beat constante en CAN
    if ( ucETCFlagSupervisor == TRUE )
    {
        //HACER CONVERSION DE 0-100% A 2-12 DUTY
        uiETCDuty = slTargetMove;
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

void etc_calibrate(void) {
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
    ui_tps1_default = ANALOG_GetVoltage(ENT_TPS1);
    ui_tps2_default = ANALOG_GetVoltage(ENT_TPS2);
    // Let it read
    __delay_ms(100);
    
    Nop();
    
    // Intake motor 100%
    GPIO_PWM2_Control(100, 600); // 100% PWM at 600Hz, Motor to max power.
    // Let it move
    __delay_ms(500);
    // Read TPS sensor at max opened position.
    uiTPS1_opened = ANALOG_GetVoltage(ENT_TPS1);
    uiTPS2_opened = ANALOG_GetVoltage(ENT_TPS2);
    // Let it read
    __delay_ms(100);
    Nop();
    
    // Turn off after calibration
    GPIO_PWM2_Control(0, 600); // 0% PWM at 600Hz, Motor OFF.

    // Calibration sound
    GPIO_PWM2_Control(50, 400);
    __delay_ms(200);
    GPIO_PWM2_Control(50, 600);
    __delay_ms(200);
}

void TPSAnalysis(void) // Called by TEMPORIZATIONS.c. right after ANALOGRead(). 
{
    /* Calculate ucTPS from ucTPS1 and ucTPS2 of the intake sensor (Throttle Position Sensor)
     *
     */
    
    /*
     //    if ( ui_tps1_default < uiTPS1_opened ) // Plugged normally
//    {
//        //slTPS1calc = ( ( uiTPS1_opened - ui_tps1_default ) * uiETCDuty ) + ui_tps1_default;
//        Nop();
//        slTPS1calc = ( uiTPS1_opened - ui_tps1_default );
//        slTPS1calc = ( slTPS1calc * uiETCDuty );
//        slTPS1calc = ( ( slTPS1calc / 100 ) + ui_tps1_default );
//        
//        slTPS1calc = ( ui_tps1_mv - ui_tps1_default)*100/(uiTPS1_opened - ui_tps1_default);
//        ucTPS_Volts_STATE = TPS1_NO_INVERTED;
//    } // TODO else plugged inverted... Ignore for now
    // ucTPS_Volts_STATE = TPS1_NO_INVERTED;
    
    
//    // Analysis TPS2
//    if ( ui_tps2_default < uiTPS2_opened )    //TPS2 voltaje no invertido
//    {
//        //slTPS2calc = ( ( uiTPS2_opened - ui_tps2_default ) * uiETCDuty ) + ui_tps2_default;
//        slTPS2calc = ( uiTPS2_opened - ui_tps2_default );
//        slTPS2calc = ( slTPS2calc * uiETCDuty );
//        slTPS2calc = ( ( slTPS2calc / 100 ) + ui_tps2_default );
//        ucTPS_Volts_STATE = TPS2_NO_INVERTED;
//    }
     */
    
    
    // Calculate fraction of travel. Sensor 1 and 2 provide voltages of constant average. They move in opposite directions.
    uc_tps1_perc = 100* (signed long)(ui_tps1_mv - ui_tps1_default) / (signed long)(uiTPS1_opened - ui_tps1_default);
    uc_tps2_perc = 100* (signed long)(ui_tps2_mv - ui_tps2_default) / (signed long)(uiTPS2_opened - ui_tps2_default);
    
    // TODO - CHECK THAT THEY MATCH, If too different, CAN error.
    
    // Crop values from 0 to 100. It's a percentage.
    if (uc_tps1_perc > 100){
        uc_tps1_perc = 100;
    } else if (uc_tps1_perc < 0){
        uc_tps1_perc = 0;
    }
    
    if (uc_tps2_perc > 100){
        uc_tps2_perc = 100;
    } else if (uc_tps2_perc < 0){
        uc_tps2_perc = 0;
    }
    // uc_tps_perc is the average.
    uc_tps_perc = ( ( uc_tps1_perc + uc_tps2_perc ) / 2 );
    
    Nop();
    
    /// Analysis of TPS error when out of range.
//    if ( ( slTPS1calc > ui_tps1_mv + TPSMARGEN ) || ( slTPS1calc < ui_tps1_mv - TPSMARGEN ) )
//    {
//        //apuntar fallo de TPS1
//        ucTPS_STATE |= TPS1_ERROR;
//    }
//    else
//    {
//        //quitar error TPS1
//        ucTPS_STATE |= QUITAR_ERROR_TPS1;
//    }
//    
//    if ( ( slTPS2calc > ui_tps2_mv + TPSMARGEN ) || ( slTPS2calc < ui_tps2_mv - TPSMARGEN ) )
//    {
//        //apuntar fallo de TPS2
//        ucTPS_STATE |= TPS2_ERROR;
//    }
//    else
//    {
//        //quitar error TPS1
//        ucTPS_STATE |= QUITAR_ERROR_TPS2;
//    }
//    
//    // Analysis by voltage inversion
//    if ( ucTPS_Volts_STATE == TPS1_NO_INVERTED_TPS2_NO_INVERTED )
//    {
//        ucTPS_STATE |= TPS_Volt_ERROR;
//    }
//    else if ( ucTPS_Volts_STATE == TPS1_NO_INVERTED_TPS2_INVERTED )
//    {
//        //todo OK, eliminar erro de estado
//        ucTPS_STATE &= QUITAR_ERROR_VOLTS;
//    }
//    else if ( ucTPS_Volts_STATE == TPS1_INVERTED_TPS2_NO_INVERTED )
//    {
//        //todo OK, eliminar erro de estado
//        ucTPS_STATE &= QUITAR_ERROR_VOLTS;
//    }
//    else if ( ucTPS_Volts_STATE == TPS1_INVERTED_TPS2_INVERTED )
//    {
//        ucTPS_STATE |= TPS_Volt_ERROR;
//    }
//    else
//    {
//        ucTPS_STATE |= TPS_Volt_ERROR;
//    }
    
    // Analysis by difference between TPS1 and TPS2
    
    
}

void ETCSupervisor(void) {
    Nop();
    if ( ucASMode == ASMode ) {
        if ( ucETCBeatSupervisor == TRUE )
        {
            ucETCFlagSupervisor = TRUE; // Allow movement (from Autonomous System)
        }
        else
        {
            ucETCFlagSupervisor = FALSE; // Error. Don't allow movement
            // Clutch to 0
            GPIO_PWM1_Control(0, 300);
            // Intake to idle (motor off)
            GPIO_PWM2_Control(0, 300);
        }
    } else if ( ucASMode == ManualMode) {
        ucETCFlagSupervisor = TRUE; // Allow movement (from pedal)
    }
    
}

void ETCManual(unsigned char ucTargetManual)
{
    if ( ucASMode == ManualMode )
    {
        ETCMove(ucTargetManual, ManualMode);
    }
}

void ETC_PIDcontroller(signed long slTargetMove, unsigned char ucMode) {
    /*
     *
     * 
     * slTargetMove is the target position of the throttle, in 0-100% = default-fully opened?
     * ucMode is the driving mode input to the function. It will be checked with the global variable 
     */
    
    // TODO REMOVE AND CALIBRATE VALUES, use PARAMETERS.h #define constants
    // Static local variables to change values at run time for calibration.
    static signed long sl_K_P = 1000;
    static signed long sl_K_I = 10;
    static signed long sl_K_D = 0;
    static signed long slIntegral = 0;
    signed long slDerivative;
    signed long sl_motor_pwm_duty = 0; // TODO CHANGED!!
    static signed long slLastErrorPos;
    
    Nop();
    
    
    // Only let the motor move if CAN beat flag is OK.
    if ( ucETCFlagSupervisor == TRUE ) {
        
        /// MAIN CALCULATION
        
        // TODO map_perc(value,min,max)... to map a value between min and max as a percentage
        // ui_tp1_mv is the current position. Usually 16-55 for default-open.
        
        // TODO ---- USE uc_tps_perc INSTEAD OF ui_tps1_mv        
        slErrorPos = (signed long)(slTargetMove) - ( (signed long)(ui_tps1_mv) - 1212 )*100 / (3126-1212);  // in % or what units? TODO RJM
        slIntegral += slErrorPos;
        slDerivative = slErrorPos - slLastErrorPos;
        
//        sl_motor_pwm_duty = ETC_KP * slErrorPos + ETC_KI * slIntegral + ETC_KD * slDerivative;
        sl_motor_pwm_duty = sl_K_P * slErrorPos + sl_K_I * slIntegral + sl_K_D * slDerivative; // TODO complete
        sl_motor_pwm_duty /= 1000; // Because the constants are long, not floats
//        sl_motor_pwm_duty /= 100;
        
        
        // Crop between 0 and 100. It's a duty cycle.
        if ( sl_motor_pwm_duty < 0 ) {
            sl_motor_pwm_duty = 0;
        }
        else if ( sl_motor_pwm_duty > 100 ) {
            sl_motor_pwm_duty = 100;
        }
        
      
        // fprintf(stderr,"%i\n",slErrorPos);
        Nop();
        
        // MAIN MOVEMENT - DEPENDS ON ASMode¡
        if ( ucMode == ucASMode ) {
            if ( ucASMode == ASMode ) {
                //ETCMove(sl_motor_pwm_duty, ASMode); // TODO UNCOMMENT AND MAKE IT WORK
                GPIO_PWM2_Control(sl_motor_pwm_duty, 600);
            }
            else if ( ucASMode == ManualMode ) {
                //ETCMove(sl_motor_pwm_duty, ASMode); // TODO UNCOMMENT AND MAKE IT WORK
//                ETCMove(sl_motor_pwm_duty, ManualMode);
                GPIO_PWM2_Control(sl_motor_pwm_duty, 600);
            }
            else {
                // ERROR: Wrong value
            }
        } else {
            // ERROR: Movement prevented by driving mode
        }
        
         // Set error as last error for next iteration
        slLastErrorPos = slErrorPos;
    } 
    else {
        // ET Motor OFF
        GPIO_PWM2_Control(0, 600);
    }
}

/// TESTING
void sensor_sound(void){
    GPIO_PWM2_Control(30, uiAPPS1); // Play as frequency the value of the accelerator pedal.
//    GPIO_PWM2_Control(50, uiAPPS2); // Play as frequency the value of the accelerator pedal.
    
//    GPIO_PWM2_Control(50, ui_tps1_mv); // Play as frequency the value of the Electronic Throttle Motor.
//    GPIO_PWM2_Control(50, ui_tps2_mv); // Play as frequency the value of the Electronic Throttle Motor.
    
    
}