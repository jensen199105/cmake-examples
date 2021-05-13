/*
 * FreeRTOS Kernel V10.0.0
 * Copyright (C) 2017 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software. If you wish to use our Amazon
 * FreeRTOS name, please do so in a fair use way that does not cause confusion.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

#include <stdio.h>
#include <stdlib.h>
#include "mtk_pdk.h"

/* SIMULATOR */
#include "platform.h"

/* The period after which the check timer will expire provided no errors have
been reported by any of the standard demo tasks.  ms are converted to the
equivalent in ticks using the portTICK_PERIOD_MS constant. */
#define mainCHECK_TIMER_PERIOD_MS			( 3000UL / portTICK_PERIOD_MS )

/* A block time of zero simply means "don't block". */
#define mainDONT_BLOCK						( 0UL )

/*
 * Enable example code for RTOS task.
 */

/*-----------------------------------------------------------*/

/*
 * FreeRTOS hook for when malloc fails, enable in FreeRTOSConfig.
 */
void vApplicationMallocFailedHook( void );

/*
 * FreeRTOS hook for when freertos is idling, enable in FreeRTOSConfig.
 */
void vApplicationIdleHook( void );

/*
 * FreeRTOS hook for when tick plus, enable in FreeRTOSConfig.
 */
void vApplicationTickHook( void );
/*
 * FreeRTOS hook for when a stackoverflow occurs, enable in FreeRTOSConfig.
 */
void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName );

void platform_init(void);

int main(int argc, char *argv[]);

uint32_t ulTotalTaskNum = 0;

void vPDKTaskEndHook(float fValue, bool isLastTask) __attribute__((weak));

void vPDKTaskEnd(float fValue) {
    static volatile uint32_t ulTaskCount = 0;
#ifdef configMultiHarts
    static volatile uint32_t ulHartCount = 0;
#endif /* configMultiHarts */

    taskENTER_CRITICAL();
    ulTaskCount++;
    taskEXIT_CRITICAL();
    vTaskPrioritySet(NULL, tskIDLE_PRIORITY+1);

    if(vPDKTaskEndHook) {
        vPDKTaskEndHook(fValue, ulTaskCount>=ulTotalTaskNum);
    }
    if (ulTaskCount >= ulTotalTaskNum) {
#ifdef MRV_PROFILING
        profile_off();
        profile_end();
#endif /* MRV_PROFILING */
#if SIMULATOR
#ifdef configMultiHarts
        taskENTER_CRITICAL();
        ulHartCount++;
        taskEXIT_CRITICAL();
        if (ulHartCount >= configMultiHarts)
#endif /* configMultiHarts */
        if(!vPDKTaskEndHook) exit(1);
#endif /* SIMULATOR */
    }
    vTaskSuspend(NULL);
}

#ifndef MULTITASK
void vUserTask(void *pvParameters) {
    main(0, 0);
}
#endif // #ifndef MULTITASK

void vPDKTaskCreationHook(void) __attribute__((weak));
int PDKTaskCreation(void);
int PDKTaskCreation(void) {
    int ret = 0;
#ifndef MULTITASK
    /* uxTaskNumber = 0 */
    xTaskCreate(vUserTask, "User", 32768, NULL, 3, NULL);
#else
    ret = main(0, 0);
#endif // #ifndef MULTITASK
    ulTotalTaskNum = (uint32_t) uxTaskGetNumberOfTasks();

    if(vPDKTaskCreationHook) {
        vPDKTaskCreationHook();
    }

    return ret;
}

int task_main( void );
int task_main( void )
{
    //TaskHandle_t pxCreatedTask;
#ifdef configMultiHarts
    int uxCpuId = uxPortGetCpuId();
#endif /* configMultiHarts */

    platform_init();
    /* Output a message on Hyperterminal using mrv_printf function */
#ifdef configMultiHarts
    mrv_printf("Hart%d: Build time: %s %s\n\r", uxCpuId, __DATE__, __TIME__);
#else
    mrv_printf("Build time: %s %s\n\r", __DATE__, __TIME__);
#endif /* configMultiHarts */

    PDKTaskCreation();

    /* Check if tasks are setup correctly before execution. */
    if (ulTotalTaskNum == 0) {
        printf("failed: No tasks to run!\n\r");
        return -1;
    }

    /* Start the kernel.  From here on, only tasks and interrupts will run. */
    vTaskStartScheduler();

    /* Will not get here unless a task calls vTaskEndScheduler(). */

    /* Exit FreeRTOS */
    return 0;
}

/*-----------------------------------------------------------*/
void vApplicationTickHook( void )
{
}

/*-----------------------------------------------------------*/

void vApplicationMallocFailedHook( void )
{
	/* vApplicationMallocFailedHook() will only be called if
	configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
	function that will get called if a call to pvPortMalloc() fails.
	pvPortMalloc() is called internally by the kernel whenever a task, queue,
	timer or semaphore is created.  It is also called by various parts of the
	demo application.  If heap_1.c or heap_2.c are used, then the size of the
	heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
	FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
	to query the size of free heap space that remains (although it does not
	provide information on how the remaining heap might be fragmented). */
	printf("malloc fail\n");
	taskDISABLE_INTERRUPTS();
	for( ;; );
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook( void )
{
	/* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
	to 1 in FreeRTOSConfig.h.  It will be called on each iteration of the idle
	task.  It is essential that code added to this hook function never attempts
	to block in any way (for example, call xQueueReceive() with a block time
	specified, or call vTaskDelay()).  If the application makes use of the
	vTaskDelete() API function (as this demo application does) then it is also
	important that vApplicationIdleHook() is permitted to return to its calling
	function, because it is the responsibility of the idle task to clean up
	memory allocated by the kernel to any task that has since been deleted. */
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
	( void ) pcTaskName;
	( void ) pxTask;

	/* Run time stack overflow checking is performed if
	configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	function is called if a stack overflow is detected. */
	printf("stack overflow\n");
	taskDISABLE_INTERRUPTS();
	for( ;; );
}
/*-----------------------------------------------------------*/
