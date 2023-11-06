/**
  * TCA1 Generated Driver File
  *
  * @file tca1.c
  *
  * @ingroup tca1_normal
  *
  * @brief This file contains the API implementations for TCA1 module driver in Normal (16-bit) mode.
  *
  * @version TCA1 Driver Version 2.1.2
*/
/*
© [2023] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.? 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
*/


#include "../tca1.h"

const struct TMR_INTERFACE TCA1_Interface = {
    .Initialize = TCA1_Initialize,
    .Start = TCA1_Start,
    .Stop = TCA1_Stop,
    .PeriodCountSet = TCA1_Write,
    .TimeoutCallbackRegister = TCA1_OverflowCallbackRegister,
    .Tasks = NULL
};
void TCA1_DefaultCompare0CallbackRegister(void);
void (*TCA1_CMP0_isr_cb)(void) = &TCA1_DefaultCompare0CallbackRegister;

void TCA1_DefaultCompare1CallbackRegister(void);
void (*TCA1_CMP1_isr_cb)(void) = &TCA1_DefaultCompare1CallbackRegister;

void TCA1_DefaultCompare2CallbackRegister(void);
void (*TCA1_CMP2_isr_cb)(void) = &TCA1_DefaultCompare2CallbackRegister;

void TCA1_DefaultOverflowCallbackRegister(void);
void (*TCA1_OVF_isr_cb)(void) = &TCA1_DefaultOverflowCallbackRegister;

void TCA1_DefaultCompare0CallbackRegister(void)
{
    //Add your ISR code here
}

void TCA1_DefaultCompare1CallbackRegister(void)
{
    //Add your ISR code here
}

void TCA1_DefaultCompare2CallbackRegister(void)
{
    //Add your ISR code here
}

void TCA1_DefaultOverflowCallbackRegister(void)
{
    //Add your ISR code here
}

void TCA1_OverflowCallbackRegister(TCA1_cb_t cb)
{
    TCA1_OVF_isr_cb = cb;
}

void TCA1_Compare0CallbackRegister(TCA1_cb_t cb)
{
    TCA1_CMP0_isr_cb = cb;
}

void TCA1_Compare1CallbackRegister(TCA1_cb_t cb)
{
    TCA1_CMP1_isr_cb = cb;
}

void TCA1_Compare2CallbackRegister(TCA1_cb_t cb)
{
    TCA1_CMP2_isr_cb = cb;
}


ISR(TCA1_OVF_vect)
{
    if (TCA1_OVF_isr_cb != NULL)
        (*TCA1_OVF_isr_cb)();
    
    TCA1.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;
}

void TCA1_Initialize(void) {
    // Compare 0
    TCA1.SINGLE.CMP0 = 0x0;
        
    // Compare 1
    TCA1.SINGLE.CMP1 = 0x0;
    
    // Compare 2
    TCA1.SINGLE.CMP2 = 0x0;
        
    // Count
    TCA1.SINGLE.CNT = 0x0;
    
    // ALUPD disabled; CMP0EN disabled; CMP1EN disabled; CMP2EN disabled; WGMODE NORMAL; 
    TCA1.SINGLE.CTRLB = 0x0;
    
    // CMP0OV disabled; CMP1OV disabled; CMP2OV disabled; 
    TCA1.SINGLE.CTRLC = 0x0;
    
    // SPLITM disabled; 
    TCA1.SINGLE.CTRLD = 0x0;
    
    // CMD NONE; DIR disabled; LUPD disabled; 
    TCA1.SINGLE.CTRLECLR = 0x0;
    
    // CMD NONE; DIR UP; LUPD disabled; 
    TCA1.SINGLE.CTRLESET = 0x0;
    
    // CMP0BV disabled; CMP1BV disabled; CMP2BV disabled; PERBV disabled; 
    TCA1.SINGLE.CTRLFCLR = 0x0;
    
    // CMP0BV disabled; CMP1BV disabled; CMP2BV disabled; PERBV disabled; 
    TCA1.SINGLE.CTRLFSET = 0x0;
    
    // DBGRUN disabled; 
    TCA1.SINGLE.DBGCTRL = 0x0;
    
    // CNTAEI disabled; CNTBEI disabled; EVACTA CNT_POSEDGE; EVACTB NONE; 
    TCA1.SINGLE.EVCTRL = 0x0;
    
    // CMP0 disabled; CMP1 disabled; CMP2 disabled; OVF enabled; 
    TCA1.SINGLE.INTCTRL = 0x1;
    
    // CMP0 disabled; CMP1 disabled; CMP2 disabled; OVF disabled; 
    TCA1.SINGLE.INTFLAGS = 0x0;
    
    // Period
    TCA1.SINGLE.PER = 0x3D08;
    
    // Temporary data for 16-bit Access
    TCA1.SINGLE.TEMP = 0x0;
    
    // CLKSEL DIV256; ENABLE enabled; RUNSTDBY disabled; 
    TCA1.SINGLE.CTRLA = 0xD;
    
}

void TCA1_Start(void)
{
    TCA1.SINGLE.CTRLA|= TCA_SINGLE_ENABLE_bm;
}

void TCA1_Stop(void)
{
    TCA1.SINGLE.CTRLA&= ~TCA_SINGLE_ENABLE_bm;
}

void TCA1_Write(uint16_t timerVal)
{
    TCA1.SINGLE.PER=timerVal;
}

uint16_t TCA1_Read(void)
{
    uint16_t readVal;

    readVal = TCA1.SINGLE.CNT;

    return readVal;
}

void TCA1_EnableInterrupt(void)
{
        TCA1.SINGLE.INTCTRL = 1 << TCA_SINGLE_CMP0_bp /* Compare 0 Interrupt: enabled */
	 				| 1 << TCA_SINGLE_CMP1_bp /* Compare 1 Interrupt: enabled */
	 				| 1 << TCA_SINGLE_CMP2_bp /* Compare 2 Interrupt: enabled */
	 				| 1 << TCA_SINGLE_OVF_bp; /* Overflow Interrupt: enabled */
}
void TCA1_DisableInterrupt(void)
{
    TCA1.SINGLE.INTCTRL = 0 << TCA_SINGLE_CMP0_bp /* Compare 0 Interrupt: disabled */
	 				| 0 << TCA_SINGLE_CMP1_bp /* Compare 1 Interrupt: disabled */
	 				| 0 << TCA_SINGLE_CMP2_bp /* Compare 2 Interrupt: disabled */
	 				| 0 << TCA_SINGLE_OVF_bp; /* Overflow Interrupt: disabled */
}
void TCA1_ClearOverflowInterruptFlag(void)
{
    TCA1.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm; /* Clear Overflow Interrupt Flag */
}
bool TCA1_IsOverflowInterruptFlagSet(void)
{
    return ((TCA1.SINGLE.INTFLAGS & TCA_SINGLE_OVF_bm) > 0);
}

void TCA1_ClearCMP0InterruptFlag(void)
{
    TCA1.SINGLE.INTFLAGS = TCA_SINGLE_CMP0_bm; /* Clear Compare Channel-0 Interrupt Flag */
}

bool TCA1_IsCMP0InterruptFlagSet(void)
{
    return ((TCA1.SINGLE.INTFLAGS & TCA_SINGLE_CMP0_bm) > 0);
}

void TCA1_ClearCMP1InterruptFlag(void)
{
    TCA1.SINGLE.INTFLAGS = TCA_SINGLE_CMP1_bm; /* Clear Compare Channel-1 Interrupt Flag */
}

bool TCA1_IsCMP1InterruptFlagSet(void)
{
    return ((TCA1.SINGLE.INTFLAGS & TCA_SINGLE_CMP1_bm) > 0);
}

void TCA1_ClearCMP2InterruptFlag(void)
{
    TCA1.SINGLE.INTFLAGS = TCA_SINGLE_CMP2_bm; /* Clear Compare Channel-2 Interrupt Flag */
}

bool TCA1_IsCMP2InterruptFlagSet(void)
{
    return ((TCA1.SINGLE.INTFLAGS & TCA_SINGLE_CMP2_bm) > 0);
}

void TCA1_WaveformFreqRegCountSet(cmpx_register_value_t count)
{
    //Write value to CMP0 register
    TCA1.SINGLE.CMP0=count;
}

uint16_t TCA1_WaveformFreqRegCountGet(void)
{
    //Returns CMP0 register value
    return TCA1.SINGLE.CMP0;
}

void TCA1_WO1OffsetRegCountSet(cmpx_register_value_t count)
{
    //Write value to CMP1 register
    TCA1.SINGLE.CMP1=count;
}

uint16_t TCA1_WO1OffsetRegCountGet(void)
{
    //Returns CMP1 register value
    return TCA1.SINGLE.CMP1;
}

void TCA1_WO2OffsetRegCountSet(cmpx_register_value_t count)
{
    //Write value to CMP2 register
    TCA1.SINGLE.CMP2=count;
}

uint16_t TCA1_WO2OffsetRegCountGet(void)
{
    //Returns CMP2 register value
    return TCA1.SINGLE.CMP2;;
}



void TCA1CounterSet(uint16_t value)
{
    //Write value to CNT register
    TCA1.SINGLE.CNT=value;
}

uint16_t TCA1CounterGet(void)
{
    //Returns CNT register value
    return TCA1.SINGLE.CNT;
}

void TCA1_Compare0Set(uint16_t value)
{
    //Write value to CMP0 register
    TCA1.SINGLE.CMP0=value;
}

void TCA1_Compare1Set(uint16_t value)
{
    //Write value to CMP1 register
    TCA1.SINGLE.CMP1=value;
}

void TCA1_Compare2Set(uint16_t value)
{
    //Write value to CMP2 register
    TCA1.SINGLE.CMP2=value;
}

void TCA1_ModeSet(TCA_SINGLE_WGMODE_t mode)
{
  uint8_t temp;
  if((mode == TCA_SINGLE_WGMODE_NORMAL_gc) ||
     (mode == TCA_SINGLE_WGMODE_FRQ_gc) ||
     (mode == TCA_SINGLE_WGMODE_SINGLESLOPE_gc) ||
     (mode == TCA_SINGLE_WGMODE_DSTOP_gc) ||
     (mode == TCA_SINGLE_WGMODE_DSBOTH_gc) ||
     (mode == TCA_SINGLE_WGMODE_DSBOTTOM_gc))
  {
      temp = (TCA1.SINGLE.CTRLB & ~TCA_SINGLE_WGMODE_gm) |
             (  mode     &  TCA_SINGLE_WGMODE_gm);
      TCA1.SINGLE.CTRLB = temp;
  }
}


