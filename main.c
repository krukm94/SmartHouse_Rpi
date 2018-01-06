
/* 
 * File:   main.c
 * Author: mkruk
 *
 * Created on 12 listopada 2017, 22:07
 */


#include "SmartHouse_pin_def.h"


/*
 * 
 */

void* main_thread(void *arg); 

int main(void) {
    
    // Variables for main Thread
    pthread_t main_thread_ID; 
    uint8_t thread_create_status;
    
    printf("\n\r\n\r############## M.KRUK ##############\n\r \n\r ----------> SmartHouse <---------- \n\r\n\r\n");
    
    // >>>>>>>>>>> Initialize GPIO
    gpio_init();  
    
    // >>>>>>>>>>> Init Temp sensor
    temp_init_mk();
    
    // >>>>>>>>>>> Init UDP server
     udp_server_init();
     
     // >>>>>>>>>>> Init Lux sensor
     init_TSL2561();
    
    // >>>>>>>>> Create main thread
    thread_create_status = pthread_create(&main_thread_ID , NULL , main_thread , NULL);  
    printf("main_thread create: %d\n" , thread_create_status);
     
    // >>>>>>>>>>>>>> WHILE
    while(1)  usleep(5000000);
 
   return (EXIT_SUCCESS);
}

// >>>>>>>>>>>>>>> MAIN THREAD
void* main_thread(void *arg)
{
    //Variable for move detection
    uint8_t move;
    
    while(1)
    {
      move = digitalRead(MOVE_1);
      digitalWrite(LED_GREEN , LOW);
    
        if (move == 1)
        {
            printf("Wykryto ruch: %d \n" , move);
            digitalWrite(LED_GREEN , HIGH);     
        }
        usleep(500000);
    }
}

void error_Func(void)
{    
    printf("\n\n ERROR!!!\n");
    while(1){
        digitalWrite(LED_RED , HIGH);
        delay(100);
        digitalWrite(LED_RED , LOW);
        delay(100);
    }
}

// END OF FILE

