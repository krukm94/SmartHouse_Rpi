/* 
 * File:   gpio_mk.c
 * Author: Michał Kruk
 *
 * Created on 06 stycz 2018
 */

#include "SmartHouse_pin_def.h"

//GLOBAL CONFIG SRUCTURE
extern volatile struct smarthouse_config smarthouse_struct;

int config_file_descriptor;


/* 
 * Func:  error_Func
 * Description: Error code
 */
void error_Func(void)
{    
    printf("\n\n ERROR!!!\n");
    while(1){
        digitalWrite(LED_RED , HIGH);
        delay(100);
        digitalWrite(LED_RED , LOW);
        delay(100);
    }
}

/* 
 * Func:  action
 * Description: decode action enum variable
 */
void action(uint8_t action)
{
    switch (action)
    {
        // RELAY 1
        case RELAY_1_ON:
            digitalWrite(RELAY_CH1 , HIGH);
            save_report("Relay 1 ON");
#if PRINT_ACTION_CHANGES_ON_TERMINAL  == 1
            printf("$ Relay 1 ON!\n");
#endif
            break;
            
        case RELAY_1_OFF:  
            digitalWrite(RELAY_CH1 , LOW);
            save_report("Relay 1 OFF");
#if PRINT_ACTION_CHANGES_ON_TERMINAL  == 1
            printf("$ Relay 1 OFF!\n");
#endif
            break;
          
        // RELAY 2    
        case RELAY_2_ON:
            digitalWrite(RELAY_CH2 , HIGH);
            save_report("Relay 2 ON");
#if PRINT_ACTION_CHANGES_ON_TERMINAL  == 1
            printf("$ Relay 2 ON!\n");
#endif
            break;
            
        case RELAY_2_OFF:  
            digitalWrite(RELAY_CH2 , LOW);
            save_report("Relay 2 OFF");
#if PRINT_ACTION_CHANGES_ON_TERMINAL  == 1
            printf("$ Relay 2 OFF!\n");
#endif
            break;  
            
        // RELAY 3
        case RELAY_3_ON:
            digitalWrite(RELAY_CH3 , HIGH);
            save_report("Relay 3 ON");
#if PRINT_ACTION_CHANGES_ON_TERMINAL  == 1
            printf("$ Relay 3 ON!\n");
#endif
            break;
            
        case RELAY_3_OFF:  
            digitalWrite(RELAY_CH3 , LOW);
            save_report("Relay 3 OFF");
#if PRINT_ACTION_CHANGES_ON_TERMINAL  == 1
            printf("$ Relay 3 OFF!\n");
#endif
            break;
            
        // RELAY 4
        case RELAY_4_ON:
            digitalWrite(RELAY_CH4 , HIGH);
            save_report("Relay 4 ON");
#if PRINT_ACTION_CHANGES_ON_TERMINAL  == 1
            printf("$ Relay 4 ON!\n");
#endif 
            break;
            
        case RELAY_4_OFF:  
            digitalWrite(RELAY_CH4 , LOW);
            save_report("Relay 4 OFF");
#if PRINT_ACTION_CHANGES_ON_TERMINAL  == 1
            printf("$ Relay 4 OFF!\n");
#endif 
            break;
            
        // LED 1
        case LED_1_ON:
            digitalWrite(LED_GREEN , HIGH);
            save_report("LED GREEN ON");
#if PRINT_ACTION_CHANGES_ON_TERMINAL  == 1
            printf("$ LED 1 ON!\n");
#endif
            break;
            
        case LED_1_OFF:
            digitalWrite(LED_GREEN , LOW);
            save_report("LED GREEN OFF");
#if PRINT_ACTION_CHANGES_ON_TERMINAL  == 1
            printf("$ LED 1 OFF!\n");
#endif
            break;
            
        // LED 2
        case LED_2_ON:
            digitalWrite(LED_RED , HIGH);
            save_report("LED RED ON");
#if PRINT_ACTION_CHANGES_ON_TERMINAL  == 1
            printf("$ LED 2 ON!\n");
#endif
            break;
            
        case LED_2_OFF:
            digitalWrite(LED_RED , LOW);
            save_report("LED RED OFF");
#if PRINT_ACTION_CHANGES_ON_TERMINAL  == 1
            printf("$ LED 2 OFF!\n");
#endif
            break;
            
        case 0:
#if PRINT_ACTION_CHANGES_ON_TERMINAL  == 1
            printf("$ NO ACTION!\n");
#endif
            break;
            
        default:
            printf("$ Wrong Action!\n");
    }
}

