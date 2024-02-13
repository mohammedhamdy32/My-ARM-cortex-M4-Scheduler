/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Mohammed hamdy
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#include "std_types.h"
#include "Kernal_interface.h"
#include "Tasks.h"






int main(void)
{
	OS_Init(  &IdleTask );
	OS_CreateTask( 1024 , &Task1 );
	OS_CreateTask( 1024 , &Task2 );
	OS_CreateTask( 1024 , &Task3 );
	OS_CreateTask( 1024 , &Task4 );

	OS_StartScheduler(10000);


	for(;;);
}





