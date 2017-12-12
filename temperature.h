
/* 
 * File:   temperature.h
 * Author: mkruk
 *
 * Created on 16 listopada 2017, 17:49
 */

#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#ifdef __cplusplus
extern "C" {
#endif

    #include <stdio.h>
    #include <dirent.h>
    #include <string.h>
    #include <fcntl.h>
    #include <stdlib.h>
    #include <unistd.h>

    
    
    //Functions
    int temp_init_mk(void);
    int temp_read_mk(void);
    
#ifdef __cplusplus
}
#endif

#endif /* TEMPERATURE_H */

