//*****************************************************
//**
//** General Purpose FIR filter C++
//** Using Vectors
//**
//** Paul Byrne 9/11/21
//**
//******************************************************


#include <iostream>
#include <fstream>      // std::ifstream
#include <algorithm>
#include <functional>
#include <vector>
#include <deque>
#include <numeric>

#include "firvect.hpp"

using namespace std;


CLPFilter::CLPFilter()
{

  m_FiltCount = 0;
  m_FiltSize =0;
}

CLPFilter::~CLPFilter()
{
  ClearBuffers();
}

//************************************************************************
//**
//** AllocateVectors
//** Reserve for FIR coefficients and filter data buffer.
//**
//** Parameters
//** int FilterSize; number of taps in FIR filter.
//**
//************************************************************************

int  CLPFilter::AllocateVectors(int FilterSize )
{

  ClearBuffers();

   m_vFilter.reserve( FilterSize);
   m_vFilterResults.reserve( FilterSize);
   m_vFilterResults.resize(FilterSize);


  m_FiltSize = FilterSize;

  return 0;
}


//************************************************************************
//**
//** ClearBuffers
//**
//** Ensure vectors are cleared  prior to loading new coefficeints
//**
//************************************************************************

void CLPFilter::ClearBuffers(void)
{

  m_FiltCount =0;
  m_FiltSize=0;
  m_vFilter.clear();
  m_vFilterBuffer.clear();
  m_vFilterResults.clear();

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
  int filter_size =0;
  double t = 0.0;
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
      filter_size++;
      }
    }
  else
    {
    std::cout << "Error: Unable to continue" << std::endl;
    return -1;
    }

  //** Reserve vectors for number of filter terms
  if( AllocateVectors( filter_size ) != 0 )
     {
     std::cout << "Memory allocation error: unable to continue" << std::endl;
     return -1;
     }
     
  //** parse 2: load filter coefficinets
  //** rewind file
  coefffile.clear();
  coefffile.seekg(0, coefffile.beg);

  if ( coefffile.is_open())
    {
    getline ( coefffile, line); //** consume first line with filter specs

    //** continue to load filter coefficients
    while ( getline ( coefffile, line) )
      {
      //use line here
      m_vFilter.push_back( stod(line) ); //** filter coefficients
      m_vFilterBuffer.push_back( 0.0); //** filter buffer
      }
    coefffile.close();
    }
  else
    {
    std::cout << "Error: Unable to open filter file" << std::endl;
    ClearBuffers();
    return -1;
    }
    
   // matching filter taps with input data vector order
   reverse( m_vFilter.begin(), m_vFilter.end()); 
   t =  accumulate( m_vFilter.begin(), m_vFilter.end(), 0);

   std::cout << "Number filter taps = " << m_FiltSize << "   Sum= " << t << std::endl;
   return 0;
}

//************************************************************************
//**
//** Setup
//**
//** Setup FIR filter from parameters
//**
//** Paramters
//** int FilterSize; Number of filter taps
//** double* pFilterCoeff; pointer to array containing filter coefficients
//**
//************************************************************************

int CLPFilter::Setup( int FilterSize, double* pFilterCoeff)
{
  int j;
  double t = 0.0;


   if( AllocateVectors( FilterSize) != 0 )
     {
     std::cout << "Memory allocation error: unable to continue" << std::endl;
     return -1;
     }

  m_FiltSize = FilterSize;


  for ( j=0; j < m_FiltSize; j++ )
    {
    m_vFilter.push_back( pFilterCoeff[j] );
    m_vFilterBuffer.push_back( 0.0); //**
    }

    // matching filter taps with input data vector order
    reverse( m_vFilter.begin(), m_vFilter.end());
    t =  accumulate( m_vFilter.begin(), m_vFilter.end(), 0);

    std::cout << " Sum = " << t << std::endl;
    return 0;

}

//*******************************************************
//**
//** Filter
//**
//** Filter data
//**
//** Paramters
//**  double* pIpVal; pointer to input data sample
//**  double* pOpVa; pointer to filter output sample
//**
//*******************************************************

void CLPFilter::Filter( double* pIpVal, double* pOpVal )
{


  m_vFilterBuffer.pop_front();
  m_vFilterBuffer.push_back(*pIpVal);

  *pOpVal  = 0.0;

  transform( m_vFilter.begin(), m_vFilter.end(), m_vFilterBuffer.begin(), m_vFilterResults.begin(), multiplies<double>());
  *pOpVal = accumulate( m_vFilterResults.begin(), m_vFilterResults.end(),0.0 );

  }
