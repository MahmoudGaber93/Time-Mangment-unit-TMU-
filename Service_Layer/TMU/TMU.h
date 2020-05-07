/******************************************************************************
 *
 * 	Module: 		TMU
 *
 *	File Name: 		TMU.h
 *
 * 	Authors: 		Mahmoud Gaber
 *
 *	Created on:		Saturday, 11-JAN-20 10:15:18 PM
 *
 *******************************************************************************/
#ifndef SERVICE_LAYER_TMU_TMU_H_
#define SERVICE_LAYER_TMU_TMU_H_


/************************************************************************/
/*	                        	 INCLUDES		            	        */
/************************************************************************/

#include "std_types.h"
#include "common_macros.h"



/************************************************************************/
/*				                   DEFINES	            		        */
/************************************************************************/

#define ONE_SHOT 0
#define PERIODIC 1

/************************************************************************/
/*			                Structures Definitions		                */
/************************************************************************/

typedef struct str_TMUfg_t
{

uint8 counter;
uint8 delay;
gptrfun_cbk_t  Tmu_AppFunc_cbk;
uint8 Fun_State;

}str_TMUfg_t;


/************************************************************************/
/*		         TIMER FUNCTIONS' PROTOTYPES		                    */
/************************************************************************/



/** *****************************************************************
 * Input: NONE
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: Initiates the TMU module.
 *
 * *******************************************************************/
ERROR_STATUS Sos_Init(void);


/** ***************************************************************
 * Input: NONE.
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: Checking on the valid function in certain time and call it .
 *
 *******************************************************************/
ERROR_STATUS Tmu_Dispatcher(void);

/********************************************************************
 * Input: Pointer to a structure contains the information needed to initialize the timer and start it .
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: Initiates the timer and start it and store the new function in the buffer.
 *
 ********************************************************************/
ERROR_STATUS Tmu_Start(str_TMUfg_t * ptrstr_TmuFun);

/*********************************************************************
 * Input: Pointer to function.
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: remove the function from the  TMU buffer.
 *
 **********************************************************************/
ERROR_STATUS Tmu_Stop( gptrfun_cbk_t Fun_Adrress);

/***********************************************************************
 * Input:  NONE.
 * Output:
 * In/Out:
 * Return:  The error status of the function.
 * Description: deInitiates the module.
 *
 ************************************************************************/
ERROR_STATUS Tmu_Deinit(void);

#endif /* SERVICE_LAYER_TMU_TMU_H_ */
