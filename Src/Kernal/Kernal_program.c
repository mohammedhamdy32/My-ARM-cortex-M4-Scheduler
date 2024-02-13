/*
 * Kernal_program.c
 *
 *  Created on: Feb 2, 2024
 *      Author: mohammedhamdy32
 */

#include "std_types.h"
#include "common_macros.h"

#include "OS_MCAL_interface.h"
#include "OS_MCAL_config.h"
#include "OS_MCAL_private.h"
#include "Kernal_interface.h"
#include "Kernal_config.h"

extern u8 g_currentTask;
extern TCB_t UserTasks[MAX_TASKS_NUM];
extern u32 g_GlobalTickCount;
extern u8  g_TasksNumber;

u32 g_UsedStackSize = IDEL_TASK_STACK_SIZE; /* Contains the amount of used stack  */
u8 g_SRAM_Arr[TOTAL_SRAM_SIZE]; /* Reserve a memory in RAM for Tasks, g_SRAM_Arr pointer is the start of our SRAM */
u32 g_SRAM_End =  (u32)( &(g_SRAM_Arr[TOTAL_SRAM_SIZE ]) ); /* End of the stack */

/* Initialize OS */
void OS_Init( void (*a_IdelTaskHandler)(void) )
{

	/*** Init Idel Task ***/
	UserTasks[0].TaskStackStart =  g_SRAM_End ;
	/* Initialize PSP values of Idle Tasks */
	UserTasks[0].TaskPSP = g_SRAM_End;
	/* Initialize Idle Task Handler */
	UserTasks[0].TaskHandler = a_IdelTaskHandler;
	UserTasks[0].TaskState = READY; /* Idle Task is always ready */
	Stack_voidInitTaskStack(); /* Init Idle Task stack */

	/*** Init Scheduler ***/
	Stack_voidSchedulerStackInit( SCHEDULER_STACK_START );

}




/* A Function used to create a Task during run time */
u8 OS_CreateTask( u32 a_task_stack_size , void (*a_Task_handler)(void)  )
{
	/* Increment Task number */
	g_TasksNumber++;

	/* If the number of Tasks reaches it's maximum */
	if( g_TasksNumber >= MAX_TASKS_NUM )
	{
		return 0;
	}

	UserTasks[g_TasksNumber].TaskStackStart =  ( (g_SRAM_End) - ( g_UsedStackSize ) ) ;
	g_UsedStackSize += a_task_stack_size;

	/* If I used all the size of my stack */
	if( g_UsedStackSize > TOTAL_SRAM_SIZE )
	{
		return 0;
	}

	/* Initialize PSP values of Tasks */
	UserTasks[g_TasksNumber].TaskPSP = UserTasks[g_TasksNumber].TaskStackStart;

	/* Initialize Task Handlers */
	UserTasks[g_TasksNumber].TaskHandler = a_Task_handler;
	/* Each Task starts in Ready state */
	UserTasks[g_TasksNumber].TaskState = READY;

	Stack_voidInitTaskStack(); /* Initialize the Stack of the Task */

	return 1;

}


/* A function that make a delay using kernal, to save CPU clocks */
void OS_TaskDelay(u32 a_delay )
{
	if( g_currentTask != 0 ) /* Check that the current Task is not the Idle Task */
	{
		ENTER_CRITICAL_SECTION(); /* used to close Exception because it's a critical section */
		UserTasks[g_currentTask].BlockingCount = a_delay + g_GlobalTickCount ; /* The blocking count contains number of ticks the task wants to wait + the current tick number */
		UserTasks[g_currentTask].TaskState = BLOCKING; /* Change the state of task to blocking state */
		SET_BIT( SCB_ICSR , 28 ); /* Setting PendSV pending bit to switch to another Task*/
		EXIT_CRITICAL_SECTION();
	}

}

/* Initialize Scheduler */
void OS_StartScheduler( u32 a_TickCounts )
{
	/*** Init Systick ***/
	SYSTICK_voidInit(a_TickCounts);
	ChangeToPSP();

	UserTasks[1].TaskHandler();  /* Start with Task1 */
}


/* Make a SVC Exception */
u32 SVC_execute( u8 a_svc_num )
{
	u32 data;
	switch(a_svc_num)
	{
	case 0:
		__asm volatile("SVC #0");
		break;
	case 1:
		__asm volatile("SVC #1");
		break;
	case 2:
		__asm volatile("SVC #2");
		break;
	default:
		__asm volatile("SVC #5");
	}
	__asm volatile("MOV %0,R0 " :"=r"(data) );
	return data;
}

/* SVC Handler  */
__attribute__ ((naked)) void SVC_Handler(void) /* Make function naked to avoid the first instructions that uses the stack at the begining of function */
{
	/* MRS is an instruction to read special register */
	__asm volatile("MRS R0 , MSP" ); /* Get MSP value and put it in R0 as an argument */

	__asm volatile("B SVC_Handeler_C"); /* Branch to void SVC_Handeler_C function */
}

/* SVC Handler in C */
void SVC_Handeler_C( u32 *a_MSP_value )
{
	/* a_MSP_value    ==> address of place that contains (R0,PC,...)
	 * (*a_MSP_value) ==> Register value (R0,PC,...)
	 **/
	a_MSP_value += 6; /* ADD 6 steps to point to the address of PC */
	u16 *PC_value = (u16 *)(*a_MSP_value); /* Get PC value */
	PC_value -= 1; /* Get the previous address in PC that is SVC instruction*/
	u8 SVC_value = *PC_value; /* Get SVC value */

	/* Put your cases her */
	switch(SVC_value)
	{
	case 0:

		break;
	case 5:
		/* I want to return a value to the task */
		/* I will save the return value in R0 that is place in stack */
		a_MSP_value -= 6; /* The stack will point to R0 */
		(*a_MSP_value) = SVC_value*2 ;/* Put the value that you want to return (10)  */

		break;

	}

}


