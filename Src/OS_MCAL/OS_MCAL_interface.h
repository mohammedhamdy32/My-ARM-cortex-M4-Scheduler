/*
 * SYSTICK_interface.h
 *
 *  Created on: Oct 1, 2023
 *      Author: mohammedhamdy32
 */

#ifndef SYSTICK_INTERFACE_H_
#define SYSTICK_INTERFACE_H_

/*************************** Defines **********************************/
#define INFINITY_TIMES    -1
#define DUMMY_XPSR         (0x01000000)
#define DUMMY_LR           (0xFFFFFFFD)



/***************** Prototypes ****************************/
/* SYSTICK */
void SYSTICK_voidInit(u32 a_number_of_delay_ticks );


__attribute__ ((naked)) void Stack_voidSchedulerStackInit( u32 a_ShedulerSatckTop );
void Stack_voidTasksStack( void );
__attribute__ ((naked)) void ChangeToPSP(void);
void Stack_voidInitTaskStack( void );




/*************************** Struct *****************************/
typedef enum{
	READY , BLOCKING
}TASK_STATE_t;

typedef struct {
	TASK_STATE_t TaskState;
	u32 BlockingCount;
	u32 TaskPSP;
	void (*TaskHandler)(void);
	u32 TaskStackStart;

}TCB_t; /* Task Control Block */





/***************** Macros ****************************/
#define  ENTER_CRITICAL_SECTION()   {          \
		__asm__ volatile( "PUSH {R0}" ) ;      \
		__asm__ volatile("MOV R0,#1");         \
		__asm__ volatile("MSR PRIMASK,R0");    \
		__asm__ volatile("POP {R0}");          \
}

#define  EXIT_CRITICAL_SECTION()    {         \
		__asm__ volatile( "PUSH {R0}" ) ;     \
		__asm__ volatile("MOV R0,#0");        \
		__asm__ volatile("MSR PRIMASK,R0");   \
		__asm__ volatile("POP {R0}");         \
}


#endif /* SYSTICK_INTERFACE_H_ */
