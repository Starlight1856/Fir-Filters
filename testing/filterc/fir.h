//*****************************************************
//**
//** General Purpose FIR filter C++
//**
//** Paul Byrne 9/11/21
//**
//******************************************************


#ifndef CFILT_H_   /* Include guard */
#define CFILT_H_

void cfilt_reset_memory(void);
int cfilt_filesetup( const char* pfilename );
int cfilt_setup( int filtersize, double* pfiltercoeff);
void cfilt_filter(double* pIpVal, double* pOpVal);


#endif




