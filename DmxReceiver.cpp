#include "DmxReceiver.h"
#include "HardwareSerial.h"
#define DMX_BUFFER_SIZE 513

static volatile uint8_t dmxBuffer1[DMX_BUFFER_SIZE];
static volatile uint8_t dmxBuffer2[DMX_BUFFER_SIZE];
static volatile uint8_t *activeBuffer;
static volatile uint8_t *inactiveBuffer;
static volatile uint16_t dmxBufferIndex;
static volatile unsigned int frameCount=0;
static volatile bool newFrame=false;
HardwareSerial Uart = HardwareSerial();

void dmx_begin(void)
{
	//UART Initialization
	Uart.begin(250000);
	UART0_C3 = UART_C3_FEIE;  //Enable UART0 interrupt on frame error
	NVIC_ENABLE_IRQ(IRQ_UART0_ERROR);  //Enable the frame error IRQ
	activeBuffer=dmxBuffer1;
	inactiveBuffer=dmxBuffer2;
}

void dmx_end(void)
{
	Uart.end();
	NVIC_DISABLE_IRQ(IRQ_UART0_ERROR);  //Enable the frame error IRQ
}

void dmx_clear(void)
{
        __disable_irq();
        memset((void *)dmxBuffer1, 0, DMX_BUFFER_SIZE);
        memset((void *)dmxBuffer2, 0, DMX_BUFFER_SIZE);
        __enable_irq();
}

unsigned int dmx_frameCount(void)
{
	return frameCount;
}

uint8_t dmx_getDimmer(uint16_t d)
{
	return inactiveBuffer[d];
}

int dmx_bufferService (void)
{
	__disable_irq(); //Prevents conflicts with the UART0 error ISR
	int available=Uart.available();
	int retval=available;
	while (available--)
	{
		activeBuffer[dmxBufferIndex]=Uart.read();
		if (dmxBufferIndex<(DMX_BUFFER_SIZE-1)) dmxBufferIndex++;
	}
	__enable_irq();
	return retval;
}

bool dmx_newFrame(void)
{
	if (newFrame)
	{
		newFrame=false;
		return true;
	}
	return false;
}

//UART0 will throw a frame error on the DMX break pulse.  That's our cue to switch buffers
//and reset the index to zero
void uart0_error_isr(void)
{
		if (UART0_S1 & UART_S1_FE)
		{
			(void) UART0_D;
			frameCount++;
			dmx_bufferService();
			dmxBufferIndex=0;
			if (activeBuffer==dmxBuffer1)
			{
				activeBuffer=dmxBuffer2;
				inactiveBuffer=dmxBuffer1;
			}
			else
			{
				activeBuffer=dmxBuffer1;
				inactiveBuffer=dmxBuffer2;
			}
			newFrame=true;
		}
}
