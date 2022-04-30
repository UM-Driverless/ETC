/* 
 * File:   GPIO.h
 * Author: 93Urbano
 *
 * Created on 4 de diciembre de 2021, 13:40
 */

#ifndef GPIO_H
#define	GPIO_H

#ifdef	__cplusplus
extern "C" {
#endif

#define GPIO_PWM2_CLOCK_FREQ 10000 // reference interrupt frequency to build the PWM. Interrupt with prescale from crystal, MCC.



//FUNCIONES
void GPIOInit(void);
void GPIO_PWM1_Control (unsigned int uiDutyCycle, unsigned int uiFreq);
void GPIO_PWM2_Control (unsigned int uiDutyCycle, unsigned int uiFreq);
void GPIO_INT2_desembragar(void);

#ifdef	__cplusplus
}
#endif

#endif	/* GPIO_H */

