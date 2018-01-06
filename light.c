
/* 
 * File:   light.h
 * Author: mkruk
 *
 * Created on 12 grudnia 2017, 19:38
 */

#include "light.h"


 // >>>>>>>>>> VARIABLS
int rc;
uint16_t broadband, ir;
uint32_t lux = 0;


uint32_t read_lux(void)
{

    TSL2561 light1 = TSL2561_INIT(1, TSL2561_ADDR_FLOAT);
    
    // initialize the sensor
    rc = TSL2561_OPEN(&light1);
    if(rc != 0) {
            fprintf(stderr, "Error initializing TSL2561 sensor (%s). Check your i2c bus (es. i2cdetect)\n", strerror(light1.lasterr));
            // you don't need to TSL2561_CLOSE() if TSL2561_OPEN() failed, but it's safe doing it.
            TSL2561_CLOSE(&light1);
            return 1;
    }
    
    // set the gain to 1X (it can be TSL2561_GAIN_1X or TSL2561_GAIN_16X)
    // use 16X gain to get more precision in dark ambients, or enable auto gain below
    rc = TSL2561_SETGAIN(&light1, TSL2561_GAIN_1X);
    
    // set the integration time 
    // (TSL2561_INTEGRATIONTIME_402MS or TSL2561_INTEGRATIONTIME_101MS or TSL2561_INTEGRATIONTIME_13MS)
    // TSL2561_INTEGRATIONTIME_402MS is slower but more precise, TSL2561_INTEGRATIONTIME_13MS is very fast but not so precise
    rc = TSL2561_SETINTEGRATIONTIME(&light1, TSL2561_INTEGRATIONTIME_101MS);
    
    TSL2561_SENSELIGHT(&light1, &broadband, &ir, &lux, 1);
    

    TSL2561_CLOSE(&light1);
     
    return lux;
}

uint32_t init_TSL2561(void)
{
    // Variables for main Thread
    pthread_t lux_thread_ID; 
    uint8_t thread_create_status;
    
    // >>>>>>>>> Create main thread
    thread_create_status = pthread_create(&lux_thread_ID , NULL , lux_thread , NULL);  
    printf("lux_thread create: %d\n" , thread_create_status);
    
    return 0;
}

void* lux_thread(void *arg)
{
    uint32_t lux;
    while(1)
    {
        printf("Lux = %d \n" , read_lux());
        usleep(2000000);
    }
}
