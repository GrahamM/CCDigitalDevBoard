Readme.md

Introduction
------------
This contains a basic 'framework' for building your own CurrentCost compatible
data device from a CurrentCost Digital Development Board. It does NOT transmit
electricity consumption (watts) values - you would need to modify the code for
that, as the packet structure is somewhat different.

In its unmodified form, the code will transmit a value of 1 if CH1 sensor pins
are shorted, and a value of 0 if they are not. After pairing, this information
will turn up in the XML output from the EnviR.

The function getValue() is where the impulse value is set, and this should be
the bit of code you want to modify. The impulse value is set as four bytes,
making up a 32bit counter. This is called approximately every 26 seconds.

This framework includes pairing code, which is initiated if you hold down the
red button for approx 5 seconds. The LED will flash gently on and off when
pairing is occurring. This will take approximately 12 seconds, so make sure you
have your EnviR ready for pairing before you push it!

Impulses and sensor type
------------------------
The code includes an impulses per unit value (mIpu), and a sensor type (mType).
Both of these turn up in the XML output, and the impulses per unit value can
only be set during pairing. The impulses per unit value sets how many impulses
make up a unit. The sensor type indicates if the device is electricity, gas or
water. While there is a oil indicator on the LCD, I've not been able to activate
it!

EEPROM use
----------
The code uses the first two memory locations in EEPROM to store the device
address.

EnviR Output
------------
The XML output from the EnviR should be something like:

<msg><src>CC128-v1.29</src><dsb>00759</dsb><time>19:09:09</time><tmpr>20.7</tmpr><sensor>8</sensor><id>12345</id><type>4</type><imp>0989912344</imp><ipu>0001</ipu></msg>

The value in the <imp> tag will be the decimal equivalent of whatever was
transmitted by this code.


Useful Notes
------------
Once the device is paired, do not change the sensor type - the EnviR will
ignore your sensor if you do. Additionally, if you want to change the sensor
type, you MUST change your address too. Trying to repair with a different sensor
type on the same address isn't acceptable to the EnviR.

If you need to update your code, you can avoid overwriting the EEPROM data, and
thus you won't need to pair your device again. If you overwrite the EEPROM data,
you'll need to pair your device, as the device address will be lost.

Useful debugging functions
--------------------------
There are some serial output functions, that output data at a speed of whatever
you've set as BAUD in user.h. These output to RA0, which is also ICSPDAT. If you
have a PICkit2 device, you can use the UART Tool (Tools -> UART Tool),as ICSPDAT
is used for serial input.

These functions are:
_Soft_USART_Write(uint8_t chr) - write a character to the serial port
_Soft_USART_Write_Hex(unsigned char data) - write a byte as ASCII hex
_Soft_USART_Writes(const char * s) - write a string to the serial port.