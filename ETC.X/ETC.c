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
unsigned int ucAPPS1Perc;
unsigned int ucAPPS2Perc; 
unsigned int ucAPPS; 
unsigned int uiTPS1;
unsigned int uiTPS2; 
signed long ulTPS1calc;
signed long ulTPS2calc; 
unsigned int ucTPS1Perc;
unsigned int ucTPS2Perc; 
unsigned int ucTPS; 
unsigned char ucTPS_STATE; 
unsigned char ucTPS1_STATE; 
unsigned char ucTPS2_STATE; 
unsigned char ucTPS_Volts_STATE; 
unsigned int uiETCDuty;
unsigned char ucETB_STATE; 
unsigned char ucETCBeatSupervisor = FALSE; 
unsigned char ucETCFlagSupervisor = FALSE; 
unsigned char ucAPPSManual;
unsigned char ucETCTimerRuleTPS = FALSE;
unsigned char ucETCTimerRuleAPPS = FALSE;
unsigned char ucCount100msTPSError = 0;
unsigned char ucCount100msAPPSError = 0;
unsigned char ucETCRuleSupervisor = TRUE;
unsigned char ucETCTargetTPSDiff = 0;
unsigned char ucCount500msTPSDiff = 0;
unsigned char ucETCMotorNotClose = TRUE;
unsigned char ucETCResolveNotCloseError = FALSE;
unsigned char ucCount500msResolveNotCloseError = 0;

//Declaración de rangos que mas tarde se deberian autocalibrar aqui
unsigned int uiTPS1TableIn[]   = {1147,1167,1169,1202,1376,1349,1385,1572,1729,1780,1900,1951,2033,2145,2251,2318,2402,2493,2774,2778,2962};
unsigned char ucTPS1TableOut[] = {   0,   5,  10,  15,  20,  25,  30,  35,  40,  45,  50,  55,  60,  65,  70,  75,  80,  85,  90,  95, 100};
//unsigned char ucTPS2TableIn[]  = {999,1060,1070,1178,1238,1291,1329,1393,1485,1546,1604,1708,1761,1960,2215,2273,2365,2490,2594,2603,2618};
unsigned int uiTPS2TableIn[]   = {2618,2603,2594,2490,2365,2273,2215,1960,1761,1708,1604,1546,1485,1393,1329,1291,1238,1178,1070,1060,999};
unsigned char ucTPS2TableOut[] = {   0,   5,  10,  15,  20,  25,  30,  35,  40,  45,  50,  55,  60,  65,  70,  75,  80,  85,  90,  95, 100};
//unsigned char ucTPS2TableOut[] = { 100, 95, 90,85,80,75,70,65,60,55,50,45,40,35,30,25,20,15,10,5,0};

unsigned int ucAPPSTargetPruebas; 

//FUNCIONES
void ETCInit(void) 
{
    SDC_SetHigh(); //Habilitamos SDC
    OUTM_SetHigh();
    ETCCalibrate();
    PIDController_Init(&pid);
}
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
void ETCRulesSensorsSupervision(void) //ejecutar a 20Hz min o en cada porcentaje calculado 
{
    if ( ActiveRules == 1 )
    {
        //Supervision TPS
        if (ucTPS1Perc>ucTPS2Perc+TPSRulesPercent)
        {
            ucETCTimerRuleTPS = FALSE; 
        }
        else if (ucTPS2Perc>ucTPS1Perc+TPSRulesPercent)
        {
            ucETCTimerRuleTPS = FALSE; 
        }
        else 
        {
            ucETCTimerRuleTPS = TRUE; 
            ucCount100msTPSError = 0;
        }

        //Supervision APPS
        if (ucAPPS1Perc>ucAPPS2Perc+APPSRulesPercent)
        {
            ucETCTimerRuleAPPS = FALSE; 
        }
        else if (ucAPPS2Perc>ucAPPS1Perc+APPSRulesPercent)
        {
            ucETCTimerRuleAPPS = FALSE; 
        }
        else 
        {
            ucETCTimerRuleAPPS = TRUE; 
            ucCount100msAPPSError = 0;
        }
    }
}

