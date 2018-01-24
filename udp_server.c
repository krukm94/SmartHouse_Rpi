
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

int8_t word_len = 0;
uint8_t compare_return;

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
    int8_t sendto_return = 0;
    
    while(1)
    {
        
#if PRINT_UDP_DATA_ON_TERMINAL == 1
        puts("%Waiting for incoming connections...\n");
#endif
        fflush(stdout);
        
        char* rec_buf;
        char *searching_string;
        char *searching_param;
        
        rec_buf = malloc(BUFLEN);
        searching_string = malloc(80);
        searching_param = malloc(50);
        
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

                sprintf(send_buf ,"$ Zapalono Diode!\n" );
                
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
                
                sprintf(send_buf ,"%f\n" , smarthouse_struct.temp_1_value );
                
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

                
                sprintf(send_buf  ,"%d\n" , smarthouse_struct.light_value );
                
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
               
                snprintf(send_buf , sizeof(send_buf) ,"%s\n" , smarthouse_struct.last_move );
                
                    if(sendto_return = sendto(socket_desc , send_buf , strlen(send_buf) ,0, (struct sockaddr *) &client, slen) ==  -1)
                    {
                        printf("\n ERROR in sendto = %d:\n" , sendto_return);
                    }            
                free(send_buf);  
            }
            
            // _--------------------------- >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> TESTY
            if(strcmp(rec_buf , "FRAME_TEST") == 0){
                
                char test_buf[30] = "TEST RAMKI-> DAREK TO GEJ";
                uint8_t command = 0x2A;
                uint8_t param_type = 0x53;
                
                send_frame(command, test_buf , strlen(test_buf) , param_type , 1);
            }
            
            // ----------------------------------------------------------------------------------->>>>>>>>>>>>>>>>>>>>>> WORK ON FRAMES
            
            int8_t ret = 0;
            uint8_t command,rw,type;
            uint16_t size;
            char payload[20];
            
            ret = decode_frame(rec_buf , &command , &rw , &type , &size , payload);
            
            printf("decode_frame = %d , command: 0x%x , rw: %d , type: %c , size: %d , %s\n\n" , ret , command , rw , type , size , payload);
            
            if(!ret){ //IF rec_buf contain proper frame
                
                int8_t send_return = 0;     // Return variable for send to function 
                char return_buf_glob[50];
                
                //COMMAND CONNECT
                    if(command == CONNECT){                            //0x2A = Check connect
                        char return_buf[15] = "CONNECT_OK";

                        if(!rw){
                            if(send_return = send_frame(CONNECT , return_buf , strlen(return_buf) , PARAM_TYPE_STRING , rw) != 0){
                                printf("$ Warning: sendto , file udp_server.c , line 258\n");
                            }
                        }

                    }
                //COMMAND TEMP_1_ACTIVE
                    else if(command == TEMP_1_ACTIVE){
                        
                        //GET VALUE
                        sprintf(return_buf_glob , "%d" , smarthouse_struct.temp_1_activate);
                        
                        //READ 
                        if(!rw) send_return = send_frame(command , return_buf_glob , strlen(return_buf_glob) , type , rw);
                        
                        //WRITE
                        else {
                            if(type == PARAM_TYPE_INT) { //Check type
                                smarthouse_struct.temp_1_activate = atoi(payload);
                                send_return = send_frame(command , "OK" , 4 , type , WRITE);
#if PRINT_FRAME_DATA == 1
                                printf("$ Zapis temp 1 active = %d \n" , smarthouse_struct.temp_1_activate);
#endif     
                            }
                            else{
                                send_return = send_frame(command , "Wrong type of temp active" , 30 , type , WRITE);
                            } 
                        }
                    }
                    
                //COMMAND TEMP_1_VALUE
                    else if(command == TEMP_1_VALUE){
                        
                        //GET VALUE
                        sprintf(return_buf_glob , "%0.3f" , smarthouse_struct.temp_1_value);
                        
                        //READ 
                        if(!rw) send_return = send_frame(command , return_buf_glob , strlen(return_buf_glob) , type , rw);
                        
                        //WRITE (USER CANT WRITE VALUE OF TEMP 1 !!!
                        else send_return = send_frame(command , "You can't write value of temperature!", 40 , type , WRITE);   
                    }
                
                //COMMAND TEMP_1_TS_HIGH
                    else if(command == TEMP_1_TS_HIGH){
                        
                        //GET VALUE
                        sprintf(return_buf_glob , "%0.3f" , smarthouse_struct.temp_1_threshold_high);
                        
                        //READ 
                        if(!rw) send_return = send_frame(command , return_buf_glob , strlen(return_buf_glob) , type , rw);
                        
                        //WRITE
                        else {
                            if(type == PARAM_TYPE_FLOAT) { //Check type
                                smarthouse_struct.temp_1_threshold_high = atof(payload);
                                send_return = send_frame(command , "OK" , 4 , type , WRITE);
#if PRINT_FRAME_DATA == 1
                                printf("$ Zapis temp 1 high threshold OK = %d \n" , smarthouse_struct.temp_1_threshold_high);
#endif 
                            }
                            else{
                                send_return = send_frame(command , "Wrong type of temp threshold high" , 30 , type , WRITE);
                            } 
                        }
                    }
                
                //COMMAND TEMP_1_TS_HIGH_ACT
                    else if(command == TEMP_1_TS_HIGH_ACT){
                        
                        uint8_t flag=0 , array_id = 0;
                        //Get nr of action and action char 
                        char nr_of_action_char = payload[0];
                        char action_char = payload[1];
                        
                      
                        //Chec values 
                        if(is_action_name_ok((uint8_t) nr_of_action_char , &array_id)) flag = 0x01;
                        if(is_action_ok((uint8_t) action_char)) flag = 0x01;
                        
                        if(!flag){
                            //GET VALUE
                            sprintf(return_buf_glob , "%d" , smarthouse_struct.temp_1_threshold_high_action[array_id]);

                            //READ 
                            if(!rw) send_return = send_frame(command , return_buf_glob , strlen(return_buf_glob) , type , rw);

                            //WRITE
                            else{
                                if(type == PARAM_TYPE_INT) { //Check type
                                    smarthouse_struct.temp_1_threshold_high_action[array_id] = atoi(&payload[1]);
                                    send_return = send_frame(command , "OK" , 4 , type , WRITE);
#if PRINT_FRAME_DATA == 1
                                    printf("$ Zapis temp 1 high threshold action[%d] OK = %d \n" , nr_of_action_char ,smarthouse_struct.temp_1_threshold_high_action[array_id]);
#endif 
                                }
                                else{
                                    send_return = send_frame(command , "Wrong type of temp threshold high action" , 30 , type , WRITE);
                                } 
                            }
                        }else send_return = send_frame(command , "Wronge action number or action type" , 40 , type , WRITE);
                    }
                
                
                    else{
                        char return_buf[15] = "Wrong command!";  
                        if(send_return = send_frame(command , return_buf , strlen(return_buf) , type , rw) != 0){
                                printf("$ Warning: sendto , file udp_server.c , line 258\n");
                        }
                    }
            }
            
            command = 0; rw = 0 ; type = 0 ; size = 0; ret = 0;
            
            // ClEAR BUFORS etc. etc.
            memset(rec_buf , 0 , BUFLEN);
            free(rec_buf);
            free(searching_string);
            free(searching_param);
     
            memset( &client, 0, sizeof(client));
            
            
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
    uint8_t cnt2 = 0;
    uint8_t read = 0;
  
    //EGZAMPLE OF buffer = "COMMAND=PARAM\n"
    
    while(read != 0x0A)                      // 0x0A = "\n"                     
    { 
        read = buffer[cnt];                  // Save char in local variable
        
        if(read == 0x3D) {                   // 0x3D = "="
            cnt2 = cnt + 1;                
            *read_word_len = cnt;        // Save word len
        }
        else read_word[cnt] = read;         // if read char is not equal "=" then save it to buffer
 
        cnt++;
    }  
   
    for(cnt = 0 ; cnt < 20 ; cnt++){
        read_param[cnt] = read_word[cnt2 + cnt];
        if(read_param[cnt] == 0x0A) break;         // 0x0A = "\n"
    }

    return strcmp(read_word , word);
}

