
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
    
#define LED_RED     2   // 2 is GPIO2 (Rv1:21 - Rv2:27)
#define LED_GREEN   3   // 3 is GPIO3 (BCM 22)
#define BUTTON_0    0   // 0 is GPIO0 (BCM 17)    
    
// RELAYS
#define CH1     21      // 21 is GPIO21 (BCM5)        
#define CH2     4       // 4  is GPIO4 (BCM23)
#define CH3     5       // 5  is GPIO5 (BCM24)
#define CH4     6       // 6  is GPIO6 (BCM25)  


#ifdef __cplusplus
}
#endif

#endif /* SMARTHOUSE_PIN_DEF_H */

