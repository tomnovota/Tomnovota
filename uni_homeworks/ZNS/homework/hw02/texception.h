#ifndef TEXCEPTION_H
#define TEXCEPTION_H

#include <iostream>
using namespace std;

struct TException : public exception
{
   string s;
   TException( string ss) : s(ss) {}
   ~TException() throw () {}
   const char* what() const throw();
};

#endif // TEXCEPTION_H
