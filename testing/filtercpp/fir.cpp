//*****************************************************
//**
//** General Purpose FIR filter C++
//** Using arrays
//**
//** Paul Byrne 9/11/21
//**
//******************************************************


#include <iostream>
#include <fstream>      // std::ifstream
#include "fir.hpp"


CLPFilter::CLPFilter()
{

  m_pFilter = (double*)NULL;
  m_pFilterBuffer = (double*)NULL;
  m_FiltCount = 0;
  m_FiltSize =0;
}

CLPFilter::~CLPFilter()
{
  ClearBuffers();
}

//************************************************************************
//**
//** AllocateArrays
//** Allocate data arrays for FIR coefficients and filter data buffer.
//**
//** Parameters
//** int FilterSize; number of taps in FIR filter.
//**
//************************************************************************

int  CLPFilter::AllocateArrays(int FilterSize )
{

  ClearBuffers();

  m_pFilter = new double[FilterSize*2]();
  m_pFilterBuffer = new double[FilterSize]();

  if(  ( m_pFilter == (double*)NULL)
     ||( m_pFilterBuffer == (double*)NULL) )
     {
     std::cout << "Memory allocation error: unable to continue" << std::endl;
     return -1;
     }

  m_FiltSize = FilterSize;

  return 0;
}

//************************************************************************
//**
//** ClearBuffers
//**
//** Ensure data memory is deallocated
//**
//************************************************************************

void CLPFilter::ClearBuffers(void)
{

  if( m_pFilter != (double*)NULL )
      delete [] m_pFilter;

  if( m_pFilterBuffer != (double*)NULL )
      delete [] m_pFilterBuffer;

  m_FiltCount =0;
  m_FiltSize=0;

}

//************************************************************************
//**
//** FileSetup
//**
//** Setup FIR filter from data contained in text file.
//** Note; this expects coefficients generated from python applications.
//** The first line in the file contains the filter specifications and is
//** ignored. If other files are used this must be taken into account.
//**
//** Paramters
//** string sFilterFile; filename of FIR data file.
//**
//**
//************************************************************************


int CLPFilter::FileSetup( std::string sFilterFile)
{
  int j;
  int filter_size =0;
  std::string line;


  ClearBuffers();

  std::ifstream coefffile;
  coefffile.open( sFilterFile, std::ifstream::in);
  if( !coefffile )
    {
    std::cout << "File " << sFilterFile << " Open Error"  << std::endl;
    return -1;
    }

  //** parse 1: find number of coefficients
  if ( coefffile.is_open())
    {
    while ( getline ( coefffile, line) )
      {
      //use line here
      filter_size++;
      }
    //coefffile.close();
    }
  else
    {
    std::cout << "Error: Unable to continue" << std::endl;
    return -1;
    }

  if( AllocateArrays( filter_size ) != 0 )
     {
     std::cout << "Memory allocation error: unable to continue" << std::endl;
     return -1;
     }
  //** parse 2: load filter coefficinets
  //** rewind
  coefffile.clear();
  coefffile.seekg(0, coefffile.beg);

  j=0;
  if ( coefffile.is_open())
    {
    getline ( coefffile, line); //** consume first line with filter specs

    //** continue to load filter coefficients
    while ( getline ( coefffile, line) )
      {
      //use line here
      m_pFilter[j] = stod(line);
      m_pFilter[ filter_size + j] = m_pFilter[j];
      j++;
      }
    coefffile.close();
    }
  else
    {
    std::cout << "Error: Unable to open filter file" << std::endl;
    ClearBuffers();
    return -1;
    }

   std::cout << "Number filter taps = " << m_FiltSize<< std::endl;
   return 0;
}

//************************************************************************
//**
//** Setup
//**
//** Setup FIR filter
//**
//** Paramters
//** int FilterSize; Number of filter taps
//** double* pFilterCoeff; pointer to array containing filter coefficients
//**
//************************************************************************

int CLPFilter::Setup( int FilterSize, double* pFilterCoeff)
{
  int j;
  //double t = 0.0;


   if( AllocateArrays( FilterSize) != 0 )
     {
     std::cout << "Memory allocation error: unable to continue" << std::endl;
     return -1;
     }

  m_FiltSize = FilterSize;


  for ( j=0; j < m_FiltSize; j++ )
    {
    m_pFilter[j] = pFilterCoeff[j];
    m_pFilter[m_FiltSize + j] = pFilterCoeff[j];
    }

    return 0;

}

//*******************************************************
//**
//** Filter
//**
//** Filter data
//**
//** Parameters
//**  double* pIpVal; pointer to input data sample
//**  double* pOpVa; pointer to filter output sample
//**
//*******************************************************

void CLPFilter::Filter( double* pIpVal, double* pOpVal )
{

  int i;

  m_pFilterBuffer[m_FiltCount] = *pIpVal;

  *pOpVal  = 0.0;
  for( i=0; i < m_FiltSize; i++ )
    {
    *pOpVal += (m_pFilterBuffer[i] * m_pFilter[ m_FiltSize -i + m_FiltCount]);
    }


  m_FiltCount++;

  if (m_FiltCount == m_FiltSize) m_FiltCount = 0 ;

  }
