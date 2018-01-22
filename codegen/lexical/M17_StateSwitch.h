#ifndef M17_StateSwitch_h
#define M17_StateSwitch_h


#include "Input.h"
//#include "StringMaker.h"
#include "M17_StateFunctions.h"


int setupAnalyzer( char * filename, Token * token );
void destroyAnalyzer();
int getToken( Token * token );

#endif
