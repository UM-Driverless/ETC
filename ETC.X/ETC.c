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

/// Local functions
void tps_read_default(void);
void tps_read_opened(void);

/// Global Variables
unsigned int uiAPPS1min; // int -> 16 bit in MPLAB, PIC18. short = int here.
unsigned int uiAPPS1max;
unsigned int uiAPPS2min;
unsigned int uiAPPS2max;

unsigned int uiTPS1_default; // Called by TEMPORIZATIONS.c
unsigned int uiTPS1_opened; // Called by TEMPORIZATIONS.c
unsigned int uiTPS2_default; // Called by TEMPORIZATIONS.c
unsigned int uiTPS2_opened; // Called by TEMPORIZATIONS.c

unsigned int uiAPPS1; // Sent by CAN
unsigned int uiAPPS2; // Sent by CAN
unsigned char ucAPPS_STATE;
unsigned long ulAPPS1calc;
unsigned long ulAPPS2calc;
unsigned char ucAPPS1Perc;
unsigned char ucAPPS2Perc;
unsigned char ucAPPS;
unsigned int ui_tps1_mv; // Sent by CAN
unsigned int ui_tps2_mv; // Sent by CAN
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


// Estableciendo los valores limite se puede establecer una relación lineal para
// conseguir calcular la nueva función de transferencia cada vez que se tare el sistema

