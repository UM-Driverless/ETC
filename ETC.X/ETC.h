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

// PID controller struct with constants.
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
    
    
//CONSTANTES
#define TPS_OK              0
//errores por salidad de margenes
#define TPS1_ERROR          1
#define TPS2_ERROR          2
#define TPS1_TPS2_ERROR     3
#define TPS_Volt_ERROR      4
#define QUITAR_ERROR_TPS1  0xFE    
#define QUITAR_ERROR_TPS2  0xFD   
//inversion de voltaje
#define TPS1_NO_INVERTED    1  
#define TPS1_INVERTED       2
#define TPS2_NO_INVERTED    4  
#define TPS2_INVERTED       8
#define TPS1_NO_INVERTED_TPS2_NO_INVERTED       5
#define TPS1_NO_INVERTED_TPS2_INVERTED          9  
#define TPS1_INVERTED_TPS2_NO_INVERTED          6  
#define TPS1_INVERTED_TPS2_INVERTED             10  
#define QUITAR_ERROR_VOLTS                      0xFB       

extern PIDController pid;

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

// PID CONSTANTS
#define PID_KP  2.8f //2.8f
#define PID_KI  1.4f //1.4f
#define PID_KD  0.16f //0.16f

#define PID_TAU 0.02f

#define PID_LIM_MIN 0.0f //0%
#define PID_LIM_MAX  100.0f //100%

#define PID_LIM_MIN_INT -5.0f
#define PID_LIM_MAX_INT  5.0f

#define SAMPLE_TIME_S 0.01f

//FUNCIONES
void ETCMove(unsigned char ucTargetMove, unsigned char ucMode);

void  PIDController_Init(PIDController *pid);
float PIDController_Update(PIDController *pid, float setpoint, float measurement);

void APPSSend (unsigned char ucPercent);
void APPSReadmin(void);
void APPSReadmax(void);
void ETCRulesSupervision(void);
void ETC_PID(signed long slTargetMove, unsigned char ucMode);
void ETCCalibrate(void);
void TPSAnalysis(void);
void APPSAnalysis(void);
void ETCSupervisor(void);
void ETCManual(unsigned char ucTargetManual);
unsigned int ETCPercentCalc(signed long val, signed long min, signed long max);


#ifdef	__cplusplus
}
#endif

#endif	/* ETC_H */