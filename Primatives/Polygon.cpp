// Polygon.cpp: implementation of the CPolygon class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Polygon.h"
#include "Vertex.h"
#include "ViewObject.h"

void CPolygon::load(istream& is, CViewObject &theViewObject)
{
   CVertex * pVertex=0;
   int nVertex;
   is>>nVertex;
   
   resize(nVertex);

   for(int i=0; i<m_nVertices; i++)
   {
      is>>nVertex;

      pVertex=theViewObject.at(nVertex-1);
      if (pVertex !=0)
      {
         m_ppVertexSet[i]=pVertex;
         m_ppVertexSet[i]->AddRef();
         pVertex->addPolygon(this);
      }
   }

   computeCenter();
}


PRIMATIVES_EXPORT ostream& operator<<(ostream& os, const CPolygon & p)
{
   //os<<"Dot Product: "<<p.m_dDotProduct<<endl;
	os<<"Normal: "<<p.m_vecPolyNormal<<endl;
	os<<"Center: "<<p.m_vecCenter<<endl;
   
   os<<"Vertices: "<<endl;
   for(int i=0; i<p.m_nVertices; i++)
   {
      os<<*(p.m_ppVertexSet[i]);
   }

   os<<endl;

   return os;
}


