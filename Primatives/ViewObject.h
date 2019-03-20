// ViewObject.h: interface for the CViewObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_GFXOBJECT_H__E2CFC027_CEDF_11D1_A27F_006097A05989__INCLUDED_)
#define _GFXOBJECT_H__E2CFC027_CEDF_11D1_A27F_006097A05989__INCLUDED_

#include <vector>

#include "..\primatives\Matrix.h"
#include "..\primatives\Polygon.h"
#include "..\primatives\VertexHolder.h"
#include "..\primatives\Surface.h"
#include "..\primatives\import.h"

typedef std::vector<CSurface *> SurfaceSet;
typedef std::vector<CViewObject *> ViewObjectSet;

class CCamera;

class PRIMATIVES_EXPORT CViewObject : public CVertexHolder
{
public:
   CViewObject();
   ~CViewObject();

   CSurface * getSurface( int i );
   CVertex * getVertex( int i );
   bool load( std::string objectName, CMatrix &mtrxTransformation );

   void calculateNormals()
   {
      calculatePolygonNormals();
      calculateVertexNormal();
   }

   void calculatePolygonNormals()
   {
      for( int i = 0; i<m_nSurfaces; i++ )
      {
         m_SurfaceSet[i]->calculatePolygonNormals();
      }
   }

   void calculateVertexNormal()
   {
      for ( int i = 0; i < m_nVertices; i++ )
      {
         m_ppVertexSet[i]->calculateNormal();
      }
   }

   std::string getName() 
   { 
      return m_strName; 
   }
   
   int getID() 
   { 
      return m_nID; 
   }
   
   int getNoVertices() 
   { 
      return m_nVertices; 
   }

   int getNoSurfaces() 
   { 
      return m_nSurfaces; 
   }

private:
   std::string m_strName;
	int m_nID;
	int m_nSurfaces;
	CMatrix m_mtrxTransformation;
	SurfaceSet m_SurfaceSet;
};

#endif // !defined(_GFXOBJECT_H__E2CFC027_CEDF_11D1_A27F_006097A05989__INCLUDED_)
