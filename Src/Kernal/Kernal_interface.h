/*
 * Kernal_interface.h
 *
 *  Created on: Feb 2, 2024
 *      Author: mohammedhamdy32
 */

#ifndef KERNAL_KERNAL_INTERFACE_H_
#define KERNAL_KERNAL_INTERFACE_H_



/* Scheduler */
#define  SCHEDULER_STACK_START  ( (g_SRAM_End) + ( SCHEDULER_STACK_SIZE ) )


void OS_TaskDelay(u32 a_delay );
void OS_Init( void (*a_IdelTaskHandler)(void) );
u8 OS_CreateTask( u32 a_task_stack_size , void (*a_Task_handler)(void)  );
void OS_StartScheduler( u32 a_TickCounts );
u32 SVC_execute( u8 a_svc_num );
void SVC_Handeler_C( u32 *a_MSP_value );


#endif /* KERNAL_KERNAL_INTERFACE_H_ */
