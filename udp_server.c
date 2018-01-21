
/* 
 * File:   udp_server.h
 * Author: mkruk
 *
 * Created on 12 grudnia 2017, 15:55
 */

#include <stdlib.h>

#include "udp_server.h"


extern volatile struct smarthouse_config smarthouse_struct;

struct sockaddr_in server, client;
int socket_desc, new_socket, c, recv_len;
int slen;


// >>>>>>>>>>>> Thread variables
pthread_t UDP_thread_IDs;   //identyfikator wątku UDP

/* 
 * Func:   udp_server_init
 * Description: Init UDP server on PORT specifying in SmartHouse_pin_def.h
 */
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

/* 
 * Func:   udp_thread_1
 * Description: UDP THREAD
 */
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
                
                sprintf(send_buf , "%f\n" , smarthouse_struct.temp_1_value );
                
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
                
                sprintf(send_buf , "%f\n" , smarthouse_struct.temp_2_value );
                
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

                
                sprintf(send_buf , "%d\n" , smarthouse_struct.light_value );
                
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
            
            // RETURN MOVE STATUS
            if(strcmp(rec_buf , "LAST_MOVE") == 0)
            {
                char* send_buf;
                send_buf = malloc(150);
               
                sprintf(send_buf , "%s\n" , smarthouse_struct.last_move );
                
                    if(sendto_return = sendto(socket_desc , send_buf , strlen(send_buf) ,0, (struct sockaddr *) &client, slen) ==  -1)
                    {
                        printf("\n ERROR in sendto = %d:\n" , sendto_return);
                    }            
                free(send_buf);  
            }
           
            //Reserve memory for stirng
//            volatile char *searching_string;
//            volatile char *searching_param;
//
//            searching_string = malloc(80);
//            searching_param = malloc(50);
//            uint8_t word_len = 0;
//            uint8_t compare_return;
//
//            if(!(compare_return = compare_word(searching_string , &word_len , searching_param ,"SET_TEMP1_ACTIVE\n" , rec_buf)))
//            {
//               // printf("SET OK, PARAM:%s\n" , searching_param);
//                memset(searching_string , 0 , strlen(searching_string));
//            }
//            
//                
//            if(!(compare_return = compare_word(searching_string , &word_len , searching_param  ,"ELOELO\n" , rec_buf)))
//            {
//               printf("ELO OK\n");
//               memset(searching_string , 0 , strlen(searching_string));
//            }
//  
//            free(searching_string);
//            free(searching_param);
            
            
            
            
            // TEMP_1_ACTIVE_READ
//            if(strcmp(rec_buf , "READ_TEMP1_ACTIVE") == 0)
//            {
//                char* send_buf;
//                send_buf = malloc(150);
//               
//                sprintf(send_buf , "%d\n" , smarthouse_struct.temp_1_activate );
//                
//                    if(sendto_return = sendto(socket_desc , send_buf , strlen(send_buf) ,0, (struct sockaddr *) &client, slen) ==  -1)
//                    {
//                        printf("\n ERROR in sendto = %d:\n" , sendto_return);
//                    }            
//                free(send_buf);  
//            }

               
//            if(getc(rec_buf) == "SET_TEMP1_ACTIVE")
//            {
//                printf("RECIVE: SET_TEMP1_ACTIVE\n");
//            }
//            // TEMP_1_ACTIVE_SET
//            if(strcmp(rec_buf , "SET_TEMP1_ACTIVE_1") == 0)
//            {
//                char* send_buf;
//                send_buf = malloc(150);
//                
//                smarthouse_struct.temp_1_activate = 1;
//                
//                sprintf(send_buf , "TEMP1_ACTIVATE\n");
//                
//                if(sendto_return = sendto(socket_desc , send_buf , strlen(send_buf) ,0, (struct sockaddr *) &client, slen) ==  -1)
//                {
//                    printf("\n ERROR in sendto = %d:\n" , sendto_return);
//                }   
//                
//                free(send_buf);  
//            }
//            
//            // TEMP_1_ACTIVE_SET
//            if(strcmp(rec_buf , "SET_TEMP1_ACTIVE_0") == 0)
//            {
//                char* send_buf;
//                send_buf = malloc(150);
//                
//                smarthouse_struct.temp_1_activate = 0;
//                
//                free(send_buf);  
//            }
            
            memset(rec_buf , 0 , BUFLEN);
            free(rec_buf);
            memset((char *) &client, 0, sizeof(client));
        }

        usleep(UDP_THREAD_SLEEP_uS);
        digitalWrite(LED_GREEN , LOW);
    }
    

}

/* 
 * Func:   compare_word
 * Description: Functions gets first word from "buffer" (read everything since to find "\n" char and compare it with "word" 
 * and get param "read_param" after "\n" char
 * Return strcmp return value
 */
int8_t compare_word(char* read_word , uint8_t* read_word_len , char* read_param , char* word , char* buffer)
{
    uint8_t cnt = 0;
    uint8_t read = 0;
    char *read_buf;
    int read_p;
    
    read_buf = malloc(30);
    
    while(read != 0x0A)                         // GET WORD
    {
        read_word[cnt] = buffer[cnt];
        read = read_word[cnt];
        cnt++;
    }  
    
    read_word_len = cnt - 1;
    read = 0;
    
    while(read != 0x0A)                       // GET PARAM
    {  
        read_buf[cnt] = buffer[cnt];
        read = read_buf[cnt];
        
        printf("param %c %s\n" , read , (char*)read_buf);
        cnt++;
    }
    read_p = (int) atoi(read_buf);
    
    printf("word: %s , param: %s , read_p: %d , buf:%s\n" , read_word , read_buf , read_p ,buffer);
    
    free(read_buf);
    
    return strcmp(read_word , word);
}


            

            //Looking for stings
//            cnt = 0;
//            uint8_t search_pos = 0;
//            
//            for(cnt = 0; cnt < slen ; cnt++)
//            {
//                if(rec_buf[cnt] == searching_string[search_pos])
//                {
//                    search_pos++;
//                    if(search_pos == strlen(searching_string))
//                    {
//                        
//                    }     
//                }
//                else  search_pos = 0;
//            }

