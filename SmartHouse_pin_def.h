
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
    
#include "temperature.h"
#include "gpio_mk.h"
#include "udp_server.h"
#include "light.h"


    void error_Func(void);
    
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



#ifdef __cplusplus9+
}
#endif

#endif /* SMARTHOUSE_PIN_DEF_H */

