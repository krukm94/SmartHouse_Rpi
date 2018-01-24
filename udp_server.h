
/* 
 * File:   udp_server.h
 * Author: mkruk
 *
 * Created on 12 grudnia 2017, 15:55
 */

#ifndef UDP_SERVER_H
#define UDP_SERVER_H

#ifdef __cplusplus
extern "C" {
#endif
    


// >>>>>>>>> INCLUDES
#include "SmartHouse_pin_def.h"
    
#include <sys/socket.h>
#include <arpa/inet.h> 


// >>>>>>>> SOCKET SETTINGS
#define PORT 8888

// >>>>>>>> Variables
#define BUFLEN 512

// >>>>>>>> Frame variables
#define PARAM_TYPE_STRING   ((uint8_t)0x53)
#define PARAM_TYPE_INT      ((uint8_t)0x49)
#define PARAM_TYPE_FLOAT    ((uint8_t)0x46)
    
#define READ                ((uint8_t)0x52) //READ BYTE VALUE (FRAME)
#define WRITE               ((uint8_t)0x57) //WRITE BYTE VALUE (FRAME)
    
//>>>>>>>>>>> COMANDS
#define CONNECT             ((uint8_t)0x2A) //Command check connection

#define TEMP_1_ACTIVE       ((uint8_t)0x2B) //Command can R/W temp_1_active (ON OFF TEMP1 value)
#define TEMP_1_VALUE        ((uint8_t)0x2C) 
#define TEMP_1_TS_HIGH      ((uint8_t)0x2D) //Threshold HIGH for temp 1
#define TEMP_1_TS_HIGH_ACT  ((uint8_t)0x2E) //Actions from threshold high for temp 1
#define TEMP_1_TS_LOW       ((uint8_t)0x2F) //Threshold LOW for temp 1
#define TEMP_1_TS_LOW_ACT   ((uint8_t)0x30) //Actions from threshold low for temp 1

#define TEMP_2_ACTIVE       ((uint8_t)0x31)
#define TEMP_2_VALUE        ((uint8_t)0x32)
#define TEMP_2_TS_HIGH      ((uint8_t)0x33)
#define TEMP_2_TS_HIGH_ACT  ((uint8_t)0x34) 
#define TEMP_2_TS_LOW       ((uint8_t)0x35)
#define TEMP_2_TS_LOW_ACT   ((uint8_t)0x36)

#define LUX_ACTIVE          ((uint8_t)0x37) //Command can R/W lux_active (ON OFF Lux Sensor)
#define LUX_VALUE           ((uint8_t)0x38)
#define LUX_TS_HIGH         ((uint8_t)0x39)
#define LUX_TS_HIGH_ACT     ((uint8_t)0x3A) //Lux threshold high actions
#define LUX_TS_LOW          ((uint8_t)0x3B) 
#define LUX_TS_LOW_ACT      ((uint8_t)0x3C) //Lux threshold low actions

#define MOTION_ACTIVE       ((uint8_t)0x3D) //Active motion sensor
#define MOTION_ACT          ((uint8_t)0x3E) //Motion sensor actions 
#define MOTION_LAST_DET     ((uint8_t)0x3F) //Motion sensor last move
    
#define BUTTON_ACT          ((uint8_t)0x40) //Buton Actions
    
#define WETNESS_ACTIVE      ((uint8_t)0x41) //Active Wetness sensor
#define WETNESS_ACT         ((uint8_t)0x42) //Wetness sensor actions
    
// >>>>>>>>>>>> Functions
int8_t udp_server_init(void);
void* udp_thread_1(void *arg);    

int8_t compare_word(char* read_word , uint8_t* read_word_len , char* read_param , char* word , char* buffer);

int8_t send_frame(uint8_t  command , char* param , uint16_t size_of_param , uint8_t param_type , uint8_t read_or_write);
int8_t decode_frame(char *rcv_frame , uint8_t* command ,uint8_t* rw , uint8_t* type , uint16_t *size , char *payload);

#ifdef __cplusplus
}
#endif

#endif /* UDP_SERVER_H */

