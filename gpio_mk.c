/* 
 * File:   gpio_mk.c
 * Author: Michał Kruk
 *
 * Created on 13 listopada 2017, 21:35
 */
#include "gpio_mk.h"
#include <stdio.h>
#include <stdlib.h>


uint8_t gpio_init(void)
{
    int return_value = 0;
    //initialize wiringPi
    return_value = wiringPiSetup(); 
    
    if(return_value != 0)
    {
        printf("ERROR in: wiringPiSetup = %d\n", return_value);
        return 1;
    }
    
    // INIT LED
    pinMode(LED_RED , OUTPUT);
    pinMode(LED_GREEN , OUTPUT); 
    
    // INIT BUTTON
    pinMode(BUTTON_0 , INPUT);
    pullUpDnControl (BUTTON_0, PUD_UP) ;
    
    // INIT RELAYS
    pinMode(RELAY_CH1 , OUTPUT);
    pinMode(RELAY_CH2 , OUTPUT);
    pinMode(RELAY_CH3 , OUTPUT);
    pinMode(RELAY_CH4 , OUTPUT);
   
    digitalWrite(RELAY_CH1 , LOW);
    digitalWrite(RELAY_CH2 , LOW);
    digitalWrite(RELAY_CH3 , LOW);
    digitalWrite(RELAY_CH4 , LOW);
    
    // Przywitanie
    digitalWrite(RELAY_CH1 , HIGH);
    delay(200);
    digitalWrite(RELAY_CH2 , HIGH);
    delay(200);
    digitalWrite(RELAY_CH3 , HIGH);
    delay(200);
    digitalWrite(RELAY_CH4 , HIGH);
    delay(200);
    digitalWrite(LED_GREEN , HIGH);
    digitalWrite(LED_RED , HIGH);

    delay(300);

    digitalWrite(RELAY_CH1 , LOW);
    delay(50);
    digitalWrite(RELAY_CH2 , LOW);
    delay(50);
    digitalWrite(RELAY_CH3 , LOW);
    delay(50);
    digitalWrite(RELAY_CH4 , LOW);
    digitalWrite(LED_GREEN , LOW);
    digitalWrite(LED_RED , LOW);digitalWrite(LED_GREEN , LOW);
    
    // INIT MOVMENT SENSOR
    pinMode(MOVE_1 , INPUT);
    pullUpDnControl (MOVE_1, PUD_UP) ;
    
    // INIT WETNESS SENSOR
    pinMode(WETNESS , INPUT);
    pullUpDnControl (WETNESS, PUD_UP) ;
    
    //INIT BUTTON INT EN
    button_en_int();
    
    //INIT MOVE INT EN
    move_1_en_int();
    
    //INIT WETNESS INT EN
    wetness_en_int();
    
    printf("--> gpio_init Done\n");
    
    return (EXIT_SUCCESS);
}

uint8_t button_en_int(void)
{
    uint8_t return_value;
    if(return_value = wiringPiISR(BUTTON_0 , INT_EDGE_FALLING , &button_int_func) < 0)
    {
        printf("\nERROR IN: wiringPiISR(BUTTON_0 , INT_EDGE_FALLING , &button_int_func)\n");
        error_Func();
    }
    else printf("\nButton 0 interrupt enable \n");
    
    return return_value;
}

uint8_t move_1_en_int(void)
{
    uint8_t return_value;
    if(return_value = wiringPiISR(MOVE_1 , INT_EDGE_BOTH , &move_1_int_func) < 0)
    {
        printf("\n ERROR IN:  wiringPiISR(MOVE_1 , INT_EDGE_RISING , &move_1_int_func) = %d\n" , return_value);
        error_Func();
    }
    else printf("Move 1 interrupt enable: %d \n" , return_value);
    
    return return_value;
}

uint8_t wetness_en_int(void)
{
    uint8_t return_value;
    if(return_value = wiringPiISR(WETNESS , INT_EDGE_BOTH , &wetness_int_func) < 0)
    {
        printf("\n ERROR IN:  wiringPiISR(WETNESS , INT_EDGE_RISING , &wetness_int_func) = %d\n" , return_value);
        error_Func();
    }
    else printf("Wetness interrupt enable: %d \n" , return_value);
    
    return return_value;
}

void button_int_func(void)
{
    digitalWrite(LED_RED , HIGH);
    delay(50);
    digitalWrite(LED_RED , LOW);
}

void move_1_int_func(void)
{
    digitalWrite(LED_RED , HIGH);
    digitalWrite(RELAY_CH1 , HIGH);
    delay(100);
    digitalWrite(LED_RED , LOW);
    digitalWrite(RELAY_CH1 , LOW); 
}

void wetness_int_func(void)
{
    digitalWrite(LED_RED , HIGH);
    delay(100);
    digitalWrite(LED_RED , LOW);
}


// END OF FILE