/* 
 * Func:  save_config()
 * Description: saving configuration of system
 */
void save_config()
{
    FILE *fd;
    char* write_buf;
    uint8_t cnt = 0;
    
    write_buf = malloc(200);
    
    //OPEN FILE
    fd = fopen("/home/pi/SmartHouse/config_mk" , "w+" );

    sprintf(write_buf , "SMART HOUSE CONFIG FILE\n" );
    fputs(write_buf , fd);

    //Save to file
    // TEMP 1 THRESHOLD
    sprintf(write_buf , "temp_1_threshold_high\n%f\ntemp_1_threshold_low\n%f\n" , smarthouse_struct.temp_1_threshold_high , smarthouse_struct.temp_1_threshold_low);
    fputs(write_buf , fd);
    
    //TEMP 1 THRESHOLD HIGH ACTIOn
    sprintf(write_buf , "temp_1_threshold_high_action\n");
    fputs(write_buf , fd);   
    for(cnt = 0 ; cnt < sizeof(smarthouse_struct.temp_1_threshold_high_action) ; cnt++)
    {
        sprintf(write_buf , "%d\n" , smarthouse_struct.temp_1_threshold_high_action[cnt]);
        fputs(write_buf , fd);
    }

    //TEMP 1 THRESHOLD LOW ACTIOn
    sprintf(write_buf , "temp_1_threshold_low_action\n");
    fputs(write_buf , fd);
    for(cnt = 0 ; cnt < sizeof(smarthouse_struct.temp_1_threshold_low_action) ; cnt++)
    {
        sprintf(write_buf , "%d\n" , smarthouse_struct.temp_1_threshold_low_action[cnt]);
        fputs(write_buf , fd);
    }
   
    //TEMP 2 THRESHOLD
    sprintf(write_buf , "temp_2_threshold_high\n%f\ntemp_2_threshold_low\n%f\n" , smarthouse_struct.temp_2_threshold_high , smarthouse_struct.temp_2_threshold_low);
    fputs(write_buf , fd);
    
    // TEMP 2 THRESHOLD HIGH ACTION
    sprintf(write_buf , "temp_2_threshold_high_action\n");
    fputs(write_buf , fd);
    for(cnt = 0 ; cnt < sizeof(smarthouse_struct.temp_2_threshold_high_action) ; cnt++)
    {
        sprintf(write_buf , "%d\n" , smarthouse_struct.temp_2_threshold_high_action[cnt]);
        fputs(write_buf , fd);
    }

    // TEMP 2 THRESHOLD LOW ACTION
    sprintf(write_buf , "temp_2_threshold_low_action\n");
    fputs(write_buf , fd);
    for(cnt = 0 ; cnt < sizeof(smarthouse_struct.temp_2_threshold_low_action) ; cnt++)
    {
        sprintf(write_buf , "%d\n" , smarthouse_struct.temp_2_threshold_low_action[cnt]);
        fputs(write_buf , fd);
    }
    
    //LIGHT THRESHOLD 
    sprintf(write_buf , "light_threshold_high\n%d\nlight_threshold_low\n%d\n" , smarthouse_struct.light_threshold_high , smarthouse_struct.light_threshold_low);
    fputs(write_buf , fd);
   
    //LIGHT THRESHOLD high ACTION 
    sprintf(write_buf , "light_threshold_high_action\n");
    fputs(write_buf , fd);
    for(cnt = 0 ; cnt < sizeof(smarthouse_struct.light_threshold_high_action) ; cnt++)
    {
        sprintf(write_buf , "%d\n" , smarthouse_struct.light_threshold_high_action[cnt]);
        fputs(write_buf , fd);
    }

    //LIGHT THRESHOLD LOW ACTION 
    sprintf(write_buf , "light_threshold_low_action\n");
    fputs(write_buf , fd);
    for(cnt = 0 ; cnt < sizeof(smarthouse_struct.light_threshold_low_action) ; cnt++)
    {
        sprintf(write_buf , "%d\n" , smarthouse_struct.light_threshold_low_action[cnt]);
        fputs(write_buf , fd);
    }
    
    // MOTION SENSOR 
    sprintf(write_buf , "motion_threshold_action\n");
    fputs(write_buf , fd);
    for(cnt = 0 ; cnt < sizeof(smarthouse_struct.motion_threshold_action) ; cnt++)
    {
        sprintf(write_buf , "%d\n" , smarthouse_struct.motion_threshold_action[cnt]);
        fputs(write_buf , fd);
    }
    
    // MOTION (LAST MOVE)
    sprintf(write_buf , "last_move\n%s\n" , smarthouse_struct.last_move);
    fputs(write_buf , fd);
    
    // BUTTON
    sprintf(write_buf , "button_action\n");
    fputs(write_buf , fd);
    for(cnt = 0 ; cnt < sizeof(smarthouse_struct.button_action) ; cnt++)
    {
        sprintf(write_buf , "%d\n" , smarthouse_struct.button_action[cnt]);
        fputs(write_buf , fd);
    }
    
    //WETNESS SENSOR
    sprintf(write_buf , "wetness_threshold_action\n");
    fputs(write_buf , fd);
    for(cnt = 0 ; cnt < sizeof(smarthouse_struct.wetness_threshold_action) ; cnt++)
    {
        sprintf(write_buf , "%d\n" , smarthouse_struct.wetness_threshold_action[cnt]);
        fputs(write_buf , fd);
    }
    
    
    // >>>>>>>>>>>>>>>> ACTIVATE VARIABLES
    
    // TEMP SENSORS
    sprintf(write_buf , "temp_1_activate\n%d\n" , smarthouse_struct.temp_1_activate);
    fputs(write_buf , fd);
    
    sprintf(write_buf , "temp_2_activate\n%d\n" , smarthouse_struct.temp_2_activate);
    fputs(write_buf , fd);
    
    // LIGHT SENSOR
    sprintf(write_buf , "light_sensor_activate\n%d\n" , smarthouse_struct.light_sensor_activate);
    fputs(write_buf , fd);
    
    //MOTION SENSOR
    sprintf(write_buf , "motion_sensor_activate\n%d\n" , smarthouse_struct.motion_sensor_activate);
    fputs(write_buf , fd);
    
    //WETNESS SENSOR
    sprintf(write_buf , "wetness_sensor_activate\n%d\n" , smarthouse_struct.wetness_sensor_activate);
    fputs(write_buf , fd);
    
    printf("$ Save new conf in config file\n");
    
    //Close config File
    fclose(fd);
        
    free(write_buf);
}


