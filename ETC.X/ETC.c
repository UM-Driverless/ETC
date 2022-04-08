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

// APPS Variables - Defaults, real-time values, states
unsigned int uiAPPS1_default_mv; // int -> 16 bit in MPLAB, PIC18. short = int here.
unsigned int uiAPPS2_default_mv;
unsigned int uiAPPS1_opened_mv;
unsigned int uiAPPS2_opened_mv;

unsigned int ucAPPS1_mv; // Sent by CAN
unsigned int ucAPPS2_mv; // Sent by CAN
unsigned char ucAPPS1_perc;
unsigned char ucAPPS2_perc;
unsigned char ucAPPS_perc;

unsigned char ucAPPS_STATE;
unsigned char ucAPPSManual;

// TPS Variables - Defaults, real-time values, states
unsigned int uiTPS1_default_mv;
unsigned int uiTPS2_default_mv;
unsigned int uiTPS1_opened_mv;
unsigned int uiTPS2_opened_mv;

unsigned int uiTPS1_mv; // Global because it depends on interruptions. (TEMPORIZATIONS.c)
unsigned int uiTPS2_mv;
unsigned char ucTPS1_perc;
unsigned char ucTPS2_perc;
unsigned char ucTPS_perc;

unsigned char ucTPS_STATE;
unsigned char ucTPS1_STATE;
unsigned char ucTPS2_STATE;
unsigned char ucTPS_Volts_STATE;
unsigned int uiETCDuty;
unsigned char ucETB_STATE;
unsigned char ucETCBeatSupervisor = FALSE;
unsigned char ucETCFlagSupervisor = FALSE;


/// APPS functions
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
    /* Get limit values to get percentage later.
     *
     */
    
    uiAPPS1_default_mv = ucAPPS1_mv + APPSMARGEN;
    uiAPPS2_default_mv = ucAPPS2_mv - APPSMARGEN;
    
    // We have to push the accelerator pedal to get these values, so assign constants for now.
    uiAPPS1_opened_mv = APPS1_OPEN_MV;
    uiAPPS2_opened_mv = APPS2_OPEN_MV;
}

void APPSAnalysis(void) { // Called by TEMPORIZATIONS.c
    /* Converts ucAPPS1_mv and ucAPPS2_mv into ucAPPS_perc
     * 
     */
    
    ucAPPS1_perc = perc_of(ucAPPS1_mv, uiAPPS1_default_mv, uiAPPS1_opened_mv);
    ucAPPS2_perc = 100 - perc_of(ucAPPS2_mv, uiAPPS2_default_mv, uiAPPS2_opened_mv); // Tries to mimic ucAPPS1_perc
    ucAPPS_perc = (ucAPPS1_perc + ucAPPS2_perc) / 2;
}

