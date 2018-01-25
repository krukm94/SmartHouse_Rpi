
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

int8_t word_len = 0 , rw_flag = 0;
uint8_t compare_return;

// >>>>>>>>>>>> Thread variables
pthread_t UDP_thread_IDs;   //identyfikator wątku UDP

// GLOBAL FRAME STRUCT
struct frame_struct frame_str;
struct frame_struct *frame_ptr;

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
            

            int8_t ret = 0;
            uint8_t command,rw,type;
            uint16_t size;
            char payload[50];
            
            ret = decode_frame(rec_buf , &command , &rw , &type , &size , payload);
            
#if PRINT_FRAME_DATA == 1          
            printf("decode_frame = %d , command: 0x%x , rw: %d , type: %c , size: %d , %s\n\n" , ret , command , rw , type , size , payload);
#endif      
            
            //SET rw FLAG
            if(rw) rw_flag = 0x01;
            
            if(!ret){ //IF rec_buf contain proper frame

                int8_t send_return = 0;     // Return variable for send to function 
                char return_buf_glob[50];
                
                //FELL frame struct
                frame_str.command = command;
                memcpy(frame_str.payload  , payload , sizeof(payload));
                frame_str.param_type = type;
                frame_str.rw = rw;
                frame_str.size_of_param = size;
                
                
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
                                printf("$ Zapis temp 1 high threshold OK = %0.3f \n" , smarthouse_struct.temp_1_threshold_high);
#endif 
                            }
                            else{
                                send_return = send_frame(command , "Wrong type of temp threshold high" , 30 , type , WRITE);
                            } 
                        }
                    }
                
                //COMMAND TEMP_1_TS_LOW
                    else if(command == TEMP_1_TS_LOW){
                        
                        //GET VALUE
                        sprintf(return_buf_glob , "%0.3f" , smarthouse_struct.temp_1_threshold_low);
                        
                        //READ 
                        if(!rw) send_return = send_frame(command , return_buf_glob , strlen(return_buf_glob) , type , rw);
                        
                        //WRITE
                        else {
                            if(type == PARAM_TYPE_FLOAT) { //Check type
                                smarthouse_struct.temp_1_threshold_low = atof(payload);
                                send_return = send_frame(command , "OK" , 4 , type , WRITE);
#if PRINT_FRAME_DATA == 1
                                printf("$ Zapis temp 1 low threshold OK = %0.3f \n" , smarthouse_struct.temp_1_threshold_low);
#endif 
                            }
                            else{
                                send_return = send_frame(command , "Wrong type of temp threshold low" , 30 , type , WRITE);
                            } 
                        }
                    }
                
                // IF THRESHOLD COMMAND
                    else if(command == TEMP_1_TS_HIGH_ACT || command == TEMP_1_TS_LOW_ACT || command == TEMP_2_TS_HIGH_ACT || command == TEMP_2_TS_LOW_ACT || command == LUX_TS_HIGH_ACT || command == LUX_TS_LOW_ACT || command  == MOTION_ACT || command == BUTTON_ACT || command == WETNESS_ACT){
                        
                        frame_ptr = &frame_str;
                        processing_th_command(frame_ptr);
        
                    }
                
                
                
                //COMMAND TEMP_2_ACTIVE
                    else if(command == TEMP_2_ACTIVE){
                        
                        //GET VALUE
                        sprintf(return_buf_glob , "%d" , smarthouse_struct.temp_2_activate);
                        
                        //READ 
                        if(!rw) send_return = send_frame(command , return_buf_glob , strlen(return_buf_glob) , type , rw);
                        
                        //WRITE
                        else {
                            if(type == PARAM_TYPE_INT) { //Check type
                                smarthouse_struct.temp_2_activate = atoi(payload);
                                send_return = send_frame(command , "OK" , 4 , type , WRITE);
#if PRINT_FRAME_DATA == 1
                                printf("$ Zapis temp 2 active = %d \n" , smarthouse_struct.temp_2_activate);
#endif     
                            }
                            else{
                                send_return = send_frame(command , "Wrong type of temp active" , 30 , type , WRITE);
                            } 
                        }
                    }
                    
                //COMMAND TEMP_2_VALUE
                    else if(command == TEMP_2_VALUE){
                        
                        //GET VALUE
                        sprintf(return_buf_glob , "%0.3f" , smarthouse_struct.temp_2_value);
                        
                        //READ 
                        if(!rw) send_return = send_frame(command , return_buf_glob , strlen(return_buf_glob) , type , rw);
                        
                        //WRITE (USER CANT WRITE VALUE OF TEMP 2 !!!
                        else send_return = send_frame(command , "You can't write value of temperature!", 40 , type , WRITE);   
                    }
                
                
                //COMMAND TEMP_2_TS_HIGH
                    else if(command == TEMP_2_TS_HIGH){
                        
                        //GET VALUE
                        sprintf(return_buf_glob , "%0.3f" , smarthouse_struct.temp_2_threshold_high);
                        
                        //READ 
                        if(!rw) send_return = send_frame(command , return_buf_glob , strlen(return_buf_glob) , type , rw);
                        
                        //WRITE
                        else {
                            if(type == PARAM_TYPE_FLOAT) { //Check type
                                smarthouse_struct.temp_2_threshold_high = atof(payload);
                                send_return = send_frame(command , "OK" , 4 , type , WRITE);
#if PRINT_FRAME_DATA == 1
                                printf("$ Zapis temp 2 high threshold OK = %0.3f \n" , smarthouse_struct.temp_2_threshold_high);
#endif 
                            }
                            else{
                                send_return = send_frame(command , "Wrong type of temp threshold high" , 30 , type , WRITE);
                            } 
                        }
                    }
                
                //COMMAND TEMP_2_TS_LOW
                    else if(command == TEMP_2_TS_LOW){
                        
                        //GET VALUE
                        sprintf(return_buf_glob , "%0.3f" , smarthouse_struct.temp_2_threshold_low);
                        
                        //READ 
                        if(!rw) send_return = send_frame(command , return_buf_glob , strlen(return_buf_glob) , type , rw);
                        
                        //WRITE
                        else {
                            if(type == PARAM_TYPE_FLOAT) { //Check type
                                smarthouse_struct.temp_2_threshold_low = atof(payload);
                                send_return = send_frame(command , "OK" , 4 , type , WRITE);
#if PRINT_FRAME_DATA == 1
                                printf("$ Zapis temp 2 low threshold OK = %0.3f \n" , smarthouse_struct.temp_2_threshold_low);
#endif 
                            }
                            else{
                                send_return = send_frame(command , "Wrong type of temp threshold low" , 30 , type , WRITE);
                            } 
                        }
                    }
                
                
               //COMMAND LUX_ACTIVE
                    else if(command == LUX_ACTIVE){
                        
                        //GET VALUE
                        sprintf(return_buf_glob , "%d" , smarthouse_struct.light_sensor_activate);
                        
                        //READ 
                        if(!rw) send_return = send_frame(command , return_buf_glob , strlen(return_buf_glob) , type , rw);
                        
                        //WRITE
                        else {
                            if(type == PARAM_TYPE_INT) { //Check type
                                smarthouse_struct.light_sensor_activate = atoi(payload);
                                send_return = send_frame(command , "OK" , 4 , type , WRITE);
#if PRINT_FRAME_DATA == 1
                                printf("$ Zapis lux active = %d \n" , smarthouse_struct.light_sensor_activate);
#endif     
                            }
                            else{
                                send_return = send_frame(command , "Wrong type of lux active" , 30 , type , WRITE);
                            } 
                        }
                    }
                    
                //COMMAND LUX_VALUE
                    else if(command == LUX_VALUE){
                        
                        //GET VALUE
                        sprintf(return_buf_glob , "%d" , smarthouse_struct.light_value);
                        
                        //READ 
                        if(!rw) send_return = send_frame(command , return_buf_glob , strlen(return_buf_glob) , type , rw);
                        
                        //WRITE (USER CANT WRITE VALUE OF LUX !!!
                        else send_return = send_frame(command , "You can't write value of lux!", 40 , type , WRITE);   
                    }
                
                //COMMAND LUX_TS_HIGH
                    else if(command == LUX_TS_HIGH){
                        
                        //GET VALUE
                        sprintf(return_buf_glob , "%d" , smarthouse_struct.light_threshold_high);
                        
                        //READ 
                        if(!rw) send_return = send_frame(command , return_buf_glob , strlen(return_buf_glob) , type , rw);
                        
                        //WRITE
                        else {
                            if(type == PARAM_TYPE_INT) { //Check type
                                smarthouse_struct.light_threshold_high = atoi(payload);
                                send_return = send_frame(command , "OK" , 4 , type , WRITE);
#if PRINT_FRAME_DATA == 1
                                printf("$ Zapis lux high threshold OK = %d \n" , smarthouse_struct.light_threshold_high);
#endif 
                            }
                            else{
                                send_return = send_frame(command , "Wrong type of lux threshold high" , 30 , type , WRITE);
                            } 
                        }
                    }
                
                //COMMAND LUX_TS_LOW
                    else if(command == LUX_TS_LOW){
                        
                        //GET VALUE
                        sprintf(return_buf_glob , "%d" , smarthouse_struct.light_threshold_low);
                        
                        //READ 
                        if(!rw) send_return = send_frame(command , return_buf_glob , strlen(return_buf_glob) , type , rw);
                        
                        //WRITE
                        else {
                            if(type == PARAM_TYPE_INT) { //Check type
                                smarthouse_struct.light_threshold_low = atoi(payload);
                                send_return = send_frame(command , "OK" , 4 , type , WRITE);
#if PRINT_FRAME_DATA == 1
                                printf("$ Zapis lux low threshold OK = %d \n" , smarthouse_struct.light_threshold_low);
#endif 
                            }
                            else{
                                send_return = send_frame(command , "Wrong type of temp threshold low" , 30 , type , WRITE);
                            } 
                        }
                    }
                
                //COMMAND WETNESS_ACTIVE
                    else if(command == WETNESS_ACTIVE){
                        
                        //GET VALUE
                        sprintf(return_buf_glob , "%d" , smarthouse_struct.wetness_sensor_activate);
                        
                        //READ 
                        if(!rw) send_return = send_frame(command , return_buf_glob , strlen(return_buf_glob) , type , rw);
                        
                        //WRITE
                        else {
                            if(type == PARAM_TYPE_INT) { //Check type
                                smarthouse_struct.wetness_sensor_activate = atoi(payload);
                                send_return = send_frame(command , "OK" , 4 , type , WRITE);
#if PRINT_FRAME_DATA == 1
                                printf("$ Zapis wetness active = %d \n" , smarthouse_struct.wetness_sensor_activate);
#endif     
                            }
                            else{
                                send_return = send_frame(command , "Wrong type of wetnesss active" , 30 , type , WRITE);
                            } 
                        }
                    }
                
                //COMMAND MOTION_ACTIVE
                    else if(command == MOTION_ACTIVE){
                        
                        //GET VALUE
                        sprintf(return_buf_glob , "%d" , smarthouse_struct.motion_sensor_activate);
                        
                        //READ 
                        if(!rw) send_return = send_frame(command , return_buf_glob , strlen(return_buf_glob) , type , rw);
                        
                        //WRITE
                        else {
                            if(type == PARAM_TYPE_INT) { //Check type
                                smarthouse_struct.motion_sensor_activate = atoi(payload);
                                send_return = send_frame(command , "OK" , 4 , type , WRITE);
#if PRINT_FRAME_DATA == 1
                                printf("$ Zapis motion sensor active = %d \n" , smarthouse_struct.motion_sensor_activate);
#endif     
                            }
                            else{
                                send_return = send_frame(command , "Wrong type of motion sensor active" , 30 , type , WRITE);
                            } 
                        }
                    }
                
                //COMMAND MOTION LAST DET
                    else if(command == MOTION_LAST_DET){
                        
                        //GET VALUE
                        sprintf(return_buf_glob , "%s" , smarthouse_struct.last_move);
                        
                        //READ 
                        if(!rw) send_return = send_frame(command , return_buf_glob , strlen(return_buf_glob) , type , rw);
                        
                        //WRITE (USER CANT WRITE VALUE OF LUX !!!
                        else send_return = send_frame(command , "You can't write value of last move!", 40 , type , WRITE);   
                    }
                
                //COMMAND LOG
                    else if(command == LOG){
 
                        //READ 
                        if(!rw){
                            FILE *fd1;
                            char *send_buf;
                            size_t len = 0;
                            
                            send_buf = malloc(150);
                            
                            fd1 = fopen("/home/pi/SmartHouse/report_mk" , "r" );         // Atribute "r" means read
                            
                            while(getline(&send_buf , &len , fd1) != -1) //READ NEXT LINE
                            {    
                                send_return = send_frame(command , send_buf , strlen(send_buf), type , rw);
                                memset(send_buf , 0 , len);
                            }                          
                            fclose(fd1);
                            free(send_buf);
                        }
                        //WRITE (USER CANT WRITE VALUE OF LUX !!!
                        else send_return = send_frame(command , "You can't write value of LOG!", 40 , type , WRITE);                                              
                    }
                   else{
                        char return_buf[15] = "Wrong command!";  
                        if(send_return = send_frame(command , return_buf , strlen(return_buf) , type , rw) != 0){
                                printf("$ Warning: sendto , file udp_server.c , line 258\n");
                        }
                    } 
            } 

            //Zapis do pliku jeżeli nastapił zapis jakies wartosci
            if(rw_flag) {save_config(); rw_flag = 0;}
            
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