void ETC100msSupervisor (void)
{
    if ( ActiveRules == 1 )
    {
        if ( ucETCTimerRuleTPS == FALSE )
        {
            if ( ucCount100msTPSError < 255 )
            {
                ucCount100msTPSError++;
            }
        }
        if ( ucETCTimerRuleAPPS == FALSE )
        {
            if ( ucCount100msTPSError < 255 )
            {
                ucCount100msAPPSError++;
            }
        }
        if ( ucCount100msTPSError >= 2 )
        {
            ucTPS_STATE |= TPS1_TPS2_DIFF;
            ucETCRuleSupervisor = FALSE;
        }
        if ( ucCount100msAPPSError >= 2 )
        {
            ucTPS_STATE |= TPS1_TPS2_DIFF;
            ucETCRuleSupervisor = FALSE;
        }
    }
}

void ETCRulesMotorSupervisor (unsigned char ucTPStarget, unsigned char ucTPSactual)
{
    CANWriteMessage(0x500, DataLength_6, ucTPStarget, ucTPSactual, ucTPS, ucAPPS, ucTPS_STATE, 0, 0, 0);    //Falta meter los APPS target
    if ( ActiveRules == 1 )
    {
        if (ucTPStarget>ucTPSactual+TPSRulesPercent)
        {
            ucETCTargetTPSDiff = FALSE; 
        }
        else if (ucTPSactual>ucTPStarget+TPSRulesPercent)
        {
            ucETCTargetTPSDiff = FALSE; 
        }
        else
        {
            ucETCTargetTPSDiff = TRUE;
            ucCount500msTPSDiff = 0;
            if (( ucTPSactual <= 5 ) && ( (ucTPS_STATE & ETC_NotClose_ERROR) == ETC_NotClose_ERROR ))//consideramos ralentí y error ETC not close guardado
            {
                ucETCResolveNotCloseError = TRUE;
            }
            else 
            {
                ucETCResolveNotCloseError = FALSE;
                ucCount500msResolveNotCloseError = 0;
            }
        }
    }
}
void ETC500msSupervisor (void)
{
    if ( ActiveRules == 1 )
    {
        if ( ucETCTargetTPSDiff == FALSE )
        {
            if ( ucCount500msTPSDiff < 255 )
            {
                ucCount500msTPSDiff++;
            }
        }
        if (ucCount500msTPSDiff == 2)
        {
            ucTPS_STATE |= TPS_Target_DIFF;
            ucETCRuleSupervisor = FALSE;
        }
        else if (ucCount500msTPSDiff >= 3) //mas de 1s con TPS mal
        {
            ucTPS_STATE |= ETC_NotClose_ERROR;
            ucETCMotorNotClose = FALSE;
            //Cortar SDC cuando este conectado a relé
            SDC_SetLow();
        }
        if ( ucETCResolveNotCloseError == TRUE )
        {
            if ( ucCount500msResolveNotCloseError < 255 )
            {
                ucCount500msResolveNotCloseError++;
            }
            if ( ucCount500msResolveNotCloseError >= 3 )//mas de 1s con TPS ookey tras estado not close
            {
                ucETCMotorNotClose = TRUE;
                ucETCResolveNotCloseError = FALSE;
                ucTPS_STATE &= QUITAR_ERROR_ETCNotClose;
            }
        }
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
    __delay_ms(200);
    uiTPS1min = uiTPS1 - TPSMARGEN; // Smallest value
    uiTPS2min = uiTPS2 + TPSMARGEN; // Biggest value
    uiTPS1TableIn[0]= uiTPS1 - TPSMARGEN;
    uiTPS2TableIn[0]= uiTPS2 + TPSMARGEN;
    Nop();
    
    // Intake motor 100%
    GPIO_PWM2_Control(100, 600); // 100% PWM at 600Hz, Motor to max power.
    // Let it move
    __delay_ms(700);
    // Read TPS sensors at max opened position.
    ANALOGRead(); // Calls ANALOG_GetVoltage(), assigns values to uiAPPS, uiTPS
    // Let it read
    __delay_ms(200);
    uiTPS1max = uiTPS1 - TPSMARGEN; // Biggest value
    uiTPS2max = uiTPS2 + TPSMARGEN; // Smallest  value
    uiTPS1TableIn[TPS_ADJ_TAB_SIZE-1]= uiTPS1 - TPSMARGEN;
    uiTPS2TableIn[TPS_ADJ_TAB_SIZE-1]= uiTPS2 + TPSMARGEN;
    Nop();
    
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
    
    
    /*ucTPS1Perc = ETCPercentCalc (uiTPS1, uiTPS1min, uiTPS1max);
    ucTPS2Perc = ETCPercentCalc (uiTPS2, uiTPS2min, uiTPS2max);*/
    ucTPS1Perc = ETCPercentMultiCalcTPS1 (uiTPS1, uiTPS1TableIn, ucTPS1TableOut, TPS_ADJ_TAB_SIZE);
    ucTPS2Perc = ETCPercentMultiCalcTPS2 (uiTPS2, uiTPS2TableIn, ucTPS2TableOut, TPS_ADJ_TAB_SIZE);
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
        ucTPS_STATE &= QUITAR_ERROR_TPS_VOLTS;
    }
    else if ( ucTPS_Volts_STATE == TPS1_INVERTED_TPS2_NO_INVERTED )
    {
        //todo OK, eliminar erro de estado
        ucTPS_STATE &= QUITAR_ERROR_TPS_VOLTS;
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
    
    ucAPPS1Perc = ETCPercentCalc(uiAPPS1, uiAPPS1min, uiAPPS1max);
    ucAPPS2Perc = ETCPercentCalc(uiAPPS2, uiAPPS2min, uiAPPS2max);
    ucAPPS = ( ( ucAPPS1Perc + ucAPPS2Perc ) / 2 );
    Nop();
}

//Funcion para mover directamente el servo con PWM
void ETCMove(unsigned char ucTargetMove, unsigned char ucMode)
{
    ETCRulesMotorSupervisor (ucTargetMove, ucTPS);
    //Depender de beat constante en CAN
    if ( ( ucETCFlagSupervisor == TRUE ) && ( ucETCRuleSupervisor == TRUE ) )
    {
        //HACER CONVERSION DE 0-100% A 2-12 DUTY
        uiETCDuty = ucTargetMove;
        //nos tenemos que asegurar antes de mover que aceptamos ordenes de manual o autonomo
        if ( ucMode == ucASMode )
        {
            if ( ucASMode == ASMode ) 
            {
                //aumentar un 10% para asegurar un ralenti siempre, quiza ajustarlo con rpm
                //GPIO_PWM2_Control(uiETCDuty + 34, 600); //lo muevo sin comprobar nada
                GPIO_PWM2_Control(PIDController_Update(&pid, (float)(ucTargetMove), (float)(ucTPS)), 600);
            }
            else if ( ucASMode == ManualMode )
            {
                //hay que ver como meter aqui la conexion con TPS y APPS
                //GPIO_PWM2_Control(uiETCDuty, 600); //lo muevo sin comprobar nada
                GPIO_PWM2_Control(PIDController_Update(&pid, (float)(ucTargetMove), (float)(ucTPS)), 600);
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

void ETCXavierSupervisor (void)
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
unsigned int ETCPercentCalc(signed long val, signed long min, signed long max) 
{
    /*Returns the percentage of val between min and max. Result bounded between 0 and 100.
     * If val is out of the range [min,max], then it will be capped at 0% or 100%.
     * If min is greater than max, the closest val is to min, the closest the result will be to 0%, and viceversa.
     * If val exceeds the range going through min, it will be 0%. It if goes through max, it will be 100%, even if min > max.
     * A higher value will be 0% in this case.
     */
    val = (100*(val - min))/(max - min);
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

unsigned int ETCPercentMultiCalcTPS1(signed long value, unsigned int *uiTab_in, unsigned char *ucTab_out, unsigned char ucSize) 
{
    unsigned char ucPos = 1;  // _in[0] already tested
    signed long slResult;    
    unsigned int ucValCero= uiTab_in[0];
    unsigned int ucValMax= uiTab_in[ucSize-1];
    unsigned char ucValout=0;
    // take care the value is within range
    // value = constrain(value, _in[0], _in[size-1]);
    if (value <= ucValCero) return ucTab_out[0];
    if (value >= ucValMax) return ucTab_out[ucSize-1];

    // search right interval

    while(value > uiTab_in[ucPos]) ucPos++;
    ucValout = ucTab_out[ucPos];
    // this will handle all exact "points" in the _in array
    if (value == uiTab_in[ucPos]) return ucTab_out[ucPos];

    // interpolate in the right segment for the rest
    slResult = ( (value - uiTab_in[ucPos-1]) * (ucTab_out[ucPos] - ucTab_out[ucPos-1]) / (uiTab_in[ucPos] - uiTab_in[ucPos-1]) + ucTab_out[ucPos-1] );
    
    if ( slResult < 0 ) slResult = ucTab_out[0];
    if ( slResult > 100 ) slResult = ucTab_out[ucSize-1];
    
    return slResult ;
}

unsigned int ETCPercentMultiCalcTPS2(signed long value, unsigned int *uiTab_in, unsigned char *ucTab_out, unsigned char ucSize) 
{
    unsigned char ucPos = 1;  // _in[0] already tested
    signed long slResult;    
    unsigned int ucValCero= uiTab_in[0];
    unsigned int ucValMax= uiTab_in[ucSize-1];
    unsigned char ucValout=0;
    // take care the value is within range
    // value = constrain(value, _in[0], _in[size-1]);
    if (value >= ucValCero) return ucTab_out[0];
    if (value <= ucValMax) return ucTab_out[ucSize-1];

    // search right interval

    while(value < uiTab_in[ucPos]) ucPos++;
    ucValout = ucTab_out[ucPos];
    // this will handle all exact "points" in the _in array
    if (value == uiTab_in[ucPos]) return ucTab_out[ucPos];

    // interpolate in the right segment for the rest
    slResult = ( ( (value - uiTab_in[ucPos-1]) * (ucTab_out[ucPos] - ucTab_out[ucPos-1]) / (uiTab_in[ucPos] - uiTab_in[ucPos-1]) ) + ucTab_out[ucPos-1] );
    
    if ( slResult < 0 ) slResult = ucTab_out[0];
    if ( slResult > 100 ) slResult = ucTab_out[ucSize-1];
    
    return slResult ;
}


//-----
void PIDController_Init(PIDController *pid) {

	/* Clear controller variables */
	pid->integrator = 0.0f;
	pid->prevError  = 0.0f;

	pid->differentiator  = 0.0f;
	pid->prevMeasurement = 0.0f;

	pid->out = 0.0f;

}

float PIDController_Update(PIDController *pid, float setpoint, float measurement) {

	/*
	* Error signal
	*/
    float error = setpoint - measurement;


	/*
	* Proportional
	*/
    float proportional = pid->Kp * error;


	/*
	* Integral
	*/
    pid->integrator = pid->integrator + 0.5f * pid->Ki * pid->T * (error + pid->prevError);

    
	/* Anti-wind-up via integrator clamping */
    /*
    if (pid->integrator > pid->limMaxInt) 
    {
        pid->integrator = pid->limMaxInt;
    } 
    else if (pid->integrator < pid->limMinInt) 
    {
        pid->integrator = pid->limMinInt;
    }
    */


	/*
	* Derivative (band-limited differentiator)
	*/
		
    pid->differentiator = -(2.0f * pid->Kd * (measurement - pid->prevMeasurement)	/* Note: derivative on measurement, therefore minus sign in front of equation! */
                        + (2.0f * pid->tau - pid->T) * pid->differentiator)
                        / (2.0f * pid->tau + pid->T);


    // Reset integral if big input derivative
     if ((pid->differentiator > 2.0f) || (pid->differentiator < -2.0f)){
         pid->integrator = 0;
    }
	/*
	* Compute output and apply limits
	*/
    pid->out = proportional + pid->integrator + pid->differentiator;

    if (pid->out > pid->limMax) {

        pid->out = pid->limMax;

    } else if (pid->out < pid->limMin) {

        pid->out = pid->limMin;

    }

	/* Store error and measurement for later use */
    pid->prevError       = error;
    pid->prevMeasurement = measurement;

	/* Return controller output */
    return pid->out;

}