/* 
 * Func:   send_frame
 * Description: 
 * Return 
 */

int8_t send_frame(uint8_t  command , char* param , uint16_t size_of_param , uint8_t param_type , uint8_t read_or_write)
{
    
    //Variables
    int8_t return_vaule;
    char frame_buf[size_of_param + 7];   //Frame 
    char start_frame[3];
  
    start_frame[0] = 0x24; // $
    start_frame[1] = 0x4B; // K
    start_frame[2] = 0x47; // G

    //ADD Frame Start Bytes
    memcpy(&frame_buf[0] , start_frame , sizeof(start_frame));
    
    //ADD Command
    frame_buf[3] = command;
   
    //ADD Read or Write byte
    if(read_or_write == 1) frame_buf[4] = 0x57; // 0x57 = "W"
    else frame_buf[4] - 0x52; // 0x52 = "R"
    
    //ADD param type (Int = 0x49 , Float = 0x46 , String 0x53 
    if(param_type == 0x49 || param_type == 0x46 || param_type == 0x53) frame_buf[5] = param_type;
    else{
        printf("-----> Warning! Wrong PARAM TYPE in send_frame (file udp_server.c line 309)\n");
    }
    //ADD size of param
    frame_buf[6] = size_of_param;
    //Fill payload
    memcpy(&frame_buf[7] , (char*) param , size_of_param);
    
    //SEND FRAME
    if(return_vaule = sendto(socket_desc , frame_buf , sizeof(frame_buf) ,0, (struct sockaddr *) &client, slen) ==  -1)
    {
        printf("\n ERROR in sendto (File udp_server.c line: 328\n");
    }      
    
    return return_vaule;
}

/* 
 * Func:   decode_frame
 * Description: 
 * Return 
 */

