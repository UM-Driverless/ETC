/* 
 * File:   ANALOG.h
 * Author: 93Urbano
 *
 * Created on December 24, 2021, 6:29 PM
 */

#include "ANALOG.h"
#include "ETC.h"
#include "mcc_generated_files/adc.h"


//**********ANALOG_GetVoltage***********//
//Funcion que devuelve el valor del voltaje calculado de las analogicas)
unsigned int ANALOG_GetVoltage (unsigned char ucEntradaAnalogica)
{
    uint16_t uiValorAnalog;
    uint16_t uiValorVoltage;
    unsigned char ucFlag;
    
    switch (ucEntradaAnalogica)
    {
        case ENT_TPS1:
            ADC_DisableChannelSequencer();    //Disable scanner
            ADC_SelectContext(CONTEXT_TPS1);
            uiValorAnalog = ADC_GetSingleConversion(TPS1);
            ucFlag = 1;
            break;
        case ENT_TPS2:
            ADC_DisableChannelSequencer();    //Disable scanner
            ADC_SelectContext(CONTEXT_TPS2);
            uiValorAnalog = ADC_GetSingleConversion(TPS2);
            ucFlag = 1;
            break;
        case ENT_APPS1:
            ADC_DisableChannelSequencer();    //Disable scanner
            ADC_SelectContext(CONTEXT_APPS1);
            uiValorAnalog = ADC_GetSingleConversion(APPS1);
            ucFlag = 1;
            break;
        case ENT_APPS2:
            ADC_DisableChannelSequencer();    //Disable scanner
            ADC_SelectContext(CONTEXT_APPS2);
            uiValorAnalog = ADC_GetSingleConversion(APPS2);
            ucFlag = 1;
            break;
        default:
            ucFlag = 2;
            break;
    }
    
    if ( ucFlag == 1 )
    {
        if ( uiValorAnalog <= 6 )
        {
            uiValorVoltage = 0;
        }
        else
        {
            //EXTRAPOLACION LINEAL DE LA GRAFICA
            //uiValorVoltage = ((1.1444*uiValorAnalog)-5.2658);
            uiValorVoltage = (1*uiValorAnalog);
            uiValorVoltage = uiValorVoltage - 5;
        }
        
        if ( uiValorVoltage > 5000 )
        {
            //fallo de sobrevoltaje
            //o variable da la vuelta
        }
        else
        {
            return (uiValorVoltage);
        }   
    }
    else if ( ucFlag == 2 )
    {
        //entrada analogica no valida
        //return (0xFFFF);
    }
    else
    {
        //fallo en ucFlag
    }

}

//EJECUTARLO CADA 10ms
void ANALOGRead (void)
{
    ucAPPS1 = ANALOG_GetVoltage (ENT_APPS1);
    ucAPPS2 = ANALOG_GetVoltage (ENT_APPS2);
    ucTPS1 = ANALOG_GetVoltage (ENT_TPS1);
    ucTPS2 = ANALOG_GetVoltage (ENT_TPS2);
}
