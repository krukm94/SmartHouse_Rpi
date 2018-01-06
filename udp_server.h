
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
#include <sys/types.h>
#include <arpa/inet.h> 


// >>>>>>>> SOCKET SETTINGS
#define PORT 8888

// >>>>>>>> Variables
#define BUFLEN 512

// >>>>>>>>>>>> Functions
int udp_server_init(void);
void* udp_thread_1(void *arg);    

#ifdef __cplusplus
}
#endif

#endif /* UDP_SERVER_H */

