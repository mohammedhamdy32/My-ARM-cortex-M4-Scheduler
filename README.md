# **ARM Cortex M4 Scheduler**

This repository contains an implementation of a scheduler for ARM Cortex M4 processors based on the Round Robin algorithm. The scheduler leverages the capabilities of ARM Cortex M4 processors for real-time operating systems (RTOS), utilizing features such as PendSV and SVC.

## Features

- **Round Robin Algorithm**: The scheduler employs the Round Robin algorithm for task scheduling, ensuring fair execution among tasks.
- **ARM Cortex M4 Support**: The scheduler utilizes features specific to ARM Cortex M4 processors, including PendSV and SVC, to manage task scheduling efficiently.
- **Functions**:
  - `void OS_TaskDelay(u32 a_delay)`: Allows tasks to delay their execution for a specified duration.
  - `void OS_Init(void (*a_IdleTaskHandler)(void))`: Initializes the scheduler, specifying the handler for the idle task.
  - `u8 OS_CreateTask(u32 a_task_stack_size, void (*a_Task_handler)(void))`: Creates a new task with the specified stack size and handler function.
  - `void OS_StartScheduler(u32 a_TickCounts)`: Starts the scheduler with the specified tick counts.
  - `u32 SVC_execute(u8 a_svc_num)`: Executes the specified supervisor call (SVC) number.
  - `void SVC_Handler_C(u32 *a_MSP_value)`: Handles supervisor calls (SVC) in C language.

## Usage

To use the ARM Cortex M4 Scheduler in your project, follow these steps:

1. Include the scheduler files (`scheduler.c` and `scheduler.h`) in your project.
2. Configure your project settings to support ARM Cortex M4 processors.
3. Initialize the scheduler using `OS_Init()` and specify the idle task handler.
4. Create tasks using `OS_CreateTask()` and provide the task handler functions.
5. Start the scheduler using `OS_StartScheduler()` with the desired tick counts.
6. Implement the task handler functions to define the behavior of each task.
7. Customize the scheduler settings and parameters to suit your specific requirements.

## Example

```c
#include "scheduler.h"

// Task handler functions
void Task1_Handler(void) {
    // Task 1 behavior
}

void Task2_Handler(void) {
    // Task 2 behavior
}

int main(void) {
    // Initialize scheduler
    OS_Init(&IdleTask_Handler);

    // Create tasks
    OS_CreateTask(TASK_STACK_SIZE, &Task1_Handler);
    OS_CreateTask(TASK_STACK_SIZE, &Task2_Handler);

    // Start scheduler
    OS_StartScheduler(TICK_COUNTS);

    for(;;);
}
