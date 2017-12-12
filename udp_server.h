
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
    
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h> 
#include <pthread.h>

// >>>>>>>> SOCKET SETTINGS
#define PORT 8888

// >>>>>>>> Variables
#define BUFLEN 512
 
struct sockaddr_in server, client;
int socket_desc, new_socket, c, recv_len;
int slen;

// >>>>>>>>>>>> Thread variables
pthread_t UDP_thread_IDs;   //identyfikator wątku UDP

// >>>>>>>>>>>> Functions
int udp_server_init(void);
void* udp_thread_1(void *arg);    

#ifdef __cplusplus
}
#endif

#endif /* UDP_SERVER_H */

