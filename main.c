/***********************************************************************
 *
 *	File Name: 		main.c
 *
 * 	Authors: 		Mahmoud Gaber
 *
 *	Created on:		Saturday, 11-JAN-20 10:15:18 PM
 *
 ************************************************************************/


/************************************************************************/
/*		                     INCLUDES	                                */
/************************************************************************/


#include "interrupt.h"
#include "app.h"
#include "Sleep_mode.h"

/****error checking***/
/***to check the error read this array****/
 uint8 gu8_checkerror_index2 =  0 ;
 uint8 gau8_errorcheck2[256] = {0};


void main(void)
{
	/****enable global interrupt ****/
	sei();

	/****Initiate the app***/
	App_Init();

	while(1)
	{
		/****call the dispatcher to serve the ready functions ****/
		gau8_errorcheck2[gu8_checkerror_index2++]=Tmu_Dispatcher();

		/****put CPU on sleep mode****/
		Sleep_Cpu();
	}
}