void processing_th_command(struct frame_struct* fr_ptr){
    
    uint8_t flag = 0 , array_id = 0 , send_return = 0;
    //Get nr of action and action char 

    char return_buf[50];
    char param_string[40];
    
    //pointer to smarthouse structur param
    uint8_t* sm_struct_ptr;
    
    //Get values frome payload
    char action_name = fr_ptr -> payload[0];
    char action = fr_ptr -> payload[1];

    //Chec values 
    if(is_action_name_ok((uint8_t) action_name , &array_id)) flag = 0x01;
    if(is_action_ok((uint8_t) action)) flag = 0x01;

    if(!flag){
            
//        //GET VALUE
        if(fr_ptr->command == TEMP_1_TS_HIGH_ACT){
            sm_struct_ptr = smarthouse_struct.temp_1_threshold_high_action; sprintf(param_string , "temp_1_threshold_high_action");}
        
        if(fr_ptr->command == TEMP_1_TS_LOW_ACT){
            sm_struct_ptr = smarthouse_struct.temp_1_threshold_low_action; sprintf(param_string , "temp_1_threshold_low_action");}
        
        if(fr_ptr->command == TEMP_2_TS_HIGH_ACT){
            sm_struct_ptr = smarthouse_struct.temp_2_threshold_high_action; sprintf(param_string , "temp_2_threshold_high_action");}
        
        if(fr_ptr->command == TEMP_2_TS_LOW_ACT){
            sm_struct_ptr = smarthouse_struct.temp_2_threshold_low_action; sprintf(param_string , "temp_2_threshold_low_action"); }
        
        if(fr_ptr->command == LUX_TS_HIGH_ACT){
            sm_struct_ptr = smarthouse_struct.light_threshold_high_action; sprintf(param_string , "light_threshold_high_action");}
        
        if(fr_ptr->command == LUX_TS_LOW_ACT){
            sm_struct_ptr = smarthouse_struct.light_threshold_low_action; sprintf(param_string , "light_threshold_low_action");}

        if(fr_ptr->command == MOTION_ACT){
            sm_struct_ptr = smarthouse_struct.motion_threshold_action; sprintf(param_string , "motion_threshold_action");}
        
        if(fr_ptr->command == BUTTON_ACT){
            sm_struct_ptr = smarthouse_struct.button_action; sprintf(param_string , "button_action");}
        
        if(fr_ptr->command == WETNESS_ACT){
            sm_struct_ptr = smarthouse_struct.wetness_threshold_action; sprintf(param_string , "wetness_threshold_action");}
        
        //Prepare bufors
         sprintf(return_buf , "%d" , sm_struct_ptr[array_id]);  
         
#if PRINT_FRAME_DATA == 1
         printf("%s\n" , return_buf);
#endif
         
        //READ 
        if(!fr_ptr->rw){ 
            send_return = send_frame(fr_ptr->command , return_buf , strlen(return_buf) , fr_ptr->param_type , fr_ptr->rw);
            
#if PRINT_FRAME_DATA == 1
            printf("$ Read ok, param: %s value: %s \n" , param_string , return_buf);
#endif
        }
        //WRITE
        else{
            if(fr_ptr->param_type == PARAM_TYPE_INT) { //Check type
                sm_struct_ptr[array_id] = action;
                send_return = send_frame(fr_ptr->command , "OK" , 4 , fr_ptr->param_type , WRITE);
                
#if PRINT_FRAME_DATA == 1
                printf("$ Zapis %s [%d] OK = %d \n" ,param_string, array_id, sm_struct_ptr[array_id]);
#endif 
            }
            else{
                sprintf(return_buf , "Wrong type of %s" , param_string);
                send_return = send_frame(fr_ptr->command  , return_buf , sizeof(return_buf) , fr_ptr->param_type , WRITE);
                
#if PRINT_FRAME_DATA == 1
                printf("%s\n" , return_buf);
#endif
            } 
        }
       }else {send_return = send_frame(fr_ptr->command , "Wronge action number or action type" , 40 , fr_ptr->param_type , WRITE);
#if PRINT_FRAME_DATA == 1
                printf("%s\n" , return_buf);
#endif
       }
//  
    memset(&frame_str , 0 , sizeof(frame_str));
    printf("$ Po wyczyszczeniu frame_ptr, \ncommand:%d , payload: %s\n" , frame_ptr->command , frame_ptr ->payload);
}
