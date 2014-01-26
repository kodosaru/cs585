#include <windows.h>
#ifndef CSTOP_WATCH
#define CSTOP_WATCH

//taken from http://cplus.about.com/od/howtodothingsi2/a/timing.htm

typedef struct {
     LARGE_INTEGER start;
     LARGE_INTEGER stop;
 } stopWatch;
 
 class CStopWatch {
 
 private:
     stopWatch timer;
     LARGE_INTEGER frequency;
     double LIToSecs( LARGE_INTEGER & L) ;
 public:
     CStopWatch() ;
     void startTimer( ) ;
     void stopTimer( ) ;
     double getElapsedTime() ;
 };

#endif