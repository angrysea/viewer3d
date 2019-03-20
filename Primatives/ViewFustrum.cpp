// ViewFustrum.cpp: implementation of the ViewFustrum class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <math.h>

#include "Vertex.h"
#include "Polygon.h"
#include "ViewFustrum.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PRIMATIVES_EXPORT ostream& operator << (ostream& os, const CViewFustrum & f)
{
   os << "View Fustrum" << endl;
   for(int i=0; i<6; i++)
   {
      os << "Wall #: " << i;
      os << *(f.m_pWalls[i]) << endl;
   }
   return os;
}

CViewFustrum::CViewFustrum()
{
   m_pLeft=0;
   m_pRight=0;
   m_pTop=0;
   m_pBottom=0;
   m_pFront=0;
   m_pBack=0;
}

CViewFustrum::~CViewFustrum()
{
   for(int i=0; i<6; i++)
   {
      m_pWalls[i]->Release();
   }
}

void CViewFustrum::init(double fDistance, double dNear, double dFar, int horz, int vert)
{
   if(dNear==0)
   {
      dNear=-fDistance + (fDistance * 0.2);
   }
   else
   {
      dNear +=-fDistance;
   }

   if(dFar==0)
   {
      dFar=fDistance*2;
   }

   double x=(pi * 2)/360; // Radins in a degree
   if(horz==0)
   {
      horz=30;
   }
   else
   {
      horz=horz/2;
   }
   
   if(vert==0)
   {
      vert=25;
   }
   else
   {
      vert=vert/2;
   }
   
   double xFactor=tan(vert*x);
   double yFactor=tan(horz*x);
   double xNear=abs(dNear*xFactor);
   double yNear=abs(dNear*yFactor);
   double xFar=abs(dFar*xFactor);
   double yFar=abs(dFar*yFactor);

   CVertex vertices[4];
   
   //Front Wall 0
   vertices[0].update(-xNear, -yNear, dNear);
   vertices[1].update(xNear, -yNear, dNear);
   vertices[2].update(xNear, yNear, dNear);
   vertices[3].update(-xNear, yNear, dNear);
   m_pWalls[0]=new CPolygon(0, vertices, 4);

   //Back Wall 1
   vertices[0].update(xFar, yFar, dFar);
   vertices[1].update(xFar, -yFar, dFar);
   vertices[2].update(-xFar, -yFar, dFar);
   vertices[3].update(-xFar, yFar, dFar);
   m_pWalls[1]=new CPolygon(0, vertices, 4);

   //Top Wall 2
   vertices[0].update(-xNear, yNear, dNear);
   vertices[1].update(xNear, yNear, dNear);
   vertices[2].update(xFar, yFar, dFar);
   vertices[3].update(-xFar, yFar, dFar);
   m_pWalls[2]=new CPolygon(0, vertices, 4);

   //Bottom Wall
   vertices[0].update(xNear, -yNear, dNear);
   vertices[1].update(-xNear, -yNear, dNear);
   vertices[2].update(-xFar, -yFar, dFar);
   vertices[3].update(xFar, -yFar, dFar);
   m_pWalls[3]=new CPolygon(0, vertices, 4);

   //Left Wall
   vertices[0].update(-xNear, -yNear, dNear);
   vertices[1].update(-xNear, yNear, dNear);
   vertices[2].update(-xFar, yFar, dFar);
   vertices[3].update(-xFar, -yFar, dFar);
   m_pWalls[4]=new CPolygon(0, vertices, 4);

   //Right Wall
   vertices[0].update(xNear, yNear, dNear);
   vertices[1].update(xNear, -yNear, dNear);
   vertices[2].update(xFar, -yFar, dFar);
   vertices[3].update(xFar, yFar, dFar);
   m_pWalls[5]=new CPolygon(0, vertices, 4);

   for(int i=0; i<6; i++)
   {
      m_pWalls[i]->calculateNormal();
   }
}


void CViewFustrum::transform(const CMatrix & m)
{
   for(int i=0; i<6; i++)
   {
      m_pWalls[i]->transform(m);
   }
}

