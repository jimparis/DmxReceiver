/* DmxReceiver DmxTest.ino - DMX Test
   Copyright (c) 2014 Jim Paris

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.

   Required Connections
   --------------------
   pin 0: DMX in (from RS485 transceiver)

   This test prints out DMX data to USB serial every 1 second.
   The on-board LED is toggled every time a DMX frame is received.
*/

#include <DmxReceiver.h>

IntervalTimer dmxTimer;

void dmxTimerISR(void)
{
        DmxReceiver.bufferService();
}

void setup() {
        /* USB serial */
        Serial.begin(115200);

        /* DMX */
        DmxReceiver.begin();

        /* Use a timer to service DMX buffers every 1ms */
        dmxTimer.begin(dmxTimerISR, 1000);

        pinMode(LED_BUILTIN, OUTPUT);
}

int led = 0;
elapsedMillis elapsed;
void loop()
{
        /* Toggle LED on every new frame */
        if (DmxReceiver.newFrame())
        {
                led = !led;
                digitalWrite(LED_BUILTIN, led);
        }

        /* Dump DMX data every second */
        if (elapsed > 1000) {
                elapsed -= 1000;
                Serial.printf("DMX frameCount=%d", DmxReceiver.frameCount());

                /* Display all nonzero DMX values */
                for (int i = 0; i < 512; i++) {
                        uint8_t v = DmxReceiver.getDimmer(i);
                        if (v)
                                Serial.printf(" %d:%d", i, v);
                }
                Serial.printf("\n");
        }
}
