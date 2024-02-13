/*
 * Tasks.c
 *
 *  Created on: Jan 29, 2024
 *      Author: mohammedhamdy32
 */

#include "std_types.h"
#include "common_macros.h"

#include "GPIO_interface.h"
#include  "RCC_interface.h"

void Task1(void)
{
	RCC_voidInitSysCls();
	RCC_voidEnablePerpheralClk( AHB1 , GPIOB_ID );
	GPIO_PIN_CONFIG_T gpio_config = { PORTB_ID , PIN12_ID , GPIO_OUTPUT , 0 , PUSH_PULL_MODE , LOW_SPEED , 0 };
	GPIO_voidInti( &gpio_config );

	while(1)
	{
		GPIO_voidSetPin( PORTB_ID , PIN12_ID , LOW );
		OS_TaskDelay( 1000 );
		GPIO_voidSetPin( PORTB_ID , PIN12_ID , HIGH );
		OS_TaskDelay( 1000 );
	}

}

void Task2(void)
{
	GPIO_PIN_CONFIG_T gpio_config = { PORTB_ID , PIN13_ID , GPIO_OUTPUT , 0 , PUSH_PULL_MODE , LOW_SPEED , 0 };
	GPIO_voidInti( &gpio_config );

	while(1)
	{
		GPIO_voidSetPin( PORTB_ID , PIN13_ID , LOW );
		OS_TaskDelay( 500 );
		GPIO_voidSetPin( PORTB_ID , PIN13_ID , HIGH );
		OS_TaskDelay( 500 );
	}
}

void Task3(void)
{


	GPIO_PIN_CONFIG_T gpio_config = { PORTB_ID , PIN14_ID , GPIO_OUTPUT , 0 , PUSH_PULL_MODE , LOW_SPEED , 0 };
	GPIO_voidInti( &gpio_config );

	while(1)
	{
		GPIO_voidSetPin( PORTB_ID , PIN14_ID , LOW );
		OS_TaskDelay( 250 );
		GPIO_voidSetPin( PORTB_ID , PIN14_ID , HIGH );
		OS_TaskDelay( 250 );
	}
}


void Task4(void)
{
	GPIO_PIN_CONFIG_T gpio_config = { PORTB_ID , PIN15_ID , GPIO_OUTPUT , 0 , PUSH_PULL_MODE , LOW_SPEED , 0 };
	GPIO_voidInti( &gpio_config );

	while(1)
	{
		GPIO_voidSetPin( PORTB_ID , PIN15_ID , LOW );
		OS_TaskDelay( 125 );
		GPIO_voidSetPin( PORTB_ID , PIN15_ID , HIGH );
		OS_TaskDelay( 125 );
	}
}

void IdleTask(void)
{

	while(1)
	{

	}
}
