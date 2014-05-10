#ifndef DMXRECEIVER_H
#define DMXRECEIVER_H

#include <inttypes.h>

class DmxReceiver
{
public:
	static void begin(void);
	static void end(void);
        static void clear(void);
	static int bufferService(void);
	static uint8_t getDimmer(uint16_t d);
	static unsigned int frameCount(void);
	static bool newFrame(void);
};

#endif
