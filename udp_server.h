
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

// >>>>>>>>>>>> Functions
int8_t udp_server_init(void);
void* udp_thread_1(void *arg);    

int8_t compare_word(char * read_word , uint8_t *read_word_len , char *read_param , char *word , char *buffer);

#ifdef __cplusplus
}
#endif

#endif /* UDP_SERVER_H */

