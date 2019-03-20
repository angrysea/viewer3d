// Surface.cpp: implementation of the CSurface class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Surface.h"
#include "ViewObject.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CSurface::CSurface( CViewObject * pViewObject )
            :m_pViewObject( pViewObject )
{
}

CSurface::~CSurface()
{
   for( int i = 0; i<m_nPolygons; i++ )
   {
      m_PolygonSet[i]->Release();
   }
}

CPolygon * 
CSurface::getPolygon( int i )
{
   return m_PolygonSet.at(i);
}

void CSurface::load( istream& is, CViewObject & pViewObject )
{
	is >> m_nPolygons;

   m_PolygonSet.erase( m_PolygonSet.begin(), m_PolygonSet.end() );
   m_PolygonSet.resize( m_nPolygons );

   CPolygon * pPolygon;
   for ( int i = 0; i < m_nPolygons; i++ )
   {
      pPolygon = new CPolygon( this );
      pPolygon->load( is, pViewObject );
      m_PolygonSet[i] = pPolygon;
   }
}

void CSurface::removePolygon( CPolygon * pPolygon )
{
   PolygonSet::iterator theIterator;

   theIterator = m_PolygonSet.begin();
   while (theIterator != m_PolygonSet.end())
   {
      if ( pPolygon == (*theIterator) )
      {
         pPolygon->Release();
         m_PolygonSet.erase( theIterator );
         break;
      }
      theIterator++;
   }
   m_nPolygons--;
}

