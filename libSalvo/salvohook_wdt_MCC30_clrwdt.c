/************************************************************ 
The contents of this file are subject to the Pumpkin Salvo
License (the "License").  You may not use this file except
in compliance with the License. You may obtain a copy of
the License at http://www.pumpkininc.com, or from
warranty@pumpkininc.com.

Software distributed under the License is distributed on an
"AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express
or implied. See the License for specific language governing
the warranty and the rights and limitations under the
License.

The Original Code is Salvo - The RTOS that runs in tiny
places(TM). Copyright (C) 1995-2006 Pumpkin, Inc. and its
Licensor(s). All Rights Reserved.

$Source: C:\\RCS\\D\\Pumpkin\\Salvo\\Src\\MCC30\\salvohook_wdt_MCC30_clrwdt.c,v $
$Author: aek $
$Revision: 3.2 $
$Date: 2010-01-24 15:13:13-08 $

For Microchip MPLAB C30 C compiler and PIC24/dsPIC targets.

Most basic example of user-defined watchdog-clearing function. 

Can be replaced by user function.

************************************************************/
#include <salvo.h>


void OSClrWDTHook(void)
{
  __asm__ volatile("clrwdt");
}