void apps_calibrate(void){
    uiAPPS1min = uiAPPS1+APPSMARGEN;
    uiAPPS2min = uiAPPS2-APPSMARGEN;
    
    // We have to push the accelerator pedal to get these values, so assign constants for now.
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



void etc_calibrate(void) {
    /*
     * Test the movement of the ETB (Electronic Throttle Body) at startup, 
     * and Calibrate the minimum and maximum values of TPS1 and TPS2 (Throttle Position Sensor)
     * of the car intake
    */
    
    
    
    // ETB motor OFF - Barely Open - Engine Idle
    GPIO_PWM2_Control(0, 600); // Motor OFF. 0% PWM at 600Hz 
    __delay_ms(200); // Let it move
    tps_read_default(); // Read TPS sensor at default position. Local function.
    __delay_ms(100); // Let it read
    Nop();
    
    // ETB motor 100%
    GPIO_PWM2_Control(100, 600); // 100% PWM at 600Hz, Motor to max power.
    __delay_ms(500); // Let it move
    tps_read_opened(); // Read TPS sensor at max opened position. Local function.
    
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

void tps_read_default(void) {
    // Read TPS1 and TPS2 values
    ANALOGRead(); // Read APPS1 APPS2 TPS1 TPS2. ANALOGRead() is not called by interruptions until INTERRUPT_GlobalInterruptEnable();
    
    // Define the values for default position
    uiTPS1_default = ui_tps1_mv; // ANALOG_GetVoltage(ENT_TPS1)
    uiTPS2_default = ui_tps2_mv; // ANALOG_GetVoltage(ENT_TPS2)
    
    /// RJM Why not this? Don't call ANALOGRead() and just update the important values. I don't understand why it goes crazy with the same default values.
//    uiTPS1_default = ANALOG_GetVoltage(ENT_TPS1);
//    uiTPS2_default = ANALOG_GetVoltage(ENT_TPS2);
    
}

void tps_read_opened(void) {
    ANALOGRead(); // Read APPS1 APPS2 TPS1 TPS2
    // Define the values for fully opened position
    uiTPS1_opened = ui_tps1_mv;
    uiTPS2_opened = ui_tps2_mv;
    
}

void TPSAnalysis(void) // Called by TEMPORIZATIONS.c. right after ANALOGRead(). 
{
    /* Calculate ucTPS from ucTPS1 and ucTPS2 of the intake sensor (Throttle Position Sensor)
     *
     */
    
    /// Calculate ucTPS
//    if ( uiTPS1_default < uiTPS1_opened ) // Plugged normally
//    {
//        //slTPS1calc = ( ( uiTPS1_opened - uiTPS1_default ) * uiETCDuty ) + uiTPS1_default;
//        Nop();
//        /*slTPS1calc = ( uiTPS1_opened - uiTPS1_default );
//        slTPS1calc = ( slTPS1calc * uiETCDuty );
//        slTPS1calc = ( ( slTPS1calc / 100 ) + uiTPS1_default );
//         */
//        slTPS1calc = ( ui_tps1_mv - uiTPS1_default)*100/(uiTPS1_opened - uiTPS1_default);
//        ucTPS_Volts_STATE = TPS1_NO_INVERTED;
//    } // TODO else plugged inverted... Ignore for now
    // ucTPS_Volts_STATE = TPS1_NO_INVERTED;
    
    
//    // Analysis TPS2
//    if ( uiTPS2_default < uiTPS2_opened )    //TPS2 voltaje no invertido
//    {
//        //slTPS2calc = ( ( uiTPS2_opened - uiTPS2_default ) * uiETCDuty ) + uiTPS2_default;
//        slTPS2calc = ( uiTPS2_opened - uiTPS2_default );
//        slTPS2calc = ( slTPS2calc * uiETCDuty );
//        slTPS2calc = ( ( slTPS2calc / 100 ) + uiTPS2_default );
//        ucTPS_Volts_STATE = TPS2_NO_INVERTED;
//    }
    
    // Calculate fraction of travel. Sensor 1 and 2 provide voltages of constant average. They move in opposite directions.
    uc_tps1_perc = 100* (signed long)(ui_tps1_mv - uiTPS1_default) / (signed long)(uiTPS1_opened - uiTPS1_default);
    uc_tps2_perc = 100* (signed long)(ui_tps2_mv - uiTPS2_default) / (signed long)(uiTPS2_opened - uiTPS2_default);
    
    // TODO - CHECK THAT THEY MATCH.
    
    // Just take the 7 last bits, which correspond to 255, the current max value.
    uc_tps1_perc = (uc_tps1_perc & 0x0000007F);
    uc_tps2_perc = (uc_tps2_perc & 0x0000007F);
    
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

void APPSAnalysis(void) // Called by TEMPORIZATIONS.c
{
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


void ETCSupervisor(void)
{
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

void ETC_PIDcontroller(unsigned char ucTargetMove, unsigned char ucMode) {
    /*
     *
     * 
     * ucTargetMove is the target position of the throttle, in 0-100% = default-fully opened?
     * ucMode is the driving mode input to the function. It will be checked with the global variable 
     */
    
    // TODO REMOVE AND CALIBRATE VALUES, use PARAMETERS.h #define constants
    // Static K_P to change values at run time for calibration.
    static signed int K_P = 1000;
    static signed int K_I = 1;
    static signed int K_D;
    static signed long slIntegral = 0;
    signed long slDerivative = 0;
    signed long slMotorPWM = 0; // TODO CHANGED!!
    static signed long slLastErrorPos;
    
    Nop();
    
    
    // Only let the motor move if CAN beat flag is OK.
    if ( ucETCFlagSupervisor == TRUE ) {
        
        /// MAIN CALCULATION
        
        // TODO tps_map(ucTargetMove)... to map 16-55 to 0-100%
        // ucTPS is the current position. Usually 16-55 for default-open.
        // slErrorPos must be able to become negative!!!!
        
        slErrorPos = (signed long)(ucTargetMove) - ( (signed long)(ui_tps1_mv) - 1212 )*100 / (3126-1212);  // in % or what units? TODO RJM
        slIntegral += slErrorPos;
        slDerivative = slErrorPos - slLastErrorPos;
        
//        slMotorPWM = ETC_KP * slErrorPos + ETC_KI * slIntegral + ETC_KD * slDerivative;
        slMotorPWM = K_P * slErrorPos + K_I * slIntegral + K_D * slDerivative; // TODO complete
        slMotorPWM /= 1000; // Because the constants are long, not floats
//        slMotorPWM /= 100;
        
        
        
        if ( slMotorPWM <= 0 ) {
            slMotorPWM = 0;
        }
        else if ( slMotorPWM > 100 ) {
            slMotorPWM = 100;
        }
        else {
            // between 0 and 100, ok
        }
        
        Nop();
        // MAIN MOVEMENT - DEPENDS ON ASMode¡
        if ( ucMode == ucASMode ) {
            if ( ucASMode == ASMode ) {
                //ETCMove(slMotorPWM, ASMode); // TODO UNCOMMENT AND MAKE IT WORK
                GPIO_PWM2_Control(slMotorPWM, 600);
            }
            else if ( ucASMode == ManualMode ) {
                //ETCMove(slMotorPWM, ASMode); // TODO UNCOMMENT AND MAKE IT WORK
//                ETCMove(slMotorPWM, ManualMode);
                GPIO_PWM2_Control(slMotorPWM, 600);
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
    GPIO_PWM2_Control(50, uiAPPS1); // Play as frequency the value of the accelerator pedal.
//    GPIO_PWM2_Control(50, uiAPPS2); // Play as frequency the value of the accelerator pedal.
    
//    GPIO_PWM2_Control(50, ui_tps1_mv); // Play as frequency the value of the Electronic Throttle Motor.
//    GPIO_PWM2_Control(50, ui_tps2_mv); // Play as frequency the value of the Electronic Throttle Motor.
}