/// Electronic Throttle - Intake functions
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
void ETCMove(signed long sl_target_perc, unsigned char ucMode) {
    //Depender de beat constante en CAN
    if ( ucETCFlagSupervisor == TRUE )
    {
        //HACER CONVERSION DE 0-100% A 2-12 DUTY
        uiETCDuty = sl_target_perc;
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
    uiTPS1_default_mv = ANALOG_GetVoltage(ENT_TPS1);
    uiTPS2_default_mv = ANALOG_GetVoltage(ENT_TPS2);
    // Let it read
    __delay_ms(100);
    
    Nop();
    
    // Intake motor 100%
    GPIO_PWM2_Control(100, 600); // 100% PWM at 600Hz, Motor to max power.
    // Let it move
    __delay_ms(500);
    // Read TPS sensors at max opened position.
    uiTPS1_opened_mv = ANALOG_GetVoltage(ENT_TPS1);
    uiTPS2_opened_mv = ANALOG_GetVoltage(ENT_TPS2);
    // Let it read
    __delay_ms(100);
    Nop();
    
    // Calibration sound
    GPIO_PWM2_Control(50, 400);
    __delay_ms(200);
    GPIO_PWM2_Control(50, 600);
    __delay_ms(200);
    
    // Turn off after calibration
    GPIO_PWM2_Control(0, 600); // 0% PWM at 600Hz, Motor OFF.
}

void TPSAnalysis(void) {
    /* Calculate ucTPS_perc from uiTPS1_mv and uiTPS2_mv of the intake sensor (Throttle Position Sensor)
     * Called by TEMPORIZATIONS.c. right after ANALOGRead(). 
     */
        
    // Calculate fraction of travel. Sensor 1 and 2 provide voltages of constant average. They move in opposite directions.
    ucTPS1_perc = perc_of(uiTPS1_mv, uiTPS1_default_mv, uiTPS1_opened_mv);
    ucTPS2_perc = 100 - perc_of(uiTPS2_mv, uiTPS2_default_mv, uiTPS2_opened_mv); // Tries to mimic ucTPS1_perc
    ucTPS_perc = (ucTPS1_perc + ucTPS2_perc) / 2 ;
    
    // TODO - STOP HERE AND CHECK VALUES
    Nop();
    
    // TODO - If too different, CAN error.
    
    // ucTPS_perc is the average.
        
    /// Analysis of TPS error when out of range.
//    if ( ( slTPS1calc > uiTPS1_mv + TPSMARGEN ) || ( slTPS1calc < uiTPS1_mv - TPSMARGEN ) )
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
//    if ( ( slTPS2calc > uiTPS2_mv + TPSMARGEN ) || ( slTPS2calc < uiTPS2_mv - TPSMARGEN ) )
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
        if ( ucETCBeatSupervisor == TRUE ) {
            ucETCFlagSupervisor = TRUE; // Allow movement (from Autonomous System)
        }
        else {
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

void ETCManual(unsigned char ucTargetManual) {
    if ( ucASMode == ManualMode ) {
        ETCMove(ucTargetManual, ManualMode);
    }
}

void ETC_PID(signed long sl_target_perc, unsigned char ucMode) {
    /*
     *
     * 
     * sl_target_perc is the target position of the throttle, in 0-100% = default-fully opened?
     * ucMode is the driving mode input to the function. It will be checked with the global variable 
     */
    
    // TODO REMOVE AND CALIBRATE VALUES, use PARAMETERS.h #define constants
    // Static local variables to change values at run time for calibration.
    static signed long sl_K_P = 1000;
    static signed long sl_K_I = 10;
    static signed long sl_K_D = 0;
    static signed long slIntegral = 0;
    signed long slDerivative;
    signed long sl_motor_pwm_duty = 0;
    static signed long slLastErrorPos;
    static signed long  slErrorPos;
    
    Nop();
    
    // Only let the motor move if CAN beat flag is OK. - TODO MOVE THESE CONDITIONALS TO THE MAIN.C FILE?
    if ( ucETCFlagSupervisor == TRUE ) {
        
        /// MAIN CALCULATION
        
        // ui_tp1_mv is the current position. Usually 16-55 for default-open.
        
        // TODO ---- USE ucTPS_perc INSTEAD OF uiTPS1_mv
        // TODO: 1. play sound of ucTPS_perc to check. 2. Use ucTPS_perc INSTEAD OF uiTPS1_mv
        slErrorPos = (signed long)(sl_target_perc) - ( (signed long)(uiTPS1_mv) - 1212 )*100 / (3126-1212);  // in % or what units? TODO RJM
        slIntegral += slErrorPos;
        slDerivative = slErrorPos - slLastErrorPos;
        
        // Bound the values of slIntegral - TODO bounds to PARAMETERS.h, or remove if not needed.
//        if (slIntegral > (1<<20)){
//            slIntegral = 1<<20;
//        } else if (slIntegral < -32768){
//            slIntegral = 0;
//        }
        
        sl_motor_pwm_duty = sl_K_P * slErrorPos + sl_K_I * slIntegral + sl_K_D * slDerivative; // TODO complete
        sl_motor_pwm_duty /= 1000; // Because the constants are long, not floats        
        
        // Crop between 0 and 100. It's a duty cycle.
        if ( sl_motor_pwm_duty < 0 ) {
            sl_motor_pwm_duty = 0;
        }
        else if ( sl_motor_pwm_duty > 100 ) {
            sl_motor_pwm_duty = 100;
        }
        
        Nop();
        
        // MAIN MOVEMENT - DEPENDS ON ASMode - TODO remove conditionals to make it faster?
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
    } else {
        // ET Motor OFF
        GPIO_PWM2_Control(0, 600);
    }
}

unsigned char perc_of(signed long val, signed long min, signed long max) {
    /*
     * Returns the percentage of val between min and max. Result bounded between 0 and 100.
     * 
     * If val is out of the range [min,max], then it will be capped at 0% or 100%.
     * If min is greater than max, the closest val is to min, the closest the result will be to 0%, and viceversa.
     * If val exceeds the range going through min, it will be 0%. It if goes through max, it will be 100%, even if min > max.
     * A higher value will be 0% in this case.
     */
    val = 100*(val - min)/(max - min);
    if (val < 0){
        val = 0;
    } else if (val > 100 ){
        val = 100;
    } // val is now contained in the interval [0,100] -> ok to assign to unsigned char
    return val;
}