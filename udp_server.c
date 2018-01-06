
/* 
 * File:   udp_server.h
 * Author: mkruk
 *
 * Created on 12 grudnia 2017, 15:55
 */

#include "udp_server.h"
#include "SmartHouse_pin_def.h"

struct sockaddr_in server, client;
int socket_desc, new_socket, c, recv_len;
int slen;

// >>>>>>>>>>>> Thread variables
pthread_t UDP_thread_IDs;   //identyfikator wątku UDP

int udp_server_init(void)
{
    struct timeval recv_timeout;
    recv_timeout.tv_sec = 0;
    recv_timeout.tv_usec = 1;
    
    // >>>>>>>>>>> Create socket
    socket_desc = socket(AF_INET , SOCK_DGRAM , IPPROTO_UDP);
    if (socket_desc == -1) printf("Could not create socket, socket desc: %d" , socket_desc);
    
    // >>>>>>>>>>> Set socket timeout
    setsockopt(socket_desc, SOL_SOCKET, SO_RCVTIMEO, &recv_timeout , sizeof recv_timeout);
    
    // >>>>>>>>>>> Clear server struct
    memset((char *) &server, 0, sizeof(server));
    
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);  //INADDR_ANY -> Określa że program nie musi znać
                                                //adresu IP maszyny na której został uruchmiony
    server.sin_port = htons( PORT );
    
    // >>>>>>>>>>> Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        puts("bind failed");
    }
    puts("bind done");
    
    // >>>>>>>>> Create thread
    pthread_create(&UDP_thread_IDs , NULL , udp_thread_1 , NULL);  
    
   return 0;
}

void* udp_thread_1(void *arg)
{
    while(1)
    {
        puts("Waiting for incoming connections...");
        fflush(stdout);
        
        char* rec_buf;
        rec_buf = malloc(BUFLEN);
        
        if ((recv_len = recvfrom(socket_desc, rec_buf, BUFLEN, 0, (struct sockaddr *) &client, &slen)) == -1)
        {
            printf("\n#No data\n");
        }
        else
        {
            printf("Received packet from %s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
            printf("Data: %s\n" , rec_buf);
            
            if(!strcmp(rec_buf , "LEDON"))
            {
                char* send_buf;
                send_buf = malloc(30);

                sprintf(send_buf , "Zapalono Diode!\n" );

                if(sendto(socket_desc , send_buf , strlen(send_buf) ,0, (struct sockaddr *) &client, slen) ==  -1)
                {
                    printf("\n#ERROR in sendto: file udp_server.c line: 70\n");
                }
                
                free(send_buf);
                
                printf("LEDON\n");
                digitalWrite(LED_GREEN , HIGH);
            }     
            memset(rec_buf , 0 , BUFLEN);
            free(rec_buf);
            memset((char *) &client, 0, sizeof(client));
        }

        
        usleep(3000000);
        digitalWrite(LED_GREEN , LOW);
    }
}