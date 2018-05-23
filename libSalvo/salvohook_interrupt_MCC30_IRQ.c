/************************************************************ 
Copyright (C) 1995-2006 Pumpkin, Inc. and its
Licensor(s). Freely distributable.

$Source: C:\\RCS\\D\\Pumpkin\\Salvo\\Src\\MCC30\\salvohook_interrupt_MCC30_IRQ.c,v $
$Author: aek $
$Revision: 3.0 $
$Date: 2007-11-14 10:23:55-08 $

Most basic example of user-defined interrupt control functions. 
Can be replaced by user functions that control interrupts 
associated with Salvo services.

************************************************************/
#include <salvo.h>


void OSDisableHook(void)
{
	__asm__ volatile("disi #0x3FFF");
}

void OSEnableHook(void)
{
	__asm__ volatile("disi #0x0000");
}