/* 
 * Func:  read_config
 * Description: Read configruation of system
 */
void read_config()
{
    FILE *fd;
    char* write_buf;
    size_t len =0;
    uint8_t cnt = 0;
    
    write_buf = malloc(100);
    
    //OPEN FILE
    fd = fopen("/home/pi/SmartHouse/config_mk" , "r" );
    
    getline(&write_buf , &len , fd); //READ HEADER (ITS USELESS)
    getline(&write_buf , &len , fd); //READ NEXT LINE
    
    
    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> TEMP 1
    // READ temp_1_threshold_high
    if(strcmp(write_buf , "temp_1_threshold_high\n") == 0)
    {
        getline(&write_buf , &len , fd); //READ NEXT LINE
        smarthouse_struct.temp_1_threshold_high = (float) atoll(write_buf);
        printf("temp_1_threshold_high = %f\n" , smarthouse_struct.temp_1_threshold_high);
    }
    else printf("Can't read temp_1_threshold_high\n");
    
    memset(write_buf , 0 , len);
    getline(&write_buf , &len , fd); //READ NEXT LINE
    // READ temp_1_threshold_low
    if(strcmp(write_buf , "temp_1_threshold_low\n") == 0)
    {
        getline(&write_buf , &len , fd); //READ NEXT LINE
        smarthouse_struct.temp_1_threshold_low = (float) atoll(write_buf);
        printf("temp_1_threshold_low = %f\n" , smarthouse_struct.temp_1_threshold_low);
    }
    else printf("Can't read temp_1_threshold_low\n");
    
    // READ temp_1_threshold_high_action
    memset(write_buf , 0 , len);
    getline(&write_buf , &len , fd); //READ NEXT LINE
    if(strcmp(write_buf , "temp_1_threshold_high_action\n") == 0)
    {
        for(cnt =0 ; cnt < sizeof(smarthouse_struct.light_threshold_high_action) ; cnt++)
        {
            getline(&write_buf , &len , fd); //READ NEXT LINE
            smarthouse_struct.temp_1_threshold_high_action[cnt] = (uint8_t) atoi(write_buf);  
            printf("temp_1_threshold_high_action[%d] = %d\n" , cnt , smarthouse_struct.temp_1_threshold_high_action[cnt]);
            memset(write_buf , 0 , len);
        }
    }
    else printf("Can't read temp_1_threshold_high_action\n");
    
    // READ temp_1_threshold_low_action
    memset(write_buf , 0 , len);
    getline(&write_buf , &len , fd); //READ NEXT LINE
    if(strcmp(write_buf , "temp_1_threshold_low_action\n") == 0)
    {
        for(cnt =0 ; cnt < sizeof(smarthouse_struct.temp_1_threshold_low_action) ; cnt++)
        {
            getline(&write_buf , &len , fd); //READ NEXT LINE
            smarthouse_struct.temp_1_threshold_low_action[cnt] = (uint8_t) atoi(write_buf); 
            printf("temp_1_threshold_low_action[%d] = %d\n" , cnt , smarthouse_struct.temp_1_threshold_low_action[cnt]);
            memset(write_buf , 0 , len);
        }
    }
    else printf("Can't read temp_1_threshold_low_action\n");
    
    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> TEMP 2
    // READ temp_2_threshold_high
    memset(write_buf , 0 , len);
    getline(&write_buf , &len , fd); //READ NEXT LINE
    if(strcmp(write_buf , "temp_2_threshold_high\n") == 0)
    {
        getline(&write_buf , &len , fd); //READ NEXT LINE
        smarthouse_struct.temp_2_threshold_high = (float) atoll(write_buf);
        printf("temp_2_threshold_high = %f\n" , smarthouse_struct.temp_2_threshold_high);
    }
    else printf("Can't read temp_2_threshold_high\n");
    
    memset(write_buf , 0 , len);
    getline(&write_buf , &len , fd); //READ NEXT LINE
    // READ temp_2_threshold_low
    if(strcmp(write_buf , "temp_2_threshold_low\n") == 0)
    {
        getline(&write_buf , &len , fd); //READ NEXT LINE
        smarthouse_struct.temp_2_threshold_low = (float) atoll(write_buf);
        printf("temp_2_threshold_low = %f\n" , smarthouse_struct.temp_2_threshold_low);
    }
    else printf("Can't read temp_2_threshold_low\n");
    
    // READ temp_2_threshold_high_action
    memset(write_buf , 0 , len);
    getline(&write_buf , &len , fd); //READ NEXT LINE
    if(strcmp(write_buf , "temp_2_threshold_high_action\n") == 0)
    {
        for(cnt =0 ; cnt < sizeof(smarthouse_struct.light_threshold_high_action) ; cnt++)
        {
            getline(&write_buf , &len , fd); //READ NEXT LINE
            smarthouse_struct.temp_2_threshold_high_action[cnt] = (uint8_t) atoi(write_buf);  
            printf("temp_2_threshold_high_action[%d] = %d\n" , cnt , smarthouse_struct.temp_2_threshold_high_action[cnt]);
            memset(write_buf , 0 , len);
        }
    }
    else printf("Can't read temp_2_threshold_high_action\n");
    
    // READ temp_2_threshold_low_action
    memset(write_buf , 0 , len);
    getline(&write_buf , &len , fd); //READ NEXT LINE
    if(strcmp(write_buf , "temp_2_threshold_low_action\n") == 0)
    {
        for(cnt =0 ; cnt < sizeof(smarthouse_struct.temp_2_threshold_low_action) ; cnt++)
        {
            getline(&write_buf , &len , fd); //READ NEXT LINE
            smarthouse_struct.temp_2_threshold_low_action[cnt] = (uint8_t) atoi(write_buf); 
            printf("temp_2_threshold_low_action[%d] = %d\n" , cnt , smarthouse_struct.temp_2_threshold_low_action[cnt]);
            memset(write_buf , 0 , len);
        }
    }
    else printf("Can't read temp_2_threshold_low_action\n");
    
    
    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>> LIGHT SENSOR
     memset(write_buf , 0 , len);
     getline(&write_buf , &len , fd); //READ NEXT LINE
    // READ light_threshold_high
    if(strcmp(write_buf , "light_threshold_high\n") == 0)
    {
        getline(&write_buf , &len , fd); //READ NEXT LINE
        smarthouse_struct.light_threshold_high = (uint8_t) atoi(write_buf);
        printf("light_threshold_high = %d\n" , smarthouse_struct.light_threshold_high);
    }
    else printf("Can't read light_threshold_high\n");
    
     memset(write_buf , 0 , len);
     getline(&write_buf , &len , fd); //READ NEXT LINE
     
    // READ light_threshold_low
    if(strcmp(write_buf , "light_threshold_low\n") == 0)
    {
        getline(&write_buf , &len , fd); //READ NEXT LINE
        smarthouse_struct.light_threshold_low = (uint8_t) atoi(write_buf);
        printf("light_threshold_low = %d\n" , smarthouse_struct.light_threshold_low);
    }
    else printf("Can't read light_threshold_low\n");
     
     // READ light_threshold_high_action
    memset(write_buf , 0 , len);
    getline(&write_buf , &len , fd); //READ NEXT LINE
    if(strcmp(write_buf , "light_threshold_high_action\n") == 0)
    {
        for(cnt =0 ; cnt < sizeof(smarthouse_struct.light_threshold_high_action) ; cnt++)
        {
            getline(&write_buf , &len , fd); //READ NEXT LINE
            smarthouse_struct.light_threshold_high_action[cnt] = (uint8_t) atoi(write_buf); 
            printf("light_threshold_high_action[%d] = %d\n" , cnt , smarthouse_struct.light_threshold_high_action[cnt]);
            memset(write_buf , 0 , len);
        }
    }
    else printf("Can't faind light_threshold_high_action\n");
   
     // READ light_threshold_low_action
    memset(write_buf , 0 , len);
    getline(&write_buf , &len , fd); //READ NEXT LINE
    if(strcmp(write_buf , "light_threshold_low_action\n") == 0)
    {
        for(cnt =0 ; cnt < sizeof(smarthouse_struct.light_threshold_low_action) ; cnt++)
        {
            getline(&write_buf , &len , fd); //READ NEXT LINE
            smarthouse_struct.light_threshold_low_action[cnt] = (uint8_t) atoi(write_buf); 
            printf("light_threshold_low_action[%d] = %d\n" , cnt , smarthouse_struct.light_threshold_low_action[cnt]);
            memset(write_buf , 0 , len);
        }
    }
    else printf("Can't faind light_threshold_low_action\n");
    
    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> MOTION SENSOR
    
     // READ motion_threshold_action
    memset(write_buf , 0 , len);
    getline(&write_buf , &len , fd); //READ NEXT LINE
    if(strcmp(write_buf , "motion_threshold_action\n") == 0)
    {
        for(cnt =0 ; cnt < sizeof(smarthouse_struct.motion_threshold_action) ; cnt++)
        {
            getline(&write_buf , &len , fd); //READ NEXT LINE
            smarthouse_struct.motion_threshold_action[cnt] = (uint8_t) atoi(write_buf); 
            printf("motion_threshold_action[%d] = %d\n" , cnt , smarthouse_struct.motion_threshold_action[cnt]);
            memset(write_buf , 0 , len);
        }
    }
    else printf("Can't faind motion_threshold_action\n");
    
    memset(write_buf , 0 , len);
    getline(&write_buf , &len , fd); //READ NEXT LINE
    if(strcmp(write_buf , "last_move\n") == 0)
    {
        getline(smarthouse_struct.last_move , &len , fd);
    }
    else printf("Can't faind last_move buf\n");
    
    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> BUTTON
    
     // READ button_action
    memset(write_buf , 0 , len);
    getline(&write_buf , &len , fd); //READ NEXT LINE
    if(strcmp(write_buf , "button_action\n") == 0)
    {
        for(cnt =0 ; cnt < sizeof(smarthouse_struct.button_action) ; cnt++)
        {
            getline(&write_buf , &len , fd); //READ NEXT LINE
            smarthouse_struct.button_action[cnt] = (uint8_t) atoi(write_buf); 
            printf("button_action[%d] = %d\n" , cnt , smarthouse_struct.button_action[cnt]);
            memset(write_buf , 0 , len);
        }
    }
    else printf("Can't faind button_action\n");
    
    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> WETNESS SENSOR
    
     // READ button_action
    memset(write_buf , 0 , len);
    getline(&write_buf , &len , fd); //READ NEXT LINE
    if(strcmp(write_buf , "wetness_threshold_action\n") == 0)
    {
        for(cnt =0 ; cnt < sizeof(smarthouse_struct.wetness_threshold_action) ; cnt++)
        {
            getline(&write_buf , &len , fd); //READ NEXT LINE
            smarthouse_struct.wetness_threshold_action[cnt] = (uint8_t) atoi(write_buf); 
            printf("wetness_threshold_action[%d] = %d\n" , cnt , smarthouse_struct.wetness_threshold_action[cnt]);
            memset(write_buf , 0 , len);
        }
    }
    else printf("Can't find wetness_threshold_action\n");
    
    // >>>>>>>>>>>>>>>> ACTIVATE VARIABLES
    
    //TEMP 1 ACTIVATE
    memset(write_buf , 0 , len);
    getline(&write_buf , &len , fd); //READ NEXT LINE
    
    if(strcmp(write_buf , "temp_1_activate\n") == 0)
    {
        getline(&write_buf , &len , fd); //READ NEXT LINE
        smarthouse_struct.temp_1_activate = (uint8_t) atoi(write_buf);
        printf("temp_1_activate = %d\n" , smarthouse_struct.temp_1_activate);
        memset(write_buf , 0 , len);
    }
    
    
    // TEMP 2 ACTIVATE
    getline(&write_buf , &len , fd); //READ NEXT LINE
    
    if(strcmp(write_buf , "temp_2_activate\n") == 0)
    {
        getline(&write_buf , &len , fd); //READ NEXT LINE
        smarthouse_struct.temp_2_activate = (uint8_t) atoi(write_buf);
        printf("temp_2_activate = %d\n" , smarthouse_struct.temp_2_activate);
        memset(write_buf , 0 , len);
    }
    
    // LIGHT ACTIVATE
    getline(&write_buf , &len , fd); //READ NEXT LINE
    
    if(strcmp(write_buf , "light_sensor_activate\n") == 0)
    {
        getline(&write_buf , &len , fd); //READ NEXT LINE
        smarthouse_struct.light_sensor_activate = (uint8_t) atoi(write_buf);
        printf("light_sensor_activate = %d\n" , smarthouse_struct.light_sensor_activate);
        memset(write_buf , 0 , len);
    }
    
    // MOTION ACTIVATE
    getline(&write_buf , &len , fd); //READ NEXT LINE
    
    if(strcmp(write_buf , "motion_sensor_activate\n") == 0)
    {
        getline(&write_buf , &len , fd); //READ NEXT LINE
        smarthouse_struct.motion_sensor_activate = (uint8_t) atoi(write_buf);
        printf("motion_sensor_activate = %d\n" , smarthouse_struct.motion_sensor_activate);
        memset(write_buf , 0 , len);
    }
    
    // WETNESS ACTIVATE
    
    getline(&write_buf , &len , fd); //READ NEXT LINE
    
    if(strcmp(write_buf , "wetness_sensor_activate\n") == 0)
    {
        getline(&write_buf , &len , fd); //READ NEXT LINE
        smarthouse_struct.wetness_sensor_activate = (uint8_t) atoi(write_buf);
        printf("wetness_sensor_activate = %d\n" , smarthouse_struct.wetness_sensor_activate);
        memset(write_buf , 0 , len);
    }
    
    free(write_buf);
    fclose(fd);
}


