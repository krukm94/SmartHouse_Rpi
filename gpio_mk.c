/* 
 * File:   gpio_mk.c
 * Author: Michał Kruk
 *
 * Created on 13 listopada 2017, 21:35
 */
#include <wiringPi.h>

#include "gpio_mk.h"


//GLOBAL CONFIG SRUCTURE
extern volatile struct smarthouse_config smarthouse_struct;

/* 
 * Func:   gpio_init
 * Description: Init GPIO (Button, diodes, relays, and others)
 */
int8_t gpio_init(void)
{
    int8_t return_value = 0;
    //initialize wiringPi
    return_value = wiringPiSetup(); 
    
    printf("\n --> Init GPIO <--\n");
    
    if(return_value != 0)
    {
        printf("ERROR in: wiringPiSetup = %d\n", return_value);
        return 1;
    }
    printf("$ wiringPiSetup ok = %d\n", return_value);
    
    
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
//    digitalWrite(RELAY_CH1 , HIGH);
//    delay(200);
//    digitalWrite(RELAY_CH2 , HIGH);
//    delay(200);
//    digitalWrite(RELAY_CH3 , HIGH);
//    delay(200);
//    digitalWrite(RELAY_CH4 , HIGH);
//    delay(200);
//    digitalWrite(LED_GREEN , HIGH);
//    digitalWrite(LED_RED , HIGH);
//
//    delay(300);
//
//    digitalWrite(RELAY_CH1 , LOW);
//    delay(50);
//    digitalWrite(RELAY_CH2 , LOW);
//    delay(50);
//    digitalWrite(RELAY_CH3 , LOW);
//    delay(50);
//    digitalWrite(RELAY_CH4 , LOW);
//    digitalWrite(LED_GREEN , LOW);
//    digitalWrite(LED_RED , LOW);digitalWrite(LED_GREEN , LOW);
    
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
    
    printf("\n --> Init GPIO DONE! <--\n");
    
    return (EXIT_SUCCESS);
}

/* 
 * Func:   button_en_int
 * Description: Eneble button interrupt
 */
int8_t button_en_int(void)
{
    int8_t return_value;
    if(return_value = wiringPiISR(BUTTON_0 , INT_EDGE_RISING , &button_int_func) < 0)
    {
        printf("\nERROR IN: wiringPiISR(BUTTON_0 , INT_EDGE_FALLING , &button_int_func)\n");
        error_Func();
    }
    else printf("$ Button 0 interrupt enable \n");
    
    return return_value;
}

/* 
 * Func:   move_1_en_int
 * Description: Enable interrupt from motion sensor
 */
int8_t move_1_en_int(void)
{
    int8_t return_value;
    if(return_value = wiringPiISR(MOVE_1 , INT_EDGE_RISING , &move_1_int_func) < 0)
    {
        printf("\n ERROR IN:  wiringPiISR(MOVE_1 , INT_EDGE_RISING , &move_1_int_func) = %d\n" , return_value);
        error_Func();
    }
    else printf("$ Move 1 interrupt enable: %d \n" , return_value);
    
    return return_value;
}

/* 
 * Func:   wetness_en_int
 * Description: Enable interrupt from wetness sensor
 */
int8_t wetness_en_int(void)
{
    int8_t return_value;
    if(return_value = wiringPiISR(WETNESS , INT_EDGE_FALLING , &wetness_int_func) < 0)
    {
        printf("\n ERROR IN:  wiringPiISR(WETNESS , INT_EDGE_RISING , &wetness_int_func) = %d\n" , return_value);
        error_Func();
    }
    else printf("$ Wetness interrupt enable: %d \n" , return_value);
    
    return return_value;
}

/* 
 * Func:   button_int_func
 * Description: Callback for button interrupt
 */
void button_int_func(void)
{
    delay(1000); // Delay for deboncing
    
    if(digitalRead(BUTTON_0) == 0)
    {
        smarthouse_struct.motion_sensor_activate ^= 1;
        
        if(smarthouse_struct.motion_sensor_activate == 1) 
        {
            digitalWrite(LED_RED , HIGH);
            delay(200);
            digitalWrite(LED_RED , LOW);    
            
            save_report("Motion Sensor Activatied");
            save_config();
        }
        else if(smarthouse_struct.motion_sensor_activate == 0)
        {
            digitalWrite(LED_GREEN , HIGH);
            delay(200);
            digitalWrite(LED_GREEN , LOW);
            
            save_report("Motion Sensor Deactivatied");
            save_config();
        }
    }
}

/* 
 * Func:   button_int_func
 * Description: Callback for motion sensor interrupt
 */
void move_1_int_func(void)
{
    if(smarthouse_struct.motion_sensor_activate == 1) save_report("Movement detected! (Sensor 1)");
}


/* 
 * Func:   wetness_int_func
 * Description: Callback for wetness sensor interrupt
 */
void wetness_int_func(void)
{
    if(smarthouse_struct.wetness_sensor_activate == 1)
    {
        if(digitalRead(WETNESS) == 0) save_report("Wetness is to low!");
    }
}


// END OF FILE
