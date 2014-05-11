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

#include <inttypes.h>

class DmxReceiver
{
public:
        /* Initialize UART0 for DMX */
	static void begin(void);

        /* Deinitialize */
	static void end(void);

        /* Clear all stored DMX values to zero */
        static void clear(void);

        /* Fill DMX data with specified value */
        static void fill(uint8_t v);

        /* Call this every 2ms or faster */
	static int bufferService(void);

        /* Return the value at DMX address d */
	static uint8_t getDimmer(uint16_t d);

        /* Return the number of frame that have been received */
	static unsigned int frameCount(void);

        /* Returns true if a new frame has been received since the
           last call to this function. */
	static bool newFrame(void);
};

#endif
