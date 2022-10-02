



/*_____________________________MaIn.c_____________________________________________________
                                      ___           ___           ___
 Author: Abdelrahman Selim           /\  \         /\  \         /\  \
                                    /::\  \       /::\  \       /::\  \
Created on: SEP 20, 2022           /:/\:\  \     /:/\:\  \     /:/\:\  \
                                  /::\ \:\  \   _\:\ \:\  \   /::\ \:\  \
 Version: 01                     /:/\:\ \:\__\ /\ \:\ \:\__\ /:/\:\ \:\__\
                                 \/__\:\/:/  / \:\ \:\ \/__/ \/__\:\/:/  /
                                      \::/  /   \:\ \:\__\        \::/  /
                                      /:/  /     \:\/:/  /        /:/  /
 Brief : Deep Dive in free RTOS      /:/  /       \::/  /        /:/  /
                                     \/__/         \/__/         \/__/
 _________________________________________________________________________________________*/




/* Standard includes. */
#include <stdlib.h>
#include <stdio.h>
#include "string.h"
#include "bit_math.h"	

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "lpc21xx.h"
#include "semphr.h"

/* Peripheral includes. */
#include "serial.h"
#include "GPIO.h"

/**********************************************************************************************************************
 *  Task handlers
 *********************************************************************************************************************/
	TaskHandle_t TSK_1 = NULL;
	TaskHandle_t TSK_2 = NULL;
	TaskHandle_t TSK_3 = NULL;
	TaskHandle_t TSK_4 = NULL;
	TaskHandle_t TSK_5 = NULL;
	TaskHandle_t TSK_6 = NULL;
	TaskHandle_t TSK_7 = NULL;


/**********************************************************************************************************************
 *  queue handlers
 *********************************************************************************************************************/
	QueueHandle_t xQueue ;												

/**********************************************************************************************************************
 *  function declerations
 *********************************************************************************************************************/												
		void configTimer1(void);
		void Task_Init(void);
	signed char state_Buffer[150]; // the buffer that contain the cpu stats.
							


