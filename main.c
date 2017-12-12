
/* 
 * File:   main.c
 * Author: mkruk
 *
 * Created on 12 listopada 2017, 22:07
 */

#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include "SmartHouse_pin_def.h"

#include "temperature.h"
//#include "gpio_mk.h"

#include "udp_server.h"
/*
 * 
 */
 

int main(void) {
    
    printf("\n\r\n\r############## M.KRUK ##############\n\r \n\r ----------> SmartHouse <---------- \n\r\n\r");
    // >>>>>>>>>>> Initialize GPIO
    //gpio_init();  
    
    // >>>>>>>>>>> Init Temp sensor
    //temp_init_mk();
    
    //temp_read_mk();
    udp_server_init();
    
    while(1)
    {   

    }
    
   return (EXIT_SUCCESS);
}


