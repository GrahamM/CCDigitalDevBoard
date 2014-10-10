/* Copyright (c) 2014 Graham Murphy <graham@c0redump.net>
*
* Permission to use, copy, modify, and distribute this software for any
* purpose with or without fee is hereby granted, provided that the above
* copyright notice and this permission notice appear in all copies.
*
* THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
* WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
* ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
* WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
* ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
* OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/
/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#include <htc.h>           /* Global Header File */
#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */


#include "user.h"          /* User funct/params, such as InitApp */
 

/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/
DataPacket_t datapacket; // Our data packet to work with
const uint8_t mType=4;   // Meter type (water)
const uint32_t mIpu=1;   // Number of impulses per unit


/******************************************************************************/
/* Feel free to change getValue to suit your purposes.                        */
/* This example just sends a 1 if the pisn are shorted, and a 0 if they're not*/
/******************************************************************************/
void getValue()
{
    // Set these bytes to whatever you want. This is the value that'll
    // turn up in the IMP XML output from the EnviR.
    datapacket.value_high = 0x00;
    datapacket.value_midh = 0x00;
    datapacket.value_midl = 0x00;
    if (SENS) {                         // Read sensor input
        datapacket.value_low  = 0x01;   // Send a 1 if sensor is high (pins shorted)
    } else {
        datapacket.value_low  = 0x00;   // Send a 0 if sensor is low (pins open)
    }
}

/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/

void main(void)
{
    uint8_t cnt,debounce;
    
    
    ConfigureOscillator(1);         // high speed
    LED=0;
    InitApp();                      // Init I/O and peripherals
    InitRadio();                    // Setup radio

    for (cnt=0;cnt<8;cnt++) {       // init data packet
        datapacket.bytes[cnt]=0;
    }
    GetAddr(datapacket.bytes);      // Get our address from EEPROM
    datapacket.sensor_type=mType;   // Set sensor type

    // Delay two seconds to allow things to get started up
    for (cnt=0;cnt<20;cnt++) {
        __delay_ms(100);
    }

    // Main code loop
    while (1)
    {
        ConfigureOscillator(1);         // high speed

        LED=1;                          // LED on
        getValue();                     // Get the data value
        TXData(datapacket.bytes);       // Transmit the data
        LED=0;                          // LED off

        ConfigureOscillator(0);         // low speed
        
        for (cnt=0;cnt<20;cnt++)        // Idle loop - approx 26 seconds
        {
            __delay_ms(4);              // at low speed, it's a lot longer than
                                        // might think! (4 = ~1.25secs)
            // debounce switch
            if (!BTN) {debounce++; } else {debounce=0;}

            // pair if button held for ~5 seconds.
            if (debounce>4) {
                ConfigureOscillator(1); // high speed
                doPair(datapacket.bytes, mType,mIpu);// generate new address and pair
                debounce=0;
                ConfigureOscillator(0); // low speed
                }
            }
        }
}

