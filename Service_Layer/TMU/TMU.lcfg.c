/******************************************************************************
 *
 * 	Module: 		TMU
 *
 *	File Name: 		TMU_lcfg.c
 *
 * 	Authors: 		Mahmoud Gaber
 *
 *	Created on:		Saturday, 11-JAN-20 10:15:18 PM
 *
 *******************************************************************************/

/************************************************************************/
/*	                        	 INCLUDES		            	        */
/************************************************************************/
#include "TMU.lcfg.h"
#include "Timer.h"


/*****Creat  the post build structure that contain the timer channel and the resolution*****/

  str_TMUCfg_t  gstr_Init =
 {
		 TIMER_0,
		 1
 };

