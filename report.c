/* 
 * File:   report.c
 * Author: mkruk
 *
 * Created on 15 stycznia 2018, 16:15
 */


#include "report.h"
/* 
 * Func:   save_report()
 * Description: Function save report to file
 */
void save_report(char *report_buf)
{
    //Variables for files operation
    FILE *fd;
    char* write_buf;
    uint8_t cnt = 0;
    
    //Variables for time 
    struct tm *time_mk;
    time_t ti;

    //Reserve memory for bufor
    write_buf = malloc(200);
    
    //OPEN FILE
    fd = fopen("/home/pi/SmartHouse/report_mk" , "a" );         // Atribute "a" means: open for append
    
    //Get Time
    time(&ti);
    time_mk = localtime(&ti);
    
    //Set report content
    sprintf(write_buf , "Date: %d.%d.%d Time: %d:%d:%d %s\n" , time_mk->tm_mday , time_mk->tm_mon + 1 , time_mk->tm_year + 1900 , time_mk->tm_hour + 1, time_mk->tm_min , time_mk->tm_sec, report_buf);
    fputs(write_buf , fd);
    
    free(write_buf);
    fclose(fd);
}