// primatives.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"

#include <math.h>

#include "defines.h"
#include "Matrix.h"

class CPrimatives 
{
public:
	CPrimatives();
};

double pi = 3.1415926535;   

double g_sin[360];
double g_cos[360];

CPrimatives aPrimative;

double clearMat[4][4] = {   { 0, 0, 0, 0 },
                           { 0, 0, 0, 0 },
                           { 0, 0, 0, 0 },
                           { 0, 0, 0, 0 } };

double normal[4][4] = {  { 1, 0, 0, 0 },
                        { 0, 1, 0, 0 },
                        { 0, 0, 1, 0 },
                        { 0, 0, 0, 1 } };

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}

CPrimatives::CPrimatives()
{ 
   double x = ( pi * 2 ) / 360; // Radins in a degree

   for( int i = 0; i < 360; i++ )
   {
      g_sin[i]=sin(i*x);
      g_cos[i]=cos(i*x);
   }

   return;
}

