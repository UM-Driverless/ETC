/* 
 * File:   ETC.h
 * Author: 93Urbano
 *
 * Created on December 24, 2021, 6:15 PM
 */

#ifndef ETC_H
#define	ETC_H

#ifdef	__cplusplus
extern "C" {
#endif
    
    
//CONSTANTES
#define TPS_OK              0
//TPS STATES
#define TPS1_ERROR          0x01
#define TPS2_ERROR          0x02
#define TPS_Volt_ERROR      0x04
#define TPS1_TPS2_DIFF      0x08
#define TPS_Target_DIFF     0x10
#define ETC_NotClose_ERROR  0x20
#define QUITAR_ERROR_TPS1  0xFE    
#define QUITAR_ERROR_TPS2  0xFD   
#define QUITAR_ERROR_TPS_VOLTS 0xFB    
#define QUITAR_ERROR_ETCNotClose    0xDF
//inversion de voltaje
#define TPS1_NO_INVERTED    1  
#define TPS1_INVERTED       2
#define TPS2_NO_INVERTED    4  
#define TPS2_INVERTED       8
#define TPS1_NO_INVERTED_TPS2_NO_INVERTED       5
#define TPS1_NO_INVERTED_TPS2_INVERTED          9  
#define TPS1_INVERTED_TPS2_NO_INVERTED          6  
#define TPS1_INVERTED_TPS2_INVERTED             10  
    
//APPS STATES
#define APPS1_ERROR          1
#define APPS2_ERROR          2
#define APPS1_APPS2_DIFF     3
#define APPS_Volt_ERROR      4
#define QUITAR_ERROR_APPS1  0xFE    
#define QUITAR_ERROR_APPS2  0xFD   
#define QUITAR_ERROR_APPS_VOLTS 0xFB       
    
//PID
#define PID_KP  2.8f //2.8f
#define PID_KI  1.4f //1.4f
#define PID_KD  0.16f //0.16f

#define PID_TAU 0.02f

#define PID_LIM_MIN 0.0f //0%
#define PID_LIM_MAX  100.0f //100%

#define PID_LIM_MIN_INT -5.0f
#define PID_LIM_MAX_INT  5.0f

#define SAMPLE_TIME_S 0.01f
    
#define TPS_ADJ_TAB_SIZE 20
    
unsigned char ucTPS1TableIn [TPS_ADJ_TAB_SIZE];
unsigned char ucTPS1TableOut [TPS_ADJ_TAB_SIZE];
unsigned char ucTPS2TableIn [TPS_ADJ_TAB_SIZE];
unsigned char ucTPS2TableOut [TPS_ADJ_TAB_SIZE];
    
typedef struct {

	/* Controller gains */
	float Kp;
	float Ki;
	float Kd;

	/* Derivative low-pass filter time constant */
	float tau;

	/* Output limits */
	float limMin;
	float limMax;
	
	/* Integrator limits */
	float limMinInt;
	float limMaxInt;

	/* Sample time (in seconds) */
	float T;

	/* Controller "memory" */
	float integrator;
	float prevError;			/* Required for integrator */
	float differentiator;
	float prevMeasurement;		/* Required for differentiator */

	/* Controller output */
	float out;

} PIDController;

PIDController pid = { PID_KP, PID_KI, PID_KD,
                      PID_TAU,
                      PID_LIM_MIN, PID_LIM_MAX,
                      PID_LIM_MIN_INT, PID_LIM_MAX_INT,
                      SAMPLE_TIME_S };


//VARIABLES    
extern unsigned int uiAPPS1min;
extern unsigned int uiAPPS1max;    
extern unsigned int uiAPPS2min;
extern unsigned int uiAPPS2max;  
extern unsigned int uiTPS1min;
extern unsigned int uiTPS1max;    
extern unsigned int uiTPS2min;
extern unsigned int uiTPS2max; 
extern unsigned int uiAPPS1;
extern unsigned int uiAPPS2;   
extern unsigned char ucAPPS_STATE; 
extern unsigned long ulAPPS1calc;
extern unsigned long ulAPPS2calc; 
extern unsigned int ucAPPS1Perc;
extern unsigned int ucAPPS2Perc; 
extern unsigned int ucAPPS; 
extern unsigned int uiTPS1;
extern unsigned int uiTPS2; 
extern signed long ulTPS1calc;
extern signed long ulTPS2calc; 
extern unsigned int ucTPS1Perc;
extern unsigned int ucTPS2Perc; 
extern unsigned int ucTPS; 
extern unsigned char ucTPS_STATE; 
extern unsigned char ucTPS1_STATE; 
extern unsigned char ucTPS2_STATE; 
extern unsigned char ucTPS_Volts_STATE; 
extern unsigned int uiETCDuty;
extern unsigned char ucETB_STATE; 
extern unsigned char ucETCBeatSupervisor; 
extern unsigned char ucETCFlagSupervisor; 
extern unsigned char ucAPPSManual;
extern unsigned char ucETCTimerRuleTPS;
extern unsigned char ucETCTimerRuleAPPS;
extern unsigned char ucCount100msTPSError;
extern unsigned char ucCount100msAPPSError;
extern unsigned char ucETCRuleSupervisor;
extern unsigned char ucETCTargetTPSDiff;
extern unsigned char ucCount500msTPSDiff;
extern unsigned char ucETCMotorNotClose;
extern unsigned char ucETCResolveNotCloseError;
extern unsigned char ucCount500msResolveNotCloseError;

//FUNCIONES
void ETCInit(void);
void APPSSend (unsigned char ucPercent);
void APPSReadmin (void);
void APPSReadmax (void);
void ETCModeSelect (unsigned char ucModeSelect);
void ETCRulesSupervision(void);
void ETCMove(unsigned char ucTargetMove, unsigned char ucMode);
void ETC_PID(signed long slTargetMove, unsigned char ucMode);
void ETCCalibrate(void);
void TPSAnalysis (void);
void APPSAnalysis (void);
void ETCXavierSupervisor (void);
void ETCManual (unsigned char ucTargetManual);
unsigned int ETCPercentCalc(signed long val, signed long min, signed long max);
void ETCRulesSensorsSupervision(void);
void ETC100msSupervisor (void);
void ETCRulesMotorSupervisor (unsigned char ucTPStarget, unsigned char ucTPSactual);
void ETC500msSupervisor (void);
void PIDController_Init(PIDController *pid);
float PIDController_Update(PIDController *pid, float setpoint, float measurement);
unsigned int ETCPercentMultiCalc(signed long value, unsigned char *ucTab_in, unsigned char *ucTab_out, unsigned char ucSize);

#ifdef	__cplusplus
}
#endif

#endif	/* ETC_H */