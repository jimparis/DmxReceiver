#ifndef DMXRECEIVER_H
#define DMXRECEIVER_H

#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif
void dmx_begin(void);
void dmx_end(void);
void dmx_clear(void);
int dmx_bufferService(void);
uint8_t dmx_getDimmer(uint16_t);
unsigned int dmx_frameCount(void);
bool dmx_newFrame(void);
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
class DmxReceiver
{
public:
	void begin(void) { dmx_begin(); }
	void end(void) { dmx_end(); }
        void clear(void) { dmx_clear(); }
	int bufferService(void) { return dmx_bufferService(); }
	uint8_t getDimmer(uint16_t d) { return dmx_getDimmer(d); }
	unsigned int frameCount(void) { return dmx_frameCount(); }
	bool newFrame(void) { return dmx_newFrame(); }
};
#endif

#endif
