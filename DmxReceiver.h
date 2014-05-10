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
