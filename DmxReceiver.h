#ifndef DMXRECEIVER_H
#define DMXRECEIVER_H

#include <inttypes.h>

#define UART_C3_FEIE		(uint8_t)0x02			//Framing Error Interrupt Enable
#define UART_C5_RDMAS		(uint8_t)0x20			//Receiver Full DMA Select

#ifdef __cplusplus
extern "C" {
#endif
void dmx_begin(void);
void dmx_end(void);
int dmx_bufferService(void);
uint8_t dmx_getDimmer(uint16_t);
unsigned int dmx_frameCount(void);
bool dmx_newFrame(void);
#ifdef __cplusplus
}
#endif


class DmxReceiver
{
public:
	void begin(void) {dmx_begin();}
	void end(void) {dmx_end();}
	int bufferService(void) {return dmx_bufferService();}
	uint8_t getDimmer(uint16_t d) {return dmx_getDimmer(d);}
	unsigned int frameCount(void) {return dmx_frameCount();}
	bool newFrame(void) {return dmx_newFrame();}
};

#endif
