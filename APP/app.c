/******************************************************************************
 *
 * 	Module: 		APP
 *
 *	File Name: 		app.c
 *
 * 	Authors: 		Mahmoud Gaber
 *
 *	Created on:		Saturday, 11-JAN-20 10:15:18 PM
 *
 *******************************************************************************/

/************************************************************************/
/*	                        	 INCLUDES		            	        */
/************************************************************************/

#include "app.h"


/*********************************************************************
 * Input: NONE.
 * Output:
 * In/Out:
 * Return: ERROR_STATUS.
 * Description: initialize the app.
 *
 * *******************************************************************/

/****error checking***/
/***to check the error read this array****/
uint8 gau8_errorcheck1[20] = {0};
uint8 gu8_checkerror_index = 0;

ERROR_STATUS App_Init(void)
{
	gau8_errorcheck1[gu8_checkerror_index++] = Sos_Init();
	Led_Init(LED_0);
	Led_Init(LED_1);
	Led_Init(LED_2);

	str_TMUfg_t Fun1;
	Fun1.Tmu_AppFunc_cbk = App_1;
	Fun1.Fun_State = PERIODIC;
	Fun1.counter=0;
	Fun1.delay =100;
	gau8_errorcheck1[gu8_checkerror_index++] = Tmu_Start(&Fun1);

	str_TMUfg_t Fun2;
	Fun2.Tmu_AppFunc_cbk = App_2;
	Fun2.Fun_State = PERIODIC;
	Fun2.counter=0;
	Fun2.delay =30;
	gau8_errorcheck1[gu8_checkerror_index++] = Tmu_Start(&Fun2);

	str_TMUfg_t Fun3;
	Fun3.Tmu_AppFunc_cbk = App_3;
	Fun3.Fun_State = PERIODIC;
	Fun3.counter=0;
	Fun3.delay =20;
	gau8_errorcheck1[gu8_checkerror_index++] = Tmu_Start(&Fun3);

	return E_OK;

}


void App_1(void)
{
	Led_Toggle(LED_0);

}

void App_2(void)
{
	Led_Toggle(LED_1);

}
void App_3(void)
{
    Led_Toggle(LED_2);

}
