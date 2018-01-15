
/* 
 * File:   udp_server.h
 * Author: mkruk
 *
 * Created on 12 grudnia 2017, 15:55
 */

#include "udp_server.h"


extern volatile struct smarthouse_config smarthouse_struct;

struct sockaddr_in server, client;
int socket_desc, new_socket, c, recv_len;
int slen;

// >>>>>>>>>>>> Thread variables
pthread_t UDP_thread_IDs;   //identyfikator wątku UDP

int8_t udp_server_init(void)
{
    int8_t return_value;
    struct timeval recv_timeout;
    recv_timeout.tv_sec = 0;
    recv_timeout.tv_usec = 1;
    
    printf("\n --> Init UDP server <--\n");
    
    // >>>>>>>>>>> Create socket
    socket_desc = socket(AF_INET , SOCK_DGRAM , IPPROTO_UDP);
    if (socket_desc == -1) printf("$ Could not create socket, socket desc: %d\n" , socket_desc);
    
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
    if(return_value = bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) != 0)
    {
        printf("$ bind failed, bind() = %d\n" , return_value);
    }
    else printf("$ bind done, bind() = %d\n" , return_value);
    
    // >>>>>>>>> Create thread
    pthread_create(&UDP_thread_IDs , NULL , udp_thread_1 , NULL);  
    
    
    printf("\n --> Init UDP server DONE! <--\n");
    
   return 0;
}

void* udp_thread_1(void *arg)
{
    uint8_t cnt = 0;
    uint8_t sendto_return = 0;
    
    while(1)
    {
        
#if PRINT_UDP_DATA_ON_TERMINAL == 1
        puts("%Waiting for incoming connections...\n");
#endif
        fflush(stdout);
        
        char* rec_buf;
        rec_buf = malloc(BUFLEN);
        
        if ((recv_len = recvfrom(socket_desc, rec_buf, BUFLEN, 0, (struct sockaddr *) &client, &slen)) == -1)
        {
#if PRINT_UDP_DATA_ON_TERMINAL == 1
            printf("% No data\n");
#endif
        }
        else
        {
#if PRINT_UDP_DATA_ON_TERMINAL == 1
            printf("% Received packet from %s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
            printf("$ Data: %s\n" , rec_buf);
#endif
            
            if(strcmp(rec_buf , "LEDON") == 0)
            {
                char* send_buf;
                send_buf = malloc(30);

                sprintf(send_buf , "$ Zapalono Diode!\n" );
                
                    if(sendto_return = sendto(socket_desc , send_buf , strlen(send_buf) ,0, (struct sockaddr *) &client, slen) ==  -1)
                    {
                        printf("\n ERROR in sendto = %d:\n" , sendto_return);
                    } 
                
                free(send_buf);
#if PRINT_UDP_DATA_ON_TERMINAL == 1
                printf("% LEDON\n");
#endif 
                digitalWrite(LED_GREEN , HIGH);
            } 
            
            // RETURN CONNECT OK
            if(strcmp(rec_buf , "CONNECT") == 0)
            {
                char* send_buf;
                send_buf = malloc(30);
                
                sprintf(send_buf , "CONNECT_OK\n" );
                
                    if(sendto_return = sendto(socket_desc , send_buf , strlen(send_buf) ,0, (struct sockaddr *) &client, slen) ==  -1)
                    {
                       printf("\n ERROR in sendto = %d:\n" , sendto_return);
                    }
                free(send_buf);
            }
            
            // RETURN TEMP_1 VALUE
            if(strcmp(rec_buf , "TEMP_1") == 0)
            {
                char* send_buf;
                send_buf = malloc(30);
                
                sprintf(send_buf , "TEMP_1=%f\n" , smarthouse_struct.temp_1_value );
                
                    if(sendto_return = sendto(socket_desc , send_buf , strlen(send_buf) ,0, (struct sockaddr *) &client, slen) ==  -1)
                    {
                        printf("\n ERROR in sendto = %d:\n" , sendto_return);
                    }

                free(send_buf);
            }
            
            // RETURN TEMP_2 VALUE
            if(strcmp(rec_buf , "TEMP_2") == 0)
            {
                char* send_buf;
                send_buf = malloc(30);
                
                sprintf(send_buf , "TEMP_2=%f\n" , smarthouse_struct.temp_2_value );
                
                    if(sendto_return = sendto(socket_desc , send_buf , strlen(send_buf) ,0, (struct sockaddr *) &client, slen) ==  -1)
                    {
                        printf("\n ERROR in sendto = %d:\n" , sendto_return);
                    }

                free(send_buf);
            }
            
            // RETURN LIGHT VALUE
            if(strcmp(rec_buf , "LUX") == 0)
            {
                char* send_buf;
                send_buf = malloc(30);

                
                sprintf(send_buf , "LUX=%d\n" , smarthouse_struct.light_value );
                
                    if(sendto_return = sendto(socket_desc , send_buf , strlen(send_buf) ,0, (struct sockaddr *) &client, slen) ==  -1)
                    {
                        printf("\n ERROR in sendto = %d:\n" , sendto_return);
                    }
                free(send_buf);
            }
            
            // RETURN LOG
            if(strcmp(rec_buf , "LOG") == 0)
            {
                char* send_buf;
                send_buf = malloc(150);
                
                FILE *fd;
                size_t len =0;
                
                fd = fopen("/home/pi/SmartHouse/report_mk" , "r" );
                
                while(getline(&send_buf , &len , fd) != -1) //READ NEXT LINE
                {              
                    if(sendto_return = sendto(socket_desc , send_buf , strlen(send_buf) ,0, (struct sockaddr *) &client, slen) ==  -1)
                    {
                        printf("\n ERROR in sendto = %d:\n" , sendto_return);
                    }
                }
                
                free(send_buf);
                fclose(fd);
            }
            
            memset(rec_buf , 0 , BUFLEN);
            free(rec_buf);
            memset((char *) &client, 0, sizeof(client));
        }

        usleep(UDP_THREAD_SLEEP_uS);
        digitalWrite(LED_GREEN , LOW);
    }
}