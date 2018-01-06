

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
#include "SmartHouse_pin_def.h"  


// FUNCTIONS
    uint8_t gpio_init(void);
    uint8_t button_en_int(void);
    uint8_t move_1_en_int(void);
    uint8_t wetness_en_int(void);
    
    void button_int_func(void);
    void move_1_int_func(void);
    void wetness_int_func(void);

#ifdef __cplusplus
}
#endif

#endif /* GPIO_MK_H */

