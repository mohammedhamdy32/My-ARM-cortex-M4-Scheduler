/*
 * SYSTICK_program.c
 *
 *  Created on: Oct 1, 2023
 *      Author: mohammedhamdy32
 */

#include "std_types.h"
#include "common_macros.h"

#include "OS_MCAL_config.h"
#include "OS_MCAL_interface.h"
#include "OS_MCAL_private.h"

#include "Tasks.h"


/* Make a Global struct which contains all the information of my tasks */
TCB_t UserTasks[MAX_TASKS_NUM];


/* A Global variable contains the current Task that is running */
u8 g_currentTask = 1 ; /* Starts with Task1 */
u32 g_GlobalTickCount=0;  /* Counts the number of Ticks that Scheduler do, The first default Task is the Idle Task */
u8  g_TasksNumber=0; /* Contains the number of initialized Tasks in our system */


/* Initialize Systick timer */
void SYSTICK_voidInit(u32 a_number_of_delay_ticks )
{

	STK_LOAD = a_number_of_delay_ticks -1; /* Put the number of ticks in reload value */
	SET_BIT( STK_CTRL , TICKINT );  /* Open exception */

#if ( SYSTICK_CLOCK == AHB_DIV_8 )
	CLEAR_BIT( STK_CTRL , CLKSOURCE );
#else
	SET_BIT( STK_CTRL , CLKSOURCE );
#endif

	SET_BIT( STK_CTRL , ENABLE ); /* Enable timer */
}

/* Disable systick */
void SYSTICK_voidDisable(void)
{
	CLEAR_BIT( STK_CTRL , ENABLE );  /* Disable timer */
	CLEAR_BIT( STK_CTRL , TICKINT ); /* Disable exception  */
	STK_VAL  = 0; /* Make it count from zero */
	STK_LOAD = 0;
}



static void HelperFun_SavePSP_value(u32 a_PSP_Value )
{
	UserTasks[g_currentTask].TaskPSP = a_PSP_Value;
}

static void HelperFun_IncrementCurrentTask(void)
{
	g_currentTask = ( (g_currentTask+1) % (g_TasksNumber+1) ) ;
	for( u8 i=0 ; i<g_TasksNumber+1 ; i++ )
	{
		if( UserTasks[g_currentTask].TaskState == READY && g_currentTask != 0 ) /* The Task is in Ready state and Task is not Idle task */
			return;
		g_currentTask = ( (g_currentTask+1) % (g_TasksNumber+1) ) ;

	}
	g_currentTask = 0; /* All Tasks are in Blocking state, so run Idle Task */


}

/* Function used to unblock the Tasks that it's blocking time is finished */
static void unblockTasks(void)
{
	for(u8 i=1 ; i<MAX_TASKS_NUM ; i++)
	{
		if( UserTasks[i].TaskState == BLOCKING ) /* The Task is in Blocking state */
			if( UserTasks[i].BlockingCount == g_GlobalTickCount ) /* The Task reaches the blocking count */
			{
				UserTasks[i].TaskState = READY;
			}
	}
}

/* ISR of SYSTICK */
void SysTick_Handler(void)
{


	/* Increment global Tick count */
	g_GlobalTickCount++;

	/* unblock Tasks that it's blocking time is finished  */
	unblockTasks();

	/* Trigger PendSV */
	SET_BIT( SCB_ICSR , 28 ); /* Setting PendSV pending bit */


}

