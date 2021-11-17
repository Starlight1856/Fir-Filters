#include <iostream>
#include <fstream>      // std::ifstream
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include "fir.hpp"

//#define STORE_TO_FILE

using namespace std;


const int SIGNAL_LENGTH = 1000;
void signal_gen(double* pdata);
uint64_t current_timestamp(void);


int main()
{
    double signal[SIGNAL_LENGTH]; // test signal
    uint64_t start_time, stop_time;
    double op;
    int i;

    //** Generate test signal
    signal_gen(signal);

    CLPFilter filt;

    if( filt.FileSetup("lpfircoeff.txt" ) != 0 )
      {
      std::cout << "Error: unable to proceed " << std::endl;
      return 0;
      }

    #ifdef STORE_TO_FILE
    std::ofstream opfile;  
    opfile.open("/tmp/data1.txt", std::ofstream::out);
    #endif
    
    start_time = current_timestamp();
    for( i =0; i< SIGNAL_LENGTH; i++)
      {
      filt.Filter( &signal[i], &op);
      #ifdef STORE_TO_FILE
      opfile << op << "   "<<  signal[i]<<std::endl;
      #endif
      }
      
    stop_time = current_timestamp();
    std::cout << "Elasped time = " << stop_time- start_time << std::endl;;
    
    #ifdef STORE_TO_FILE
    opfile.close();
    #endif
    
    std::cout << "Complete! " << i <<  "  " << op << std::endl;
    return 0;
}

void signal_gen(double* pdata)
{
    int i;
    double value,k;

    k=0.0;

    //** test rate = 1000 SPS
    //** 10Hz main signal modulated by 100Hz and 200Hz @ 10% amplitude
    for(i=0; i<SIGNAL_LENGTH; i++)
      {
      value = 8192 - (16384 * sin( (2*M_PI) * (k/100.0) ) )+ (1638 * sin( (2*M_PI) * (k/10.0) ) + (1638 * sin( (2*M_PI) * (k/5.0) )) );
    
      pdata[i] = value;
      k++;
      }

}

uint64_t current_timestamp(void)
{
    struct timeval te;
    gettimeofday(&te, NULL); // get current time
    uint64_t  milliseconds = te.tv_sec*1000LL + te.tv_usec/1000; // calculate milliseconds
    return milliseconds;
}


