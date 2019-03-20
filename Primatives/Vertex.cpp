// Vertex.cpp: implementation of the CVertex class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "Vertex.h"
#include "Polygon.h"
#include "Matrix.h"

void CVertex::addPolygon( CPolygon * pPolygon )
{
   m_PolyList.push_back( pPolygon );
   m_nPolygons++;
}

void CVertex::calculateNormal()
{
   CVector sum;

   CVector * vecNormal = 0;

   for ( PolygonList::iterator it = m_PolyList.begin(); it != m_PolyList.end(); it++ )
   {
      const CVector & polyNormal = (*it)->getNormal();
      sum.m_x += polyNormal.m_x;
      sum.m_y += polyNormal.m_y;
      sum.m_z += polyNormal.m_z;
   }

   m_vecNormal.m_x = sum.m_x / m_nPolygons;
   m_vecNormal.m_y = sum.m_y / m_nPolygons;
   m_vecNormal.m_z = sum.m_z / m_nPolygons;

   m_vecNormal.normalize();
}


