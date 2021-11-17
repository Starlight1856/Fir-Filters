#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include <errno.h>
#include <string.h>


#include "fir.h"

//#define STORE_TO_FILE

const int SIGNAL_LENGTH = 1000;

void signal_gen(double* pdata);
uint64_t current_timestamp(void);

int main()
{
    double signal[SIGNAL_LENGTH]; // test signal
    double op;
    uint64_t start_time, stop_time;
    int i;


    int filtersize = cfilt_filesetup( "lpfircoeff.txt" );
    printf("Filter size =%d\n", filtersize);
    if( filtersize == -1) return 0;

    signal_gen( &signal[0]);

    #ifdef STORE_TO_FILE
    FILE* op1;
    op1 = fopen("/tmp/data1.txt", "w");
    if( op1 == NULL )
      {
      printf("The error is - %s\n", strerror(errno));
      printf("Error opening file cannot proceed\n");
      return 0;
      }
     #endif
    
    start_time = current_timestamp();
    for( i =0; i < SIGNAL_LENGTH; i++)
      {
      cfilt_filter( &signal[i], &op);
      #ifdef STORE_TO_FILE
        fprintf( op1, "%f  %f\n",op, signal[i]); //
      #endif  
      }
    stop_time = current_timestamp();

    printf("Elasped time = %ld\n", stop_time- start_time);

    #ifdef STORE_TO_FILE
    fclose(op1);
    #endif

    cfilt_reset_memory(); //** free memory
    printf("Finished!  %d  %f\n", i, op);
    return 0;
}

void signal_gen(double* pdata)
{
    int i,k;
    double value;

    k=0.0;

    //** test rate = 1000 SPS
    //** 10Hz main signal modulated by 100Hz and 200Hz @ 10% amplitude
    for( i=0; i < SIGNAL_LENGTH; i++ )
      {
      value = 8192 - (16384 * sin( (2*M_PI) * (k/100.0) ) )+ (1638 * sin( (2*M_PI) * (k/10.0) )+ (1638 * sin( (2*M_PI) * (k/5.0) )) );
    
      pdata[i] = value;
    
      k++;
      }
}


uint64_t current_timestamp(void)
{
    struct timeval te;
    gettimeofday(&te, NULL); // get current time
    uint64_t  milliseconds = te.tv_sec*1000LL + te.tv_usec/1000; // calculate milliseconds
    // printf("milliseconds: %lld\n", milliseconds);
    return milliseconds;
}
