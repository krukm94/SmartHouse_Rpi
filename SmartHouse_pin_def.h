
/* 
 * File:   SmartHouse_pin_def.h
 * Author: mkruk
 *
 * Created on 14 listopada 2017, 17:08
 */

#ifndef SMARTHOUSE_PIN_DEF_H
#define SMARTHOUSE_PIN_DEF_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include <stdint.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <wiringPi.h>
#include <pthread.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <wchar.h>
    
#include "temperature.h"
#include "gpio_mk.h"
#include "udp_server.h"
#include "light.h"
#include "report.h"
    
// ->>>>>>>> CONFIG VARIABLES 
#define PRINT_UDP_DATA_ON_TERMINAL          0
#define UDP_THREAD_SLEEP_uS                 1000
    
#define PRINT_FRAME_DATA                    1
    
#define PRINT_TEMP_ON_TERMINAL              0
#define TMEP_THREAD_SLEEP_uS                3000000    
    
#define PRINT_LUX_ON_TERMINAL               0
#define LIGHT_THREAD_SLEEP_uS               3000000
    
#define PRINT_ACTION_CHANGES_ON_TERMINAL    0
    

    
// ->>>>>>>> CONFIG STRUCTURE
struct smarthouse_config{
    
    double temp_1_value;                        // TEMP 1 VALUE
    double temp_2_value;                        // TEMP 2 VALUE
    double temp_1_threshold_high;               // TEMP 1 THRESHOLD HIGH 
    double temp_1_threshold_low;                // TEMP 1 THRESHOLD LOW 
    double temp_2_threshold_high;               // TEMP 2 THRESHOLD HIGH 
    double temp_2_threshold_low;                // TEMP 2 THRESHOLD LOW 
    uint8_t temp_1_threshold_high_action[5];      // TEMP 1 TS. HIGH ACTION
    uint8_t temp_1_threshold_low_action[5];       // TEMP 2 TS. LOW ACTION
    uint8_t temp_2_threshold_high_action[5];      // TEMP 1 TS. HIGH ACTION
    uint8_t temp_2_threshold_low_action[5];       // TEMP 2 TS. LOW ACTION
    uint8_t temp_1_activate;
    uint8_t temp_2_activate;
    
    //LIGHT SENSOR VARIABLES
    uint32_t light_value;                       // LIGHT SENSOR VALUE
    uint32_t light_threshold_high;             // LIGHT THRESHOLD HIGH
    uint32_t light_threshold_low;              // LIGHT THRESHOLD LOW
    uint8_t light_threshold_high_action[5];       // LIGHT TS. HIGH ACTION
    uint8_t light_threshold_low_action[5];        // LIGHT TS. LOW ACTION
    uint8_t light_sensor_activate;
    
    //MOTOIN SENSOR VARIABLES
    uint8_t motion_threshold_action[5];           // MOTION THRESHOLD ACTION
    uint8_t motion_sensor_activate;               // FLAG FOR ACTIVATE SENSOR
    char last_move[25];                           //STRING CONTAINS LAST MOVE DATE AND TIME
    
    //BUTTON VARIABLES
    uint8_t button_action[5];                      // BUTTON ACTION
    
    //WETNES SENSOR VARIABLES
    uint8_t wetness_threshold_action[5];          // WETNESS THRESHOLD ACTION
    uint8_t wetness_sensor_activate;             //FLAG FOR ACTIVATE SENSOR
};   
    
// ACTIONS DEFINITIONS
enum actions{
    // RELAY ACTIONS
    RELAY_1_ON =    0x50,
    RELAY_1_OFF =   0x51,
    
    RELAY_2_ON =    0x52,
    RELAY_2_OFF =   0x53,
    
    RELAY_3_ON =    0x54,
    RELAY_3_OFF =   0x55,
    
    RELAY_4_ON =    0x56,
    RELAY_4_OFF =   0x57,
    
    LED_1_ON =      0x58,
    LED_1_OFF =     0x59,
    
    LED_2_ON =      0x5A,
    LED_2_OFF =     0x5B 
};

#define ACTION1 ((uint8_t) 0x5C)
#define ACTION2 ((uint8_t) 0x5D)
#define ACTION3 ((uint8_t) 0x5E)
#define ACTION4 ((uint8_t) 0x5F)
#define ACTION5 ((uint8_t) 0x60)
  
#define LED_RED     2   // 2 is GPIO2 (Rv1:21 - Rv2:27)
#define LED_GREEN   3   // 3 is GPIO3 (BCM 22)
#define BUTTON_0    0   // 0 is GPIO0 (BCM 17)    
    
// RELAYS
#define RELAY_CH1     21      // 21 is GPIO21 (BCM5)        
#define RELAY_CH2      4       // 4  is GPIO4 (BCM23)
#define RELAY_CH3      5       // 5  is GPIO5 (BCM24)
#define RELAY_CH4      6       // 6  is GPIO6 (BCM25)  
    
//MOVeMAN0T
#define MOVE_1  22      // 31 is GPIO22 (BCM6)
    
//WETNESS SENSOR
#define WETNESS 25     // 37 is GPIO 25 (BCM26)

    void error_Func(char *error);
    int8_t default_config();
    void save_config();
    void read_config();
    
    void action(uint8_t action);

#ifdef __cplusplus
}
#endif

#endif /* SMARTHOUSE_PIN_DEF_H */

