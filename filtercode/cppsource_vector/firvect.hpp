#ifndef __LPFILTER_H
#define __LPFILTER_H

#include <vector>
#include <deque>
using namespace std;


class CLPFilter
  {
    private:
    vector <double> m_vFilter;
    deque <double> m_vFilterBuffer;
    vector <double> m_vFilterResults;
    int       m_FiltCount;
    int       m_FiltSize;

    private:
     void ClearBuffers(void);
     int  AllocateVectors(int FilterSize );

  public:
    CLPFilter();
    ~CLPFilter();
    int Setup( int FilterSize, double* pFilterCoeff);
    int FileSetup( std::string FilterFile);
    void Filter(double* pIpVal, double* pOpVal);
  };


#endif
