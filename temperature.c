/* 
 * File:   temperature.h
 * Author: mkruk
 *
 * Created on 16 listopada 2017, 17:49
 */
#include "temperature.h"


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

int temp_init_mk(void)
{
    // Variables for main Thread
    pthread_t temp_thread_ID; 
    uint8_t thread_create_status;
    
    dir = opendir (path);
    if (dir != NULL)
    {
        int i = 0;
        
        while ((dirent = readdir (dir)))
        {
            // 1-wire devices are links beginning with 28-
            if (dirent->d_type == DT_LNK && strstr(dirent->d_name, "28-") != NULL) 
            { 
                strcpy(dev[i], dirent->d_name);
                printf("\nDevice: %s\n", dev[i]);
              
                // Assemble path to OneWire device
                i++;
            }
            
        }
              
    (void) closedir (dir);
    }
    else
    {
     perror ("Couldn't open the w1 devices directory");
     return 1;
    }

   sprintf(devPath_0, "%s/%s/w1_slave", path, dev[0]); 
   sprintf(devPath_1, "%s/%s/w1_slave", path, dev[1]); 
   
   printf(devPath_0);
   printf(devPath_1);
   
   // >>>>>>>>> Create temp thread
    thread_create_status = pthread_create(&temp_thread_ID , NULL , temp_thread , NULL);  
    printf("temp_thread create: %d\n" , thread_create_status);
}

int temp_read_mk(void)
{
    while(1)
    {
        int fd_0 = open(devPath_0 , O_RDONLY);
        
        if(fd_0 == -1 )
        {
            perror ("Couldn't open the w1 device_0.");
            return 1;
        }

        while((numRead = read(fd_0, buf, 256)) > 0) 
        {
            strncpy(tmpData, strstr(buf, "t=") + 2, 5); 
            float tempC = strtof(tmpData, NULL);
            
            printf("Device_0: %s  - ", dev[0]); 
            printf("Temp: %.3f C  ", tempC / 1000);
            
        }
        close(fd_0);
        
        int fd_1 = open(devPath_1 , O_RDONLY);
        
        if(fd_1 == -1)
        {
            perror ("Couldn't open the w1 device_1.");
            return 1;
        }
        
        while((numRead = read(fd_1, buf, 256)) > 0) 
        {
            strncpy(tmpData, strstr(buf, "t=") + 2, 5); 
            float tempC = strtof(tmpData, NULL);
            
            printf("Device_1: %s  - ", dev[1]); 
            printf("Temp: %.3f C  \n\n", tempC / 1000);      
        }

        close(fd_1);
    }
}

void* temp_thread(void *arg)
{
    while(1)
    {
        temp_read_mk(); 
        
        usleep(3000000);
    }
}