int8_t decode_frame(char *rcv_frame , uint8_t* command ,uint8_t* rw , uint8_t* type , uint16_t *size , char *payload)
{
    //Variables
    int8_t return_value = 0 , sendto_return = 0;
    char return_bufor[30];
    uint8_t cnt = 0,start_of_frame = 0;
    
    //Find "Start of Frame" it should be equal "$KG"
    for(cnt = 0 ; cnt < 20 ; cnt++){
        if(rcv_frame[cnt] == 0x24 && rcv_frame[cnt+1] == 0x4B && rcv_frame[cnt+2] == 0x47) {
            start_of_frame = cnt;
            
            //GET Command
            *command = rcv_frame[cnt+3];
            
            //Get Read or Write byte
            if(rcv_frame[cnt+4] == 0x57) *rw = 1;
            else if(rcv_frame[cnt+4] == 0x52) *rw = 0;
            else{
                sprintf(return_bufor , "Wrong R/W Byte!");
                
                return_value = 0x01;  
                
                //Send error messege
                send_frame(*command , return_bufor , strlen(return_bufor), *type , *rw);
            }
            
            //GET Type of Payload
            *type = rcv_frame[cnt+5];
            
            if(!(*type == 0x49 || *type == 0x46 || *type == 0x53)){
                sprintf(return_bufor , "Wrong Type Byte!");
                
                return_value = 0x02;
                
                //Send error messege
                send_frame(*command , return_bufor , strlen(return_bufor), *type , *rw);
            }

            //GET Size
            *size = rcv_frame[cnt+6];

            //GET Paylod
            memcpy(payload , &rcv_frame[cnt+7] , *size);
            
            break;
        }
        else return 0x03;
    }
    

    
    return return_value;
}
/* 
 * Func:   is_action_ok
 * Description: Check is get actions is proper
 * Return 0 if action belong to know actions, 1 if not
 */

int8_t is_action_ok(uint8_t action){
    
    int8_t return_param = 1;
    
    if(action == RELAY_1_ON) return_param = 0;
    if(action == RELAY_1_OFF) return_param = 0;
    if(action == RELAY_2_ON) return_param = 0;
    if(action == RELAY_2_OFF) return_param = 0;
    if(action == RELAY_3_ON) return_param = 0;
    if(action == RELAY_3_OFF) return_param = 0;
    if(action == RELAY_4_ON) return_param = 0;
    if(action == RELAY_4_OFF) return_param = 0;
    
    if(action == LED_1_ON) return_param = 0;
    if(action == LED_1_OFF) return_param = 0;
    
    if(action == LED_2_ON) return_param = 0;
    if(action == LED_2_OFF) return_param = 0;
    
    return return_param;
}

/* 
 * Func:   is_action_name_ok
 * Description: Check is get action name is proper 
 * Return array index if action belong to know actions names, 1 if not
 */

int8_t is_action_name_ok(uint8_t action_name , uint8_t* array_index){
    
    int8_t return_param = 1;
    
    if(action_name == ACTION1) {return_param = 0; *array_index = 0;}
    if(action_name == ACTION2) {return_param = 0; *array_index = 0;}
    if(action_name == ACTION3) {return_param = 0; *array_index = 0;}
    if(action_name == ACTION4) {return_param = 0; *array_index = 0;}
    if(action_name == ACTION5) {return_param = 0; *array_index = 0;}
    
    return return_param;
}


                    

/* 
 * Func:   procesing_temp_th_command
 * Description: Process Temperature Threshold Commands
 * Return 
 */

//void processing_temp_th_command(uint8_t command , char* payload , uint8_t* rw , uint8_t type){
//    
//    uint8_t flag=0 , array_id = 0 , send_return = 0;
//    //Get nr of action and action char 
//    char nr_of_action_char = payload[0];
//    char action_char = payload[1];
//    char return_buf[50];
//    char param_string[20];
//
//    //Chec values 
//    if(is_action_name_ok((uint8_t) nr_of_action_char , &array_id)) flag = 0x01;
//    if(is_action_ok((uint8_t) action_char)) flag = 0x01;
//
//    if(!flag){
//        //GET VALUE
//        if(command == TEMP_1_TS_HIGH_ACT){
//            sprintf(return_buf , "%d" , smarthouse_struct.temp_1_threshold_high_action[array_id]);
//            sprintf(param_string , "temp_1_ ")
//        }
//        
//
//        //READ 
//        if(!rw) send_return = send_frame(command , return_buf , strlen(return_buf) , type , rw);
//
//        //WRITE
//        else{
//            if(type == PARAM_TYPE_INT) { //Check type
//                smarthouse_struct.temp_1_threshold_high_action[array_id] = atoi(&payload[1]);
//                send_return = send_frame(command , "OK" , 4 , type , WRITE);
//#if PRINT_FRAME_DATA == 1
//                printf("$ Zapis temp 1 high threshold action[%d] OK = %d \n" , nr_of_action_char ,smarthouse_struct.temp_1_threshold_high_action[array_id]);
//#endif 
//            }
//            else{
//                send_return = send_frame(command , "Wrong type of temp threshold high action" , 30 , type , WRITE);
//            } 
//        }
//       }else send_return = send_frame(command , "Wronge action number or action type" , 40 , type , WRITE);
////  
//}