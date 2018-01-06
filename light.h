
/* 
 * File:   light.h
 * Author: mkruk
 *
 * Created on 12 grudnia 2017, 19:38
 */

#ifndef LIGHT_H
#define LIGHT_H

#ifdef __cplusplus
extern "C" {
#endif 
    
// >>>>>>>>>> INCLUDES   
#include "SmartHouse_pin_def.h"
    
#include "TSL2561.h"

// >>>>>>>>>> FUNCTIONS
uint32_t init_TSL2561(void);
uint32_t read_lux(void);

//Thread Function
void* lux_thread(void *arg); 

#ifdef __cplusplus
}
#endif

#endif /* LIGHT_H */

