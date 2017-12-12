

/* 
 * File:   gpio_mk.h
 * Author: Michał Kruk
 *
 * Created on 13 listopada 2017, 21:35
 */

#ifndef GPIO_MK_H
#define GPIO_MK_H

#ifdef __cplusplus
extern "C" {
#endif
    
// INCLUDES
#include <wiringPi.h>
#include "SmartHouse_pin_def.h"  


// FUNCTIONS
    int gpio_init(void);
    int button_en_int(void);
    void button_int_func(void);
   

#ifdef __cplusplus
}
#endif

#endif /* GPIO_MK_H */