/* PendSV Handler, which will make context switching */
__attribute__ ((naked)) void PendSV_Handler(void)
{
	/*** Save context of current Task ***/
	__asm__ volatile("MRS R0,PSP");  /* Save values of PSP in R0 */
	/* We are in Scheduler exception, so we can't use PSP as SP, we will use MSP, so I can't use push instruction */
	__asm__ volatile("STMDB R0! , {R4-R11} "); /* STMDB mean store multiple registers with decrement each time,
	                                              ! means at the end store the final address back to R0, which will be the final value of PSP */
	__asm__ volatile("PUSH {LR}"); /* I will enter a function, so I should save value of LR in stack */
	__asm__ volatile("BL HelperFun_SavePSP_value"); /* Save PSP value in global array */

	/*** Retrieve context of next Task  ***/
	__asm__ volatile("BL HelperFun_IncrementCurrentTask"); /* Increment currentTask global variable */
	__asm__ volatile("MOV R0,%0" :: "r" (UserTasks[g_currentTask].TaskPSP) ); /* Get PSP value of this task */
	__asm__ volatile("LDMIA R0! , {R4-R11} "); /* LDMIA mean Load multiple registers with increment each time,
	   	                                                ! means at the end store the final address back to R0, which will be the final value of PSP */
	__asm__ volatile("MSR PSP,R0"); /* Put PSP value of the next task in PSP register */
	__asm__ volatile("POP {LR}"); /* Get value of the pushed LR */
	__asm__ volatile("BX LR"); /* Exit */
}


/* This function will change MSP register, so it should be naked because it will be assembly only */
__attribute__ ((naked)) void Stack_voidSchedulerStackInit( u32 a_ShedulerSatckTop )
{
	//	__asm__ volatile(" MSR MSP , %0 "::"r"(a_ShedulerSatckTop) );
	__asm__ volatile("MSR MSP,R0"); /* The argument is saved in R0 */
	__asm__ volatile("BX LR"); /* return to LR address */
}


/* Function used to initialize PSP register with the PSP value of First task, then switch from MSP to PSP */
__attribute__ ((naked)) void ChangeToPSP(void)
{

	/* Initialize PSP before switching to it with Task1 */
	__asm__ volatile ( "MOV R0,%0" :: "r" (UserTasks[g_currentTask].TaskPSP) ); /* Put PSP value in R0 */
	__asm__ volatile ("MSR PSP,R0"); /* Move R0 value to PSP */

	/* Change SP to PSP */
	__asm__ volatile ("MOV R0,#0x02");
	__asm__ volatile ("MSR CONTROL,R0"); /* Change bit one to 1 in CONTROL register to switch to PSP */

	/* Branch */
	__asm__ volatile ("BX LR");

}


/* Initialize Tasks stack */
void Stack_voidInitTaskStack( void )
{
	u32 * Local_pointer ;

	Local_pointer = (u32*)(UserTasks[g_TasksNumber].TaskPSP);
	Local_pointer--;
	*(Local_pointer) = DUMMY_XPSR;      /* Initialize first place in Task stack which will save the value of XPSR */
	Local_pointer--;
	*(Local_pointer) = (u32)UserTasks[g_TasksNumber].TaskHandler; 	  /* Initialize PC value in Task's stack  */
	Local_pointer--;
	*(Local_pointer) = DUMMY_LR ;  /* Initialize LR value in Task's stack  */
	/*  GPRS  */
	Local_pointer--;
	*(Local_pointer) = 0; /* R12 */
	Local_pointer--;
	*(Local_pointer) = 0; /* R3 */
	Local_pointer--;
	*(Local_pointer) = 0; /* R2 */
	Local_pointer--;
	*(Local_pointer) = 0; /* R1 */
	Local_pointer--;
	*(Local_pointer) = 0; /* R0 */
	Local_pointer--;
	*(Local_pointer) = 0; /* R11 */
	Local_pointer--;
	*(Local_pointer) = 0; /* R10 */
	Local_pointer--;
	*(Local_pointer) = 0; /* R9 */
	Local_pointer--;
	*(Local_pointer) = 0; /* R8 */
	Local_pointer--;
	*(Local_pointer) = 0; /* R7 */
	Local_pointer--;
	*(Local_pointer) = 0; /* R6 */
	Local_pointer--;
	*(Local_pointer) = 0; /* R5 */
	Local_pointer--;
	*(Local_pointer) = 0; /* R4 */

	UserTasks[g_TasksNumber].TaskPSP = (u32)Local_pointer;

}















