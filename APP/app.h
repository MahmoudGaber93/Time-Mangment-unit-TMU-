/******************************************************************************
 *
 * 	Module: 		APP
 *
 *	File Name: 		app.h
 *
 * 	Authors: 		Mahmoud Gaber
 *
 *	Created on:		Saturday, 11-JAN-20 10:15:18 PM
 *
 *******************************************************************************/
#ifndef APP_APP_H_
#define APP_APP_H_

/************************************************************************/
/*	                        	 INCLUDES		            	        */
/************************************************************************/

#include "common_macros.h"
#include "TMU.h"
#include "TMU.lcfg.h"
#include "led.h"

/************************************************************************/
/*		         TIMER FUNCTIONS' PROTOTYPES		                    */
/************************************************************************/



/*********************************************************************
 * Input: NONE.
 * Output:
 * In/Out:
 * Return: ERROR_STATUS.
 * Description: initialize the app
 *
 * *******************************************************************/

ERROR_STATUS App_Init(void);


void App_1(void);

void App_2(void);

void App_3(void);
#endif /* APP_APP_H_ */