/**********************************************************************************************************************
 *  Tasks functions
 *********************************************************************************************************************/
														
	 
	void Task_1(void * pvParameters ){
		int last_value;  																												// THE VALUE Of the button state in the last excution
/**********************************************************************************************************************
 *  UART COMMAND FOR BUTTON 1
 *********************************************************************************************************************/
		char CMND1[13] = "Btn1+edge\n";
		char CMND2[13] = "Btn1-edge\n";   	 
		
/**********************************************************************************************************************
 *  Task 1 schedular adjustments 
 *********************************************************************************************************************/
		
		TickType_t xLastWakeTime;																								// Last wake tick counter for adjusting the schedular
		const TickType_t xFrequency = 50;  																			// the frequency of the task excution
		vTaskSetApplicationTaskTag( NULL, (TaskHookFunction_t)( void * ) 1 ); 											// setting the task tag to identify it more easily
		xLastWakeTime=xTaskGetTickCount();
		
		

		while(1){ // Task loop
			if (GPIO_read(PORT_0,PIN7)==PIN_IS_HIGH&&last_value ==PIN_IS_LOW){ 		// if the last state was high and now is low
				xQueueSendToBack( xQueue,( void * ) &CMND2,( TickType_t ) 20 );		
}
	  else if (GPIO_read(PORT_0,PIN7)==PIN_IS_LOW&&last_value ==PIN_IS_HIGH){ 		// if the last state was low and now is High
				xQueueSendToBack( xQueue,( void * ) &CMND1,( TickType_t ) 20 );		
		}	
		else{}
			last_value=GPIO_read(PORT_0,PIN7); 																		// saving now as the last pin state
			
			vTaskDelayUntil( &xLastWakeTime, xFrequency ); 												// the schedular delay
		}
	}
	
		void Task_2(void * pvParameters ){
/**********************************************************************************************************************
 *  UART COMMAND FOR BUTTON 2
 *********************************************************************************************************************/
		char CMND1[13] = "Btn2+edge\n";
		char CMND2[13] = "Btn2-edge\n";   	 	
			
/**********************************************************************************************************************
 *  Task 2 schedular adjustments 
 *********************************************************************************************************************/
		int last_value;  																												// THE VALUE Of the button state in the last excution
		TickType_t xLastWakeTime;																								// Last wake tick counter for adjusting the schedular
		const TickType_t xFrequency = 50;  																			// the frequency of the task excution
		vTaskSetApplicationTaskTag( NULL, (TaskHookFunction_t)( void * ) 2 ); 											// setting the task tag to identify it more easily
		xLastWakeTime=xTaskGetTickCount();
		while(1){ // Task loop
			if (GPIO_read(PORT_0,PIN8)==PIN_IS_HIGH&&last_value ==PIN_IS_LOW){ 		// if the last state was high and now is low
				xQueueSendToBack( xQueue,( void * ) &CMND2,( TickType_t ) 20 );		
}
	   else if (GPIO_read(PORT_0,PIN8)==PIN_IS_LOW&&last_value ==PIN_IS_HIGH){ 		// if the last state was low and now is High
				xQueueSendToBack( xQueue,( void * ) &CMND1,( TickType_t ) 20 );		
			}	
			else{}
			last_value=GPIO_read(PORT_0,PIN8); 																		// saving now as the last pin state
			
			vTaskDelayUntil( &xLastWakeTime, xFrequency ); 												// the schedular delay
		}
	}


	void Task_3(void * pvParameters ){
/**********************************************************************************************************************
 *  UART COMMANDs periodic
 *********************************************************************************************************************/
		 signed char  Consumer_sends[9] = "PeriodC\n";				   												
		
		
/**********************************************************************************************************************
 *  Task 3 schedular adjustments 
 *********************************************************************************************************************/
		TickType_t xLastWakeTime;// Last wake tick counter for adjusting the schedular
		const TickType_t xFrequency = 100;// the frequency of the task excution
		vTaskSetApplicationTaskTag( NULL, (TaskHookFunction_t)( void * ) 3 );// setting the task tag to identify it more easily
		xLastWakeTime=xTaskGetTickCount();
		while(1){
			  xQueueSendToBack( xQueue,( void * ) &Consumer_sends,( TickType_t ) 10 );
			vTaskDelayUntil( &xLastWakeTime, xFrequency );
	
		}
	}
	
	
	void Task_4(void * pvParameters ){
		signed char Consumer_recieve[200]; // A String to hold the  variables required to end on the UART
		
/**********************************************************************************************************************
 *  Task 4 schedular adjustments 
 *********************************************************************************************************************/		
		TickType_t xLastWakeTime;															// Last wake tick counter for adjusting the schedular
		const TickType_t xFrequency = 20;											// the frequency of the task excution
		vTaskSetApplicationTaskTag( NULL, (TaskHookFunction_t)( void * ) 4 );			// setting the task tag to identify it more easily
		xLastWakeTime=xTaskGetTickCount();
		while(1){
			if(xQueueReceive( xQueue,&( Consumer_recieve ),( TickType_t ) 0 )==pdPASS){ //If there is values in the queue
					vSerialPutString( Consumer_recieve ,strlen((char *)Consumer_recieve));					 //Send the variables by the Uart
			}
			vTaskDelayUntil( &xLastWakeTime, xFrequency );
		}
	}
	void Task_5(void * pvParameters ){ 
		int dummy1;
/**********************************************************************************************************************
 *  Task 5 schedular adjustments 
 *********************************************************************************************************************/		
		TickType_t xLastWakeTime;
		const TickType_t xFrequency = 10;
		vTaskSetApplicationTaskTag( NULL, (TaskHookFunction_t)( void * )5 );
		xLastWakeTime=xTaskGetTickCount();
		while(1){
			for(dummy1=1;dummy1<33200;dummy1++){	dummy1=dummy1; }//5ms load
			vTaskDelayUntil( &xLastWakeTime, xFrequency );
		}
	}
	void Task_6(void * pvParameters ){
		int dummy1;

		TickType_t xLastWakeTime;
		const TickType_t xFrequency = 100;
		vTaskSetApplicationTaskTag( NULL,(TaskHookFunction_t) ( void * )6 );
		xLastWakeTime=xTaskGetTickCount();
		while(1){
			for( dummy1=1;dummy1<78400;dummy1++){dummy1=dummy1;}//12 ms load

			vTaskDelayUntil( &xLastWakeTime, xFrequency );
		}
	}

	
		void Task_7(void * pvParameters ){ // task 7 is to get the cpu state and send it on the UART
		TickType_t xLastWakeTime; 
		const TickType_t xFrequency = 20;
		vTaskSetApplicationTaskTag( NULL,(TaskHookFunction_t) ( void * ) 7 );
		xLastWakeTime=xTaskGetTickCount();
		while(1){
			strcpy(( char *)state_Buffer, "");
			vTaskGetRunTimeStats(( char *)state_Buffer);
				//vSerialPutString( state_Buffer ,strlen((char *)state_Buffer));
			vTaskDelayUntil( &xLastWakeTime, xFrequency );
		}
	}
	
	
	
	
	void vApplicationTickHook( void ){ // toggle pin 6 every tick
			GPIO_write(PORT_0,PIN6,PIN_IS_HIGH);
			GPIO_write(PORT_0,PIN6,PIN_IS_LOW);
	};



	int main(){
	configTimer1();
	GPIO_init();
	xSerialPortInitMinimal(115200);																 //the uart speed 
	Task_Init();																									 // initialize the list of the tasks
	xQueue = xQueueCreate( 400, (2*sizeof( unsigned long long)) ); // c reating the queue for the uart 
	vTaskStartScheduler();
	while(1){}
	}
	
	/**********************************************************************************************************************
 *  Configurations 
 *********************************************************************************************************************/		
							void Task_Init(){
		// CReating periondic tasks //FUNCTION		TASK_NAME 		STACK SIZE 		PASSED PARAMETER	PRIORITY	HANDLER 		PERIOD
						xTaskPeriodicCreate( Task_1		,"Button_1"			,100,					NULL,							1,				&TSK_1,			50 );//This task will monitor rising and falling edge on button 1
						xTaskPeriodicCreate( Task_2		,"Button_2"			,100,					NULL,							1,				&TSK_2,			50 );//This task will monitor rising and falling edge on button 2 
						xTaskPeriodicCreate( Task_3		,"Periodic"			,100,					NULL,							1,				&TSK_3,			100);//This task will send preiodic string every 100ms to the consumer task
						xTaskPeriodicCreate( Task_4		,"Uart_Rec"			,100,					NULL,							1,				&TSK_4,			20 );//This is the consumer task which will write on UART any received string from other tasks
						xTaskPeriodicCreate( Task_5		,"Load_1"				,100,					NULL,							1,				&TSK_5,			10 );// Task 5: ""Load_1_Simulation"", {Periodicity: 10, Deadline: 10}, Execution time: 5ms
						xTaskPeriodicCreate( Task_6		,"Load_2"				,100,					NULL,							1,				&TSK_6,			100);//Task 6: ""Load_2_Simulation"", {Periodicity: 100, Deadline: 100}, Execution time: 12ms
		 	 //	 xTaskPeriodicCreate( Task_7		,"states"				,100,					NULL,							1,				&TSK_7,			20);
	}	
							
		static void configTimer1(void) // timer confiquration for the cpu get stats
{
	T1PR = 1000;
	T1TCR |= 0x1;
}
