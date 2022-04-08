/*
    (c) 2016 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

#include "DAC3.h"
#include "drivers/i2c_simple_master.h"

void DAC3_Set(uint16_t dacValue)
{
    i2c_write1ByteRegister(0x60,(dacValue>>8),dacValue);
}

void DAC3_SetNonvolatile(uint16_t dacValue)
{
    i2c_write2ByteRegister(0x60,0x60,dacValue);
}

uint16_t DAC3_Read(uint16_t *dacNonvolatile)
{   
    uint16_t dacVolatile;
    struct {uint8_t volStatus, volatileVoltByte1, volatileVoltByte2, nvStatus, nonvolatileVoltByte1, nonvolatileVoltByte2; } data;
    
    i2c_readNBytes(0x60, &data, sizeof(data));

    //First byte holds D11:D04
    //Second byte holds D3:D0 padded with 4 0s on the end
    dacVolatile =  (data.volatileVoltByte1 << 4) + (data.volatileVoltByte2 >> 4);
    
    if (dacNonvolatile)
    {
        *dacNonvolatile = (data.nonvolatileVoltByte1 << 4) + (data.nonvolatileVoltByte2 >> 4);
    }
    return dacVolatile;
}