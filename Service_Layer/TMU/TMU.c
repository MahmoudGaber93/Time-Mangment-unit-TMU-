/******************************************************************************
 *
 * 	Module: 		TMU
 *
 *	File Name: 		TMU.c
 *
 * 	Authors: 		Mahmoud Gaber
 *
 *	Created on:		Saturday, 11-JAN-20 10:15:18 PM
 *
 *******************************************************************************/

/************************************************************************/
/*	                        	 INCLUDES		            	        */
/************************************************************************/

#include "TMU.h"
#include "TMU.lcfg.h"
#include "TMU_cfg.h"
#include "Timer.h"
#include "error_check.h"

/************************************************************************/
/*				                   DEFINES	            		        */
/************************************************************************/
#define TMU_INITALIZED       (1)
#define TMU_NOT_INITALIZED   (0)

#define STARTED              (1)
#define NOT_STARTED          (0)

#define ONE                  (1)
#define ZERO                 (0)

#define TIMER_COUNTS_NEEDED  (250)




/**array of structure [TMU Buffer]**/
static   str_TMUfg_t  gastr_TmuBuffer[MAX_NUM_FUN];


/************************************************************************/
/*		                     GLOBAL VARIABLES	                        */
/************************************************************************/

static   uint8  gu8_Tmu_State   = TMU_NOT_INITALIZED;
volatile static uint8 gu8_BufferTop = ZERO;
static   uint8  gu8_timer_state = NOT_STARTED;
static   uint8  gu8_Ovf_Tflag        = LOW;
/** *******************************************************************
 * Input: NONE
 * Output:
 * In/Out:
 * Return: NONE.
 * Description: call back function set the flag to high  and reload the timer  .
 *
 ********************************************************************/
void Timer_ISR(void)
{
	gu8_Ovf_Tflag =HIGH;
	Timer_Set(gstr_Init.TIMER_NO,TIMER_COUNTS_NEEDED);
}

/*********************************************************************
 * Input: NONE.
 * Output:
 * In/Out:
 * Return: NONE.
 * Description: initialize all global objects.
 *
 * *******************************************************************/
void Clear_objects(void)
{
	uint8 u8_index =ZERO;
	/****initialize the elements of structure *********/
	for(u8_index = ZERO ; u8_index < MAX_NUM_FUN ; u8_index++)
	{
		gastr_TmuBuffer[u8_index].Fun_State = ONE_SHOT;
		gastr_TmuBuffer[u8_index].Tmu_AppFunc_cbk = NULL;
		gastr_TmuBuffer[u8_index].counter = ZERO;
		gastr_TmuBuffer[u8_index].delay = ZERO;

	}
	gu8_Tmu_State   =TMU_NOT_INITALIZED;
	gu8_BufferTop =ZERO;
	gu8_timer_state = NOT_STARTED;
	gu8_Ovf_Tflag=LOW;
}

/*********************************************************************
 * Input: NONE
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: Initiates the TMU module.
 *
 ********************************************************************/
ERROR_STATUS Sos_Init(void)
{
	/***initialize all global objects***/
	Clear_objects();

	/*******error state*********/
	uint8 u8_error_val = TMU_ERROR_BASE + TMU_SUCSSECE;

	/****check if the Tmu module is initialized or not****/
	if(gu8_Tmu_State != TMU_INITALIZED )
	{
		/****initiate the timer channel****/
		Timer_cfg_s TIMER_Init_st;
		TIMER_Init_st.Timer_CH_NO = gstr_Init.TIMER_NO;
		TIMER_Init_st.Timer_Mode= TIMER_MODE;
		TIMER_Init_st.Timer_Polling_Or_Interrupt = TIMER_INTERRUPT_MODE;
		TIMER_Init_st.Timer_Cbk_ptr =Timer_ISR;

		/****switch on the timer channels****/
		switch(gstr_Init.TIMER_NO)
		{
		case TIMER_0:
			/******choose the prescaler*****/
			TIMER_Init_st.Timer_Prescaler=TIMER_PRESCALER_64;
			break;
		case TIMER_1:
			TIMER_Init_st.Timer_Prescaler=TIMER_PRESCALER_256;
			break;
		case TIMER_2:
			TIMER_Init_st.Timer_Prescaler=TIMER_PRESCALER_64;
			break;
		default:
			u8_error_val = TMU_ERROR_BASE + TMU_UNVALID_INPUT;

		}

		Timer_Init(&TIMER_Init_st);


		/*******error state*********/
		gu8_Tmu_State = TMU_INITALIZED;
	}
	else
	{
		/*******error state*********/
		u8_error_val = TMU_ERROR_BASE + TMU_ALREADYINITALIZED;;
	}
	/*******return error state*********/
	return u8_error_val;
}

/*********************************************************************
 * Input: NONE.
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: Checking on the valid function in certain time and call it .
 *
 * *******************************************************************/

ERROR_STATUS Tmu_Dispatcher(void)
{
	/*******error state*********/
	uint8 u8_error_val = TMU_ERROR_BASE + TMU_SUCSSECE;

	/****local variable used to loop on the buffer*******/
	volatile  uint8 u8_count=ZERO;

	/*****check if the ISR fire or not ******/
	if(gu8_Ovf_Tflag == HIGH)
	{

		/********loop on the buffer to check if the functions ready to serve or not *******/
		for(u8_count =ZERO; u8_count < gu8_BufferTop ;u8_count++)
		{
			/****increasing the counter till it reach to the required time for this function to be serve when it reach*****/
			/*****every function have its counter to calculate its delay******/
			gastr_TmuBuffer[u8_count].counter++;

			if(gastr_TmuBuffer[u8_count].counter == gastr_TmuBuffer[u8_count].delay )
			{
				gastr_TmuBuffer[u8_count].Tmu_AppFunc_cbk();
				/*****check if the function will be served one shot or periodic******/
				if(gastr_TmuBuffer[u8_count].Fun_State == ONE_SHOT)
				{
					/******stop serving this function cause its state is one shot ****/
					Tmu_Stop(gastr_TmuBuffer[u8_count].Tmu_AppFunc_cbk);
				}
				else
				{

				}
				/****put zero on the counter for the next time ****/
				gastr_TmuBuffer[u8_count].counter = ZERO;
			}
		}
		/****make the flag low to make sure that it will go thiow this function in the next interrupt firing *****/
		gu8_Ovf_Tflag = LOW;
	}
	/*****return the error state****/
	return u8_error_val;

}


/*********************************************************************
 * Input: Pointer to a structure contains the information needed to initialize the timer and start it .
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: Initiates the timer and start it and store the new function in the buffer.
 *
 * *******************************************************************/
ERROR_STATUS Tmu_Start(str_TMUfg_t * ptrstr_TmuFun)
{
	/*****error state****/
	uint8 u8_error_val = TMU_ERROR_BASE + TMU_SUCSSECE;

	if(ptrstr_TmuFun->Tmu_AppFunc_cbk != NULL)
	{
		if(gu8_Tmu_State == TMU_INITALIZED)
		{
			gastr_TmuBuffer[gu8_BufferTop] .Fun_State       = ptrstr_TmuFun->Fun_State;
			gastr_TmuBuffer[gu8_BufferTop] .Tmu_AppFunc_cbk = ptrstr_TmuFun->Tmu_AppFunc_cbk;
			gastr_TmuBuffer[gu8_BufferTop] .counter         = ptrstr_TmuFun->counter;
			gastr_TmuBuffer[gu8_BufferTop] .delay           = ptrstr_TmuFun->delay;
			gu8_BufferTop++;

			/***check if the timer started or not and if not then start it *****/
			if(gu8_timer_state != STARTED)
			{
				Timer_Start(gstr_Init.TIMER_NO,TIMER_COUNTS_NEEDED);
				gu8_timer_state = STARTED;
			}
			else
			{

			}
		}
		else
		{
			/*****error state****/
			u8_error_val = TMU_ERROR_BASE + TMU_NOTINITALIZED;
		}
	}

	else
	{
		/*****error state****/
		u8_error_val = TMU_ERROR_BASE + TMU_NULL_POINTER;

	}
	/*****return the error state****/
	return u8_error_val;
}

/*********************************************************************
 * Input: Pointer to function.
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: remove the function from the  TMU buffer.
 *
 * *******************************************************************/
ERROR_STATUS Tmu_Stop(gptrfun_cbk_t Fun_Adrress)
{
	volatile uint8 u8_count=ZERO;

	/*****error state****/
	uint8 u8_error_val = TMU_ERROR_BASE + TMU_SUCSSECE;

	if(Fun_Adrress != NULL)
	{
		if(gu8_BufferTop != ZERO)
		{
			/*****try to find the index of the required function  in the buffer***/
			while(Fun_Adrress!= gastr_TmuBuffer[u8_count].Tmu_AppFunc_cbk  &&  u8_count < gu8_BufferTop)
			{
				u8_count++;
			}

			/****in case function has been found  and its index equal to the top ****/
			if(u8_count == gu8_BufferTop - ONE)
			{
				/****clear all data and decrease the top by one***/
				gastr_TmuBuffer[u8_count].Tmu_AppFunc_cbk =NULL;
				gastr_TmuBuffer[u8_count].Fun_State = FALSE;
				gastr_TmuBuffer[u8_count].counter =ZERO;
				gastr_TmuBuffer[u8_count].delay =ZERO;

				/***decrease the buffer top by 1***/
				gu8_BufferTop--;

			}
			/****in case function has been found  and its index less than the top ****/

			else if(u8_count < gu8_BufferTop - ONE)
			{
				/***Equal the this  element by the last one****/
				gastr_TmuBuffer[u8_count]=gastr_TmuBuffer[gu8_BufferTop - ONE];
				/***decrease the buffer by 1***/
				gu8_BufferTop--;

			}
			else
			{		/*****error state****/
				u8_error_val = TMU_ERROR_BASE + TMU_UNVALID_INPUT;
				/*the count bigger than index*/
			}
		}
		else
		{
			/*****error state****/
			u8_error_val = TMU_ERROR_BASE + TMU_EMPTY_BUFFER;
		}
	}

	else
	{		/*****error state****/
		u8_error_val = TMU_ERROR_BASE + TMU_NULL_POINTER;

	}
	/*****return the error state****/
	return u8_error_val;
}



/*********************************************************************
 * Input: NONE.
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: deInitiates the module.
 *
 * *******************************************************************/
ERROR_STATUS Tmu_Deinit(void)
{
	/*****error state****/
	uint8 u8_error_val = TMU_ERROR_BASE + TMU_SUCSSECE;

	/***check if the Tmu initiated or not *****/
	if(gu8_Tmu_State  != TMU_NOT_INITALIZED )
	{
		/****deinitite the timer****/
		Timer_deinit(gstr_Init.TIMER_NO);

		/***initialize all global objects ***/
		Clear_objects();
		/***make the state of the Tmu not initialized****/
		gu8_Tmu_State = TMU_NOT_INITALIZED;

	}
	else
	{
    	/*****error state****/
		u8_error_val = TMU_ERROR_BASE + TMU_NOTINITALIZED;

	}
	/*****return the error state****/
	return u8_error_val;
}
