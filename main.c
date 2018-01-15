
/* 
 * File:   main.c
 * Author: mkruk
 *
 * Created on 12 listopada 2017, 22:07
 */


#include "SmartHouse_pin_def.h"

//GLOBAL CONFIG SRUCTURE
volatile struct smarthouse_config smarthouse_struct;

void* main_thread(void *arg); 

int main(void) {
    
    // Variables for main Thread
    pthread_t main_thread_ID; 
    uint8_t thread_create_status;
    
    struct tm *time_mk;
    time_t ti;
    
    time(&ti);
    time_mk = localtime(&ti);
    
    printf("\n\n############## M.KRUK D.GORGON##############\n\n ----------> SmartHouse <---------- \n\n");
    printf("Date: %d.%d.%d Time: %d:%d:%d \n\n" , time_mk->tm_mday , time_mk->tm_mon + 1 , time_mk->tm_year + 1900 , time_mk->tm_hour + 1, time_mk->tm_min , time_mk->tm_sec);
    
    // >>>>>>>>>>> SET Default config 
    default_config();
    
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
    printf("$ main_thread create: %d\n" , thread_create_status);
     
    printf("\n\n ----------> INIT SYSTEM DONE! <------------\n");
    
    // >>>>>>>>>>>>>> WHILE
    while(1)  usleep(5000000);
 
   return (EXIT_SUCCESS);
}

// >>>>>>>>>>>>>>> MAIN THREAD
void* main_thread(void *arg)
{
    while(1) usleep(500000);
}


// END OF FILE

