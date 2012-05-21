#include "Reader.h"
#include <errno.h>

#ifdef FVM_PARALLEL
#include <mpi.h>
#endif


Reader::Reader(const string& fileName) :
  _fileName(fileName),
  _fp(0)
{
  resetFilePtr();
}

Reader::~Reader()
{}



void
Reader::resetFilePtr()
{

  _fp = NULL;
   int numOfAttempts = 10;
  while( _fp == NULL && numOfAttempts > 0){
//     if (_fp==NULL)
//       fclose(_fp);
     _fp = fopen(_fileName.c_str(),"rb");
      if (_fp == NULL) 
         printf("Error opening file %s: errno=%d %m", _fileName.c_str(), errno);
      numOfAttempts--;
  }
 
  if ( _fp == NULL){
     //parallel
     #ifdef FVM_PARALLEL
     const int mpi_err=99;
     MPI::COMM_WORLD.Abort(mpi_err);
     #endif
     //serial
     #ifndef FVM_PARALLEL
     abort();
     #endif
  }
}

string
Reader::readLine()
{
  char buf[256];
  fgets(buf,256,_fp);
  string s(buf);
  return s;
}
    
void
Reader::close()
{
  if (_fp)
    fclose(_fp);
}
