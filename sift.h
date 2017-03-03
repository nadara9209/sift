//-----------------------------------------------------------------
#ifndef			__SIFT_H
#define			__SIFT_H

//-----------------------------------------------------------------
#include <list>
#include "pgm.h"

//-----------------------------------------------------------------
#define DSCR_WIDTH     4
#define DSCR_HIST_BINS 8
#define DSCR_LENGTH    (DSCR_WIDTH*DSCR_WIDTH*DSCR_HIST_BINS)

//-----------------------------------------------------------------
struct DETECTION_DATA
{
    int u;      
    int v;     
    int o;
    int s;     
    
    double ds;
    double scl_octv;
};
struct SIFT_DESCRIPTOR
{
    double x;      
    double y;      
    double scl;    
    double arg;   
    
    double v[DSCR_LENGTH]; 
    
    DETECTION_DATA  *ddata;
    
    SIFT_DESCRIPTOR(){ ddata=NULL; }
};
void SIFT(PGM &src,std::list<SIFT_DESCRIPTOR> &keys,CString &t);
void SIFT(PGM &src,std::list<SIFT_DESCRIPTOR> &keys);
#endif

