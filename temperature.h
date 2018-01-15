
/* 
 * File:   temperature.h
 * Author: mkruk
 *
 * Created on 16 listopada 2017, 17:49
 */

#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#ifdef __cplusplus
extern "C" {
#endif


   
    #include "SmartHouse_pin_def.h"
 
    //Functions
    int8_t temp_init_mk(void);
    int temp_read_mk(void);
    
    //Thread Function
    void* temp_thread(void *arg); 
    
#ifdef __cplusplus
}
#endif

#endif /* TEMPERATURE_H */

