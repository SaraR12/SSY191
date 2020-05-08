#include <stdio.h>
#include <stdlib.h>

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/* 
	Create a Semaphore handle
	The 3 tokens in p_0 is representes by task_a_Handle, b and c as below
*/
xTaskHandle task_a_Handle; 
xTaskHandle task_b_Handle;
xTaskHandle task_c_Handle; // Created a new task_c_Handle for r_C

/* 
	Additional resources r_A, r_B and r_C
*/
SemaphoreHandle_t resource_a;
SemaphoreHandle_t resource_b;
SemaphoreHandle_t resource_c; // Added resource_c

/*
	xSemaphoreTake = take a token from r_A, r_B or r_C
	xSemaphoreGive = give a token back to r_A, r_B or r_C
*/

void the_task(void *pvParameters)
{  /* The order we do our tasks */
	while (1)
	{ 
		xSemaphoreTake(resource_a, portMAX_DELAY); // Take a token from r_A 
		xSemaphoreTake(resource_c, portMAX_DELAY); // Take a token from r_C
		/* Now we have entered p1 */
		...
		xSemaphoreTake(resource_b, portMAX_DELAY); // Take a token from r_B 
		xSemaphoreGive(resource_c, portMAX_DELAY); // Give a token to r_C 
		/* Now we have entered p2 */
		...
		if (...)
		{
			...
			xSemaphoreTake(resource_c, portMAX_DELAY); // Take a token from r_C
			xSemaphoreGive(resource_a, portMAX_DELAY); // Give a token to r_A 
			/* Now we have entered p4 */
			...
			xSemaphoreTake(resource_a, portMAX_DELAY); // Take a token from r_A 
			xSemaphoreGive(resource_c, portMAX_DELAY); // Give a token to r_C 
			/* Now we have entered p5 */
			...
		}
		xSemaphoreGive(resource_b);					// Give a token to r_B 
		/* Now we have entered p6 */
		...
		xSemaphoreGive(resource_a);					// Give a token to r_A 
		...
	}
}

int main(int argc, char **argv) 
{
	 /* Create our Semaphores */
    resource_a = xSemaphoreCreateMutex();
    resource_b = xSemaphoreCreateMutex();
	  resource_c = xSemaphoreCreateMutex();
    xTaskCreate(the_task, "Task 1", configMINIMAL_STACK_SIZE, NULL, 1, &task_a_Handle);
    xTaskCreate(the_task, "Task 2", configMINIMAL_STACK_SIZE, NULL, 1, &task_b_handle);
    xTaskCreate(the_task, "Task 3", configMINIMAL_STACK_SIZE, NULL, 1, &task_c_handle);

    vTaskStartScheduler();
    for( ;; );
}
