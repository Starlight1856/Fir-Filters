//*****************************************************
//**
//** General Purpose FIR filter C++
//**
//** Paul Byrne 9/11/21
//**
//******************************************************


#ifndef __LPFILTER_H
#define __LPFILTER_H

class CLPFilter
  {
    private:


    double*    m_pFilter;
    double*    m_pFilterBuffer;
    double    m_FilterCorrect;
    int       m_FiltCount;
    int       m_FiltSize;

    private:
     void ClearBuffers(void);
     int  AllocateArrays(int FilterSize );
  public:
    CLPFilter();
    ~CLPFilter();
    int Setup( int FilterSize, double* pFilterCoeff);
    int FileSetup( std::string FilterFile);
    void Filter(double* pIpVal, double* pOpVal);
  };


#endif
