/*
 * FreeRTOS Kernel V11.1.0
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */

/*
 * This is a simple main that will start the FreeRTOS-Kernel and run a periodic task
 * that only delays if compiled with the template port, this project will do nothing.
 * For more information on getting started please look here:
 * https://freertos.org/FreeRTOS-quick-start-guide.html
 */

/* FreeRTOS includes. */
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <timers.h>
#include <semphr.h>

/* Standard includes. */
#include <stdio.h>

#define KEY_PLUS  'p'  /* 加 1 */
#define KEY_MINUS 'm'   /* 减 1 */
#define VALUE_MAX 100u  /* 上限 */

static QueueHandle_t xKeyQueue;   /* 键盘 -> 数值任务 */
static QueueHandle_t xValueQueue; /* 数值任务 -> 打印任务 */

uint8_t ucHeap[ configTOTAL_HEAP_SIZE ];
/*-----------------------------------------------------------*/

static void vKeyTask( void * parameters );

/*-----------------------------------------------------------*/

static void vKeyTask( void * parameters )
{
    /* Unused parameters. */
    ( void ) parameters;
    BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */
    printf("Key Task started.\n");
    char c;
    for(;;)
    {
        c = (char)getchar(); /* 获取键盘输入 */
        if( c == KEY_PLUS || c == KEY_MINUS )
        {
            printf("Key pressed: %c\n", c); /* 打印按键 */
            xQueueSend( xKeyQueue, &c, portMAX_DELAY ); /* 发送到键盘队列 */
        }
    }
}

static void vCounterTask(void *parameters)
{
    (void)parameters;
    char c;
    BaseType_t xReturn = pdPASS;
    printf("Counter Task started.\n");
    for(;;)
    {
        xReturn = xQueueReceive(xKeyQueue, &c, portMAX_DELAY); /* 阻塞等待 */ 
        if( xReturn== pdPASS )
        {
            printf("Key received: %c\n", c); /* 打印按键 */
            static uint32_t ulValue = 0; /* 初始值为0 */
            if( c == KEY_PLUS )
            {
                if( ulValue < VALUE_MAX )
                {
                    ulValue++;
                }
            }
            else if( c == KEY_MINUS )
            {
                if( ulValue > 0 )
                {
                    ulValue--;
                }
            }
            xQueueSend( xValueQueue, &ulValue, portMAX_DELAY ); /* 发送到数值队列 */
        }
    }
}

static void vPrintTask(void *parameters)
{
    (void)parameters;
    uint32_t ulValue;
    printf("Print Task started.\n");
    for(;;)
    {
        if( xQueueReceive( xValueQueue, &ulValue, portMAX_DELAY ) == pdPASS )
        {
            printf("Current Value: %u\n", ulValue); /* 打印当前值 */
        }
    }
}
/*-----------------------------------------------------------*/

int main( void )
{
    BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */
    xKeyQueue = xQueueCreate( 10, sizeof( char ) ); /* 创建键盘队列 */
    xValueQueue = xQueueCreate(10,sizeof(uint32_t));

    xTaskCreate( vKeyTask, "Key Task", 128, NULL, 1, NULL );
    xTaskCreate( vCounterTask, "Counter Task", 128, NULL, 1, NULL );
    xReturn =  xTaskCreate( vPrintTask, "Print Task", 128, NULL, 1 , NULL );
    if (xReturn == pdPASS) {
        printf("Key Task created successfully!\n");
    } else {
        printf("Key Task creation failed! Error code: %d\n", xReturn);
    }
    vTaskStartScheduler();   /* 启动任务，开启调度 */

    while(1);
    return 0;
}
/*-----------------------------------------------------------*/

#if ( configCHECK_FOR_STACK_OVERFLOW > 0 )

    void vApplicationStackOverflowHook( TaskHandle_t xTask,
                                        char * pcTaskName )
    {
        /* Check pcTaskName for the name of the offending task,
         * or pxCurrentTCB if pcTaskName has itself been corrupted. */
        ( void ) xTask;
        ( void ) pcTaskName;
    }

#endif /* #if ( configCHECK_FOR_STACK_OVERFLOW > 0 ) */
/*-----------------------------------------------------------*/
