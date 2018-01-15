
/* 
 * File:   light.h
 * Author: mkruk
 *
 * Created on 12 grudnia 2017, 19:38
 */

#include "light.h"

// GLOBAL CONFIG STRUCTURE
extern volatile struct smarthouse_config smarthouse_struct;


 // >>>>>>>>>> VARIABLS
int rc;
uint16_t broadband, ir;
uint32_t lux = 0;

uint8_t light_local_cnt;
uint8_t light_threshold_low_cnt;
uint8_t light_threshold_high_cnt;

/* 
 * Func:   read_lux()
 * Description: Read lux
 */
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

/* 
 * Func:   init_TSL2561
 * Description: init light sensor
 */
uint32_t init_TSL2561(void)
{
    printf("\n --> Init light sensors <--\n");
    
    // Variables for main Thread
    pthread_t lux_thread_ID; 
    uint8_t thread_create_status;
    
    // >>>>>>>>> Create main thread
    thread_create_status = pthread_create(&lux_thread_ID , NULL , lux_thread , NULL);  
    printf("$ lux_thread create: %d\n" , thread_create_status);
    
    printf("\n --> Init light sensors DONE! <--\n");
    
    return 0;
}

/* 
 * Func:   lux_thread
 * Description: lux_thread
 */
void* lux_thread(void *arg)
{
    while(1)
    {     
        // Read lux 
       smarthouse_struct.light_value = read_lux();
       
       //Check threshold
       if(smarthouse_struct.light_value > smarthouse_struct.light_threshold_high)
        {
            light_threshold_low_cnt = 0;
            
             light_threshold_high_cnt++;
            if(light_threshold_high_cnt > 20)  light_threshold_high_cnt = 2;
            
            if(light_threshold_high_cnt == 1)
            {
                //ACTION      
                save_report("Light Threshold HIGH");
                light_local_cnt = 0;     
                while(smarthouse_struct.light_threshold_high_action[light_local_cnt] != 0)
                {
                    action(smarthouse_struct.light_threshold_high_action[light_local_cnt]);
                    light_local_cnt++;
                }
                
            }
        }
       else if(smarthouse_struct.light_value < smarthouse_struct.light_threshold_low)
       {
           light_threshold_high_cnt = 0;
            
             light_threshold_low_cnt++;
            if(light_threshold_low_cnt > 20)  light_threshold_low_cnt = 2;
            
            if(light_threshold_low_cnt == 1)
            {
                //ACTION      
                save_report("Light Threshold LOW");
                light_local_cnt = 0;     
                
                while(smarthouse_struct.light_threshold_low_action[light_local_cnt] != 0)
                {
                    action(smarthouse_struct.light_threshold_low_action[light_local_cnt]);
                    light_local_cnt++;
                }
                
            }
       }
        
#if PRINT_LUX_ON_TERMINAL == 1
        printf("$ Lux = %d \n" , read_lux());
#endif

        usleep(LIGHT_THREAD_SLEEP_uS);
    }
}
