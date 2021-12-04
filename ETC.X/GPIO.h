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

//FUNCIONES
void GPIO_PWM1_Control (unsigned int uiDutyCycle, unsigned int uiFreq);
void GPIO_INT2_desembragar (void);

#ifdef	__cplusplus
}
#endif

#endif	/* GPIO_H */

