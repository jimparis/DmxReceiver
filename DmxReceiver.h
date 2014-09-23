/* DmxReceiver - DMX Receiver for Teensy 3
   Copyright (c) 2014 Jim Paris
   Copyright (c) 2014 Ward

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
*/

#ifndef DMXRECEIVER_H
#define DMXRECEIVER_H

#include "HardwareSerial.h"
#include <inttypes.h>

class DmxReceiverClass
{
public:
    DmxReceiverClass();
    
        /* Initialize UART0 for DMX */
	void begin(void);

        /* Deinitialize */
	void end(void);

        /* Clear all stored DMX values to zero */
    void clear(void);

        /* Fill DMX data with specified value */
    void fill(uint8_t v);

        /* Call this every 2ms or faster */
	int bufferService(void);

        /* Return the value at DMX address d */
	uint8_t getDimmer(uint16_t d);
	
	volatile uint8_t* getBuffer();

        /* Return the number of frame that have been received */
	unsigned int frameCount(void);

        /* Returns true if a new frame has been received since the
           last call to this function. */
	bool newFrame(void);

private:

    enum {
        DMX_BUFFER_SIZE = 513
    };

    HardwareSerial Uart;
    volatile uint8_t m_dmxBuffer1[DMX_BUFFER_SIZE];
    volatile uint8_t m_dmxBuffer2[DMX_BUFFER_SIZE];
    volatile uint8_t *m_activeBuffer;
    volatile uint8_t *m_inactiveBuffer;
    volatile uint16_t m_dmxBufferIndex;
    volatile unsigned int m_frameCount;
    volatile bool m_newFrame;
    
    friend void uart0_error_isr(void);
};

extern DmxReceiverClass DmxReceiver;

#endif
