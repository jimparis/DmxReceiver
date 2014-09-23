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

#include "mk20dx128.h"
#include "DmxReceiver.h"

#include "Arduino.h"

#ifndef UART_C3_FEIE
#define UART_C3_FEIE    (uint8_t)0x02   // Framing Error Interrupt Enable
#endif

DmxReceiverClass::DmxReceiverClass() :
    Uart(),
    m_dmxBuffer1({0}),
    m_dmxBuffer2({0}),
    m_activeBuffer(m_dmxBuffer1),
    m_inactiveBuffer(m_dmxBuffer2),
    m_dmxBufferIndex(0),
    m_frameCount(0),
    m_newFrame(false)
{ }

void DmxReceiverClass::begin(void)
{
        // UART Initialization
        Uart.begin(250000);

        // Fire UART0 receive interrupt immediately after each byte received
        UART0_RWFIFO = 1;

        // Set error IRQ priority lower than that of the status IRQ,
        // so that the status IRQ receives any leftover bytes before
        // we detect and trigger a new frame.
        NVIC_SET_PRIORITY(IRQ_UART0_ERROR,
                          NVIC_GET_PRIORITY(IRQ_UART0_STATUS) + 1);

        // Enable UART0 interrupt on frame error and enable IRQ
        UART0_C3 |= UART_C3_FEIE;
        NVIC_ENABLE_IRQ(IRQ_UART0_ERROR);

        m_activeBuffer = m_dmxBuffer1;
        m_inactiveBuffer = m_dmxBuffer2;
}

void DmxReceiverClass::end(void)
{
        Uart.end();
        NVIC_DISABLE_IRQ(IRQ_UART0_ERROR);
}

void DmxReceiverClass::fill(uint8_t v)
{
        __disable_irq();
        m_dmxBuffer1[0] = 0;
        memset((void *)(m_dmxBuffer1 + 1), v, DMX_BUFFER_SIZE - 1);
        m_dmxBuffer2[0] = 0;
        memset((void *)(m_dmxBuffer2 + 1), v, DMX_BUFFER_SIZE - 1);
        __enable_irq();
}

void DmxReceiverClass::clear(void)
{
        fill(0);
}

unsigned int DmxReceiverClass::frameCount(void)
{
        return m_frameCount;
}

uint8_t DmxReceiverClass::getDimmer(uint16_t d)
{
        return m_inactiveBuffer[d];
}

volatile uint8_t* DmxReceiverClass::getBuffer()
{
        return m_inactiveBuffer;
}

int DmxReceiverClass::bufferService (void)
{
        __disable_irq(); //Prevents conflicts with the UART0 error ISR
        int available=Uart.available();
        int retval=available;
        while (available--)
        {
                m_activeBuffer[m_dmxBufferIndex]=Uart.read();
                if (m_dmxBufferIndex<(DMX_BUFFER_SIZE-1)) m_dmxBufferIndex++;
        }
        __enable_irq();
        return retval;
}

bool DmxReceiverClass::newFrame(void)
{
        if (m_newFrame)
        {
                m_newFrame=false;
                return true;
        }
        return false;
}

DmxReceiverClass DmxReceiver;

// UART0 will throw a frame error on the DMX break pulse.  That's our
// cue to switch buffers and reset the index to zero
void uart0_error_isr(void)
{
        // On break, uart0_status_isr() will probably have already
        // fired and read the data buffer, clearing the framing error.
        // If for some reason it hasn't, make sure we consume the 0x00
        // byte that was received.
        if (UART0_S1 & UART_S1_FE)
                (void) UART0_D;

        // Ensure we've processed all the data that may still be sitting
        // in software buffers.
        DmxReceiver.bufferService();

        // Update frame count and swap buffers
        DmxReceiver.m_frameCount++;
        DmxReceiver.m_dmxBufferIndex = 0;
        if (DmxReceiver.m_activeBuffer == DmxReceiver.m_dmxBuffer1)
        {
                DmxReceiver.m_activeBuffer = DmxReceiver.m_dmxBuffer2;
                DmxReceiver.m_inactiveBuffer = DmxReceiver.m_dmxBuffer1;
        }
        else
        {
                DmxReceiver.m_activeBuffer = DmxReceiver.m_dmxBuffer1;
                DmxReceiver.m_inactiveBuffer = DmxReceiver.m_dmxBuffer2;
        }
        DmxReceiver.m_newFrame = true;
}
