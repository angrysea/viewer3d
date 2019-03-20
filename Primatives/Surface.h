// Surface.h: interface for the CSurface class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_GFXSURFACE_H__E2CFC028_CEDF_11D1_A27F_006097A05989__INCLUDED_)
#define _GFXSURFACE_H__E2CFC028_CEDF_11D1_A27F_006097A05989__INCLUDED_

#include <vector>
#include "..\primatives\Matrix.h"
#include "..\primatives\Polygon.h"
#include "..\primatives\import.h"

class CCamera;
class CViewObject;

typedef std::vector<CPolygon*> PolygonSet;

class PRIMATIVES_EXPORT CSurface  
{
public:
   CSurface( CViewObject * pViewObject );
   ~CSurface();

   void removePolygon( CPolygon * pPolygon );
   void load( istream& is, CViewObject & pViewObject );
   CPolygon * getPolygon( int i );

   void calculatePolygonNormals()
   {
      for( int i = 0; i<m_nPolygons; i++ )
      {
         m_PolygonSet[i]->calculateNormal();
      }
   }

   void addPolygon( CPolygon * pPolygon )
   {
      pPolygon->AddRef();
      m_PolygonSet.push_back( pPolygon );
      m_nPolygons++;
   }

   int getNoPolygons() 
   { 
      return m_nPolygons; 
   }

   CViewObject * getViewObject() 
   { 
      return m_pViewObject; 
   }

private:
	int            m_nPolygons;
	PolygonSet     m_PolygonSet;
   CViewObject *  m_pViewObject;
};

#endif // !defined(_GFXSURFACE_H__E2CFC028_CEDF_11D1_A27F_006097A05989__INCLUDED_)
