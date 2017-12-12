/* 
 * File:   gpio_mk.c
 * Author: Michał Kruk
 *
 * Created on 13 listopada 2017, 21:35
 */
#include "gpio_mk.h"
#include <stdio.h>
#include <stdlib.h>


int gpio_init(void)
{
    int return_value = 0;
    //initialize wiringPi
    return_value = wiringPiSetup(); 
    
    if(return_value != 0)
    {
        printf("\n\rERROR in: wiringPiSetup = %d", return_value);
        return 1;
    }
    
    // INIT LED
    pinMode(LED_RED , OUTPUT);
    pinMode(LED_GREEN , OUTPUT); 
    
    // INIT BUTTON
    pinMode(BUTTON_0 , INPUT);
    pullUpDnControl (BUTTON_0, PUD_UP) ;
    
    // INIT RELAYS
//    pinMode(CH1 , OUTPUT);
//    pinMode(CH2 , OUTPUT);
//    pinMode(CH3 , OUTPUT);
//    pinMode(CH4 , OUTPUT);
//   
//    digitalWrite(CH1 , LOW);
//    digitalWrite(CH2 , LOW);
//    digitalWrite(CH3 , LOW);
//    digitalWrite(CH4 , LOW);
    
    printf("\ngpio_init Done");
    
    //INIT BUTTON INT
    button_en_int();

    
    
    return (EXIT_SUCCESS);
}

int button_en_int(void)
{
    if(wiringPiISR(BUTTON_0 , INT_EDGE_FALLING , &button_int_func) < 0)
    {
        printf("Unable to setup ISR");
        return 1;
    }
    else printf("\nButton 0 interrupt enable");
}

void button_int_func(void)
{
    digitalWrite(LED_RED , HIGH);
    delay(50);
    digitalWrite(LED_RED , LOW);
}

// END OF FILE
