/************************************************************ 
Copyright (C) 1995-2002 Pumpkin, Inc. and its
Licensor(s). Freely distributable.

$Source: C:\\RCS\\D\\Pumpkin\\Salvo\\Example\\PIC\\PIC24\\Simulator\\Tut\\Tut5\\MCC30\\Lite\\salvocfg.h,v $
$Author: aek $
$Revision: 3.0 $
$Date: 2007-06-27 13:29:29-07 $

Header file for tutorial programs.

************************************************************/

#define OSUSE_LIBRARY				TRUE
#define OSLIBRARY_TYPE				OSF
#define OSLIBRARY_CONFIG			OST

#define OSTASKS                     4
#define OSEVENTS					8
#define OSEVENT_FLAGS				1
#define OSMESSAGE_QUEUES			1

#define OSENABLE_EVENTS				TRUE


//==================================================
// Definitions automatically made by salvo headers
//==================================================

/*
salvoscb.h

OSBYTES_OF_EVENT_FLAGS = 1
*/

/*
Since OSLIBRARY_CONFIG == OST, salvo.h defines the
next macros as follows

#define OSBYTES_OF_DELAYS               1
#define OSBYTES_OF_TICKS                4
#define OSDISABLE_TASK_PRIORITIES       FALSE
#define OSENABLE_BINARY_SEMAPHORES      TRUE
#define OSENABLE_CYCLIC_TIMERS          TRUE
#define OSENABLE_EVENT_FLAGS            TRUE
#define OSENABLE_IDLING_HOOK            TRUE
#define OSENABLE_MESSAGES               TRUE
#define OSENABLE_MESSAGE_QUEUES         TRUE
#define OSENABLE_SEMAPHORES             TRUE
#define OSENABLE_TIMEOUTS               TRUE

#define OSENABLE_BINARY_SEMAPHORES	TRUE
#define OSENABLE_EVENT_FLAGS		TRUE

#define OSENABLE_MESSAGES			TRUE
#define OSENABLE_MESSAGE_QUEUES		FALSE
#define OSENABLE_SEMAPHORES			FALSE
*/

/*
#define OSUSE_LIBRARY				TRUE
#define OSLIBRARY_TYPE				OSF
#define OSLIBRARY_CONFIG			OST

#define OSEVENTS					1
#define OSEVENT_FLAGS				0
#define OSMESSAGE_QUEUES			0
#define OSTASKS                     3
*/

/*
OSENABLE_BINARY_SEMAPHORES,
OSENABLE_EVENT_FLAGS,
OSENABLE_EVENTS, OSENABLE_MESSAGES,
OSENABLE_MESSAGE_QUEUES,
OSENABLE_SEMAPHORES, OSEVENT_FLAGS,
OSTASKS, OSMESSAGE_QUEUES
*/