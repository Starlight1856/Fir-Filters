//*****************************************************
//**
//** General Purpose FIR filter C
//**
//** Paul Byrne 9/11/21
//**
//******************************************************


#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>

#include "fir.h"

static int cfilt_allocate_memory(int filter_size);

static const int BUFF_SIZE = 200;
static double* cfilt_p_filter = (double*)NULL;
static double* cfilt_p_filterbuffer = (double*)NULL;

static int cfilt_filtcount =0;
static int cfilt_filtsize =0;


//************************************************************************
//**
//** cfilt_reset_memory
//**
//** Ensure data memory is deallocated
//**
//************************************************************************
void cfilt_reset_memory(void)
{

 cfilt_filtcount =0;
 cfilt_filtsize = 0;
 if( cfilt_p_filter != (double*)NULL) free(cfilt_p_filter) ;
 if( cfilt_p_filterbuffer != (double*)NULL) free(cfilt_p_filterbuffer) ;

}
//************************************************************************
//**
//** cfilt_allocate_memory
//**
//** Allocate memory FIR coefficients and filter data buffer.
//**
//** Parameters
//** int filter_size; number of taps in FIR filter.
//**
//************************************************************************

int cfilt_allocate_memory(int filter_size)
{

  cfilt_p_filter = (double*)calloc( cfilt_filtsize*2, sizeof(double));
  cfilt_p_filterbuffer = (double*)calloc( cfilt_filtsize, sizeof(double) );

  if(  (cfilt_p_filter == (double*)NULL)
     ||(cfilt_p_filterbuffer == (double*)NULL) )
     {
     printf("Memory allocation error  cannot proceed\n");
     return -1;
     }

  return 0;
}

//************************************************************************
//**
//** cfilt_filesetup
//**
//** Setup FIR filter from data contained in text file.
//** Note; this expects coefficients generated from python applications.
//** The first line in the file contains the filter specifications and is
//** ignored. If other files are used this must be taken into account.
//**
//** Paramters
//** const char* pfilename; filename of FIR data file.
//**
//**
//************************************************************************

int cfilt_filesetup( const char* pfilename )
{
    FILE* fp;
    char* line;
    int filter_size =0;
    char buff[BUFF_SIZE];
    size_t line_buf_size = 0;
    double value =0;
    int read;
    int j=0;
    bool first_line = false;

    cfilt_reset_memory();

    fp = fopen(pfilename, "r");
    if( fp == NULL )
      {
      printf("The error is - %s\n", strerror(errno));
      printf("Error opening file cannot proceed\n");
      return 0;
      }

     //** parse 1; find number of coefficients
     while ((read = getline(&line, &line_buf_size, fp)) != -1)
        {
       filter_size++;
        }


     //** reset file pointer
     fseek(fp, 0L, SEEK_SET);

     //** parse 2; allocate array for coefficients and load array
     cfilt_filtsize = filter_size-1; // first line is filter spec

     if( cfilt_allocate_memory( cfilt_filtsize ) != 0 )
       {
       return -1;
       }

     //** The first line in the file contains the filter specifications.
     //** The following lines have the filter coefficients.
     while( fgets( buff, sizeof(buff), fp) )
       {
       if( first_line ==  false)
         {
         first_line = true; //** ignore first line
         }
       else
         {
         sscanf(buff, "%lf", &value);
         cfilt_p_filter[j] = value;
         cfilt_p_filter[cfilt_filtsize + j] = value;
         j++;
         }
       memset(buff,(char)0, BUFF_SIZE);
       }

   fclose(fp);
   cfilt_filtcount =0;


  return(filter_size);
}


//************************************************************************
//**
//** cfilt_setup
//**
//** Setup FIR filter from parametes
//**
//** Paramters
//** int filtersize; Number of filter taps
//** double* pfiltercoeff; pointer to array containing filter coefficients
//**
//************************************************************************

int cfilt_setup( int filtersize, double* pfiltercoeff)
{
  int j;


  cfilt_reset_memory();
  
  cfilt_filtsize = filtersize;

  if( cfilt_allocate_memory( cfilt_filtsize ) != 0 )
     {
     return -1;
     }

  for ( j=0; j < cfilt_filtsize; j++ )
    {
    cfilt_p_filter[j] = pfiltercoeff[j];
    cfilt_p_filter[cfilt_filtsize + j] = pfiltercoeff[j];
    }

  cfilt_filtcount =0;

  return(cfilt_filtsize);
}

//*******************************************************
//**
//** cfilt_filter
//**
//** Filter data
//**
//** Paramters
//**  double* pIpVal; pointer to input data sample
//**  double* pOpVa; pointer to filter output sample
//**
//*******************************************************
//** FIR by circular buffers
void cfilt_filter(double* pIpVal, double* pOpVal)
{

  int i;

  cfilt_p_filterbuffer[cfilt_filtcount] = *pIpVal; //** load from beginning of buffer

  *pOpVal  = 0.0;

  for( i=0; i < cfilt_filtsize; i++ )
    {
    *pOpVal += ( cfilt_p_filterbuffer[i] * cfilt_p_filter[ cfilt_filtsize - i + cfilt_filtcount] );
    }

  //** Operates as circular buffer
  cfilt_filtcount++;

  if (cfilt_filtcount == cfilt_filtsize) cfilt_filtcount = 0 ;

  }


