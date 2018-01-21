/* 
 * File:   temperature.h
 * Author: mkruk
 *
 * Created on 16 listopada 2017, 17:49
 */
#include "temperature.h"

extern volatile struct smarthouse_config smarthouse_struct;
volatile uint8_t temp_1_threshold_high_cnt;
volatile uint8_t temp_1_threshold_low_cnt;

volatile uint8_t temp_2_threshold_high_cnt;
volatile uint8_t temp_2_threshold_low_cnt;

uint8_t local_cnt;

//Variables
    DIR *dir;
    struct dirent *dirent;
    char dev[3][16];      // Dev ID

    char devPath_0[128]; // Path to device
    char devPath_1[128]; // Path to device
    
    char buf[256];     // Data from device
    char tmpData[6];   // Temp C * 1000 reported by device 
    char path[] = "/sys/bus/w1/devices"; 
    ssize_t numRead;

 /* 
 * Func:  temp_init_mk
 * Description: Init temperature sensors
 */
int8_t temp_init_mk(void)
{
    // Variables for main Thread
    pthread_t temp_thread_ID; 
    uint8_t thread_create_status;
    
    dir = opendir (path);
    
    printf("\n --> Init temperature sensors <--\n");
    
    if (dir != NULL)
    {
        int i = 0;
        
        while ((dirent = readdir (dir)))
        {
            // 1-wire devices are links beginning with 28-
            if (dirent->d_type == DT_LNK && strstr(dirent->d_name, "28-") != NULL) 
            { 
                strcpy(dev[i], dirent->d_name);
                printf("$ Device: %s\n", dev[i]);
              
                // Assemble path to OneWire device
                i++;
            }
            
        }
              
    (void) closedir (dir);
    }
    else
    {
     perror ("$ Couldn't open the w1 devices directory\n");
     return 1;
    }

   sprintf(devPath_0, "%s/%s/w1_slave", path, dev[0]); 
   sprintf(devPath_1, "%s/%s/w1_slave", path, dev[1]); 
   
   //printf(devPath_0);
   //printf(devPath_1);
   
   // >>>>>>>>> Create temp thread
    thread_create_status = pthread_create(&temp_thread_ID , NULL , temp_thread , NULL);  
    printf("$ temp_thread create: %d\n" , thread_create_status);
    
    printf("\n --> Init temperature sensors DONE! <--\n");
}

/* 
 * Func:  temp_read_mk
 * Description: Read temperature for all sensors
 */
int temp_read_mk(void)
{
    
        int fd_0 = open(devPath_0 , O_RDONLY);
        
        if(fd_0 == -1 )
        {
            perror ("$ Couldn't open the w1 device_0.");
            return 1;
        }

        while((numRead = read(fd_0, buf, 256)) > 0) 
        {
            strncpy(tmpData, strstr(buf, "t=") + 2, 5); 
            float tempC = strtof(tmpData, NULL);
  
        smarthouse_struct.temp_1_value = tempC / 1000;    
#if PRINT_TEMP_ON_TERMINAL == 1
            printf("$ Device_0: %s  - ", dev[0]); 
            printf("$ Temp: %.3f C  \n", tempC / 1000);
#endif
        }
        close(fd_0);
        
        int fd_1 = open(devPath_1 , O_RDONLY);
        
        if(fd_1 == -1)
        {
            perror ("$ Couldn't open the w1 device_1.\n");
            return 1;
        }
        
        while((numRead = read(fd_1, buf, 256)) > 0) 
        {
            strncpy(tmpData, strstr(buf, "t=") + 2, 5); 
            float tempC = strtof(tmpData, NULL);
            
            smarthouse_struct.temp_2_value = tempC / 1000; 
            
#if PRINT_TEMP_ON_TERMINAL == 1           
            printf("$ Device_1: %s  - ", dev[1]); 
            printf("$ Temp: %.3f C  \n\n", tempC / 1000);    
#endif
        }

        close(fd_1);
}


/* 
 * Func:   temp_thread()
 * Description: Temperature thread
 */
void* temp_thread(void *arg)
{

    while(1)
    {
        
        temp_read_mk();   //Read temperature
        
        if(smarthouse_struct.temp_1_activate)
        {
            //Check threshold value
            if(smarthouse_struct.temp_1_value > smarthouse_struct.temp_1_threshold_high)
            {
                temp_1_threshold_low_cnt = 0;

                temp_1_threshold_high_cnt++;
                if(temp_1_threshold_high_cnt > 20) temp_1_threshold_high_cnt = 2;

                if(temp_1_threshold_high_cnt == 1)
                {
                    //ACTION
                    save_report("TEMP_1 Threshold HIGH");

                    local_cnt = 0;     
                    while(smarthouse_struct.temp_1_threshold_high_action[local_cnt] != 0)
                    {
                        action(smarthouse_struct.temp_1_threshold_high_action[local_cnt]);
                        local_cnt++;
                    }

                }
            }
            else if(smarthouse_struct.temp_1_value < smarthouse_struct.temp_1_threshold_low)
            {
                temp_1_threshold_high_cnt = 0;

                temp_1_threshold_low_cnt++;
                if(temp_1_threshold_low_cnt > 20) temp_1_threshold_low_cnt = 2;

                if(temp_1_threshold_low_cnt == 1)
                {
                    //ACTION
                    save_report("TEMP_1 Threshold LOW");

                    local_cnt = 0;     
                    while(smarthouse_struct.temp_1_threshold_low_action[local_cnt] != 0)
                    {
                        action(smarthouse_struct.temp_1_threshold_low_action[local_cnt]);
                        local_cnt++;
                    }
                }
            }
         }
    
    
        if(smarthouse_struct.temp_2_activate)
        {
            // >>>>>>>>>>>> TMEP 2
            if(smarthouse_struct.temp_2_value > smarthouse_struct.temp_2_threshold_high)
            {
                temp_2_threshold_low_cnt = 0;

                temp_2_threshold_high_cnt++;
                if(temp_2_threshold_high_cnt > 20) temp_2_threshold_high_cnt = 2;

                if(temp_2_threshold_high_cnt == 1)
                {
                    //ACTION
                    save_report("TEMP_2 Threshold high");

                    local_cnt = 0;     
                    while(smarthouse_struct.temp_2_threshold_high_action[local_cnt] != 0)
                    {
                        action(smarthouse_struct.temp_2_threshold_high_action[local_cnt]);
                        local_cnt++;
                    }

                }
            }
            else if(smarthouse_struct.temp_2_value < smarthouse_struct.temp_2_threshold_low)
            {
                temp_2_threshold_high_cnt = 0;

                temp_2_threshold_low_cnt++;
                if(temp_2_threshold_low_cnt > 20) temp_2_threshold_low_cnt = 2;

                if(temp_2_threshold_low_cnt == 1)
                {
                    //ACTION
                    save_report("TEMP_2 Threshold low");

                    local_cnt = 0;     
                    while(smarthouse_struct.temp_2_threshold_low_action[local_cnt] != 0)
                    {
                        action(smarthouse_struct.temp_2_threshold_low_action[local_cnt]);
                        local_cnt++;
                    }
                }
            }
        }
        
        usleep(TMEP_THREAD_SLEEP_uS);
    }
}