/* 
 * Func:  default_config
 * Description: Set default configuration of system
 */
int8_t default_config()
{ 
    uint8_t cnt = 0;
    
    if(access("/home/pi/SmartHouse/config_mk" , F_OK ) != -1)
    {
        printf("$ Config file found \n");       
        read_config();
    }
    else
    {

        printf("Config file dosen't exist \n");

        // >>>>>>>>>> SET UP DEFAULT VARIABLES
        
        //TEMP 1 
        smarthouse_struct.temp_1_threshold_high = 22.0;                     //SET HIGH THRESHOULF tO 25 deg Celcious
        smarthouse_struct.temp_1_threshold_low = 20.0;                      //SET LOW THRESHOULD to 17 deg Celcious
        
        // THRESHOLD HIGH ACTION
        smarthouse_struct.temp_1_threshold_high_action[0] = RELAY_1_ON;    //SET RELAY 1 ON  
        for(cnt = 1 ; cnt < sizeof(smarthouse_struct.temp_1_threshold_high_action) ; cnt++) //SET REST ACTION TO NULL
        {
           smarthouse_struct.temp_1_threshold_high_action[cnt] = 0;
        }
        // THRESHOLD LOW ACTION
        smarthouse_struct.temp_1_threshold_low_action[0] = RELAY_1_OFF;    //SET RELAY 1 OFF
        for(cnt = 1 ; cnt < sizeof(smarthouse_struct.temp_1_threshold_low_action) ; cnt++) //SET REST ACTION TO NULL
        {
           smarthouse_struct.temp_1_threshold_low_action[cnt] = 0;
        }   
        //TEMP 2 
        smarthouse_struct.temp_2_threshold_high = 22.0;                     //SET HIGH THRESHOULF tO 25 deg Celcious
        smarthouse_struct.temp_2_threshold_low = 20.0;                      //SET LOW THRESHOULD to 17 deg Celcious

        //THRESHOLD HIGH ACTION!
        smarthouse_struct.temp_2_threshold_high_action[0] = RELAY_2_ON;    //SET RELAY 2 ON   
        for(cnt = 1 ; cnt < sizeof(smarthouse_struct.temp_2_threshold_high_action) ; cnt++) //SET REST ACTION TO NULL
        {
           smarthouse_struct.temp_2_threshold_high_action[cnt] = 0;
        }

        // THRESHOLD LOW ACTION
        smarthouse_struct.temp_2_threshold_low_action[0] = RELAY_2_OFF;    //SET RELAY 2 OFF
        for(cnt = 1 ; cnt < sizeof(smarthouse_struct.temp_2_threshold_low_action) ; cnt++) //SET REST ACTION TO NULL
        {
           smarthouse_struct.temp_2_threshold_low_action[cnt] = 0;
        }
        
        //TEMP 1 i TEMP 2 activate
        smarthouse_struct.temp_1_activate = 1;                              //ACTIVATE TEMP 1
        smarthouse_struct.temp_2_activate = 1;                              //ACTIVATE TEMP 2
        
        //LIGHT SENSOR
        smarthouse_struct.light_threshold_high = 10;                       //SET HIGH THRESHOULD to 35 LUX
        smarthouse_struct.light_threshold_low = 5;                         //SET LOW LIGHT THRESHOULD to 5 LUX
                
        smarthouse_struct.light_threshold_high_action[0] = RELAY_3_OFF;    //SET REALY 3 OFF
        
        for(cnt = 1 ; cnt < sizeof(smarthouse_struct.light_threshold_high_action) ; cnt++) //SET REST ACTION TO NULL
        {
           smarthouse_struct.light_threshold_high_action[cnt] = 0;
        }
        
        smarthouse_struct.light_threshold_low_action[0] = RELAY_3_ON;      //SET RELAY 3 ON
        
        for(cnt = 1 ; cnt < sizeof(smarthouse_struct.light_threshold_low_action) ; cnt++) //SET REST ACTION TO NULL
        {
           smarthouse_struct.light_threshold_low_action[cnt] = 0;
        }
        
        smarthouse_struct.light_sensor_activate = 1;                        //ACTIVATE LIGHT SENSOR
        
        //BUTTON ACTION
        for(cnt = 0 ; cnt < sizeof(smarthouse_struct.button_action) ; cnt++)    //SET ALLACTION TO NULL
        {
           smarthouse_struct.button_action[cnt] = 0;
        }
        
        //MOTION SENSOR
        
        for(cnt = 0 ; cnt < sizeof(smarthouse_struct.motion_threshold_action) ; cnt++)    //SET ALL ACTION TO NULL
        {
           smarthouse_struct.motion_threshold_action[cnt] = 0;
        }
        
        sprintf(smarthouse_struct.last_move , "NO DATA");
        
        smarthouse_struct.motion_sensor_activate = 0;                                      //MOTION SENSOR DDESACTIVATE ON START
        
        //WETNESS SENSOR
        for(cnt = 0 ; cnt < sizeof(smarthouse_struct.wetness_threshold_action) ; cnt++)    //SET ALL ACTION TO NULL
        {
           smarthouse_struct.wetness_threshold_action[cnt] = 0;
        }
        
        smarthouse_struct.wetness_sensor_activate = 0;                              // DESactivate wetness sensor on start
        
        // SAVE CONFIG TO FILE
        save_config();

    }

}

//END OF FILE
