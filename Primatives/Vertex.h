// Vertex.h: interface for the CVertex class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_VERTEX_H__E2CFC026_CEDF_11D1_A27F_006097A05989__INCLUDED_)
#define _VERTEX_H__E2CFC026_CEDF_11D1_A27F_006097A05989__INCLUDED_

#include <string>
#include <vector>
#include <iostream.h>

#include "..\primatives\Camera.h"
#include "..\primatives\Matrix.h"
#include "..\primatives\Vector.h"
#include "..\primatives\import.h"

#define NSF 50

class CMatrix;
class CPolygon;

typedef std::vector<CPolygon *> PolygonList;

#pragma warning(disable:4251)

class PRIMATIVES_EXPORT CVertex
{
public:
   
   CVertex() 
      :m_ulReferenceCount( 1 ),
       m_nPolygons( 0 )
   {
   }

   CVertex( double x, double y, double z ) 
      :m_ulReferenceCount( 1 ),
       m_vecPosition( x, y, z ),
       m_nPolygons( 0 )
   {
   }

   CVertex( const CVector & vec ) 
      :m_ulReferenceCount( 1 ),
       m_vecPosition( vec ), 
       m_nPolygons( 0 )
   {
   }

   CVertex( const CVertex & v )
     :m_ulReferenceCount( 1 ),
      m_vecPosition     ( v.m_vecPosition ),
	   m_vecScreenSpace  ( v.m_vecScreenSpace ),
	   m_vecNormal       ( v.m_vecNormal ),
      m_screenPos       ( v.m_screenPos ),
      m_nPolygons       ( v.m_nPolygons ),
      m_PolyList        ( v.m_PolyList )
   {
   }

   void calculateNormal();
   void addPolygon( CPolygon * pPolygon );

   CVertex & operator = ( const CVertex & v )
   {
      m_vecPosition     = v.m_vecPosition;
	   m_vecScreenSpace  = v.m_vecScreenSpace;
	   m_vecNormal       = v.m_vecNormal;
      m_screenPos       = v.m_screenPos;

      m_nPolygons       = v.m_nPolygons;
      return *this;
   }
   
   CVertex & operator = ( const CVector & vec ) 
   {
      m_vecPosition = vec;
      return *this;
   }
   
   void load( istream & is, CMatrix & matrix )
   {
	   m_vecPosition.load( is );
      matrix.Transform( m_vecPosition );
   }

   void getViewVector( CCamera & camera, CVector & v )
   {
      CVector & vecCamera = camera.getFocalPoint();
      v.m_x = vecCamera.m_x - m_vecPosition.m_x;
      v.m_y = vecCamera.m_y - m_vecPosition.m_y;
      v.m_z = vecCamera.m_z - m_vecPosition.m_z;
   }

   void drawVertexNormals( HDC hdc, CCamera & camera, double dScreenWidthMid, double dScreenHeightMid)
   {
      MoveToEx( hdc, m_screenPos.m_x, m_screenPos.m_y, 0 );
   
      CVector tw;

      tw.m_x = m_vecPosition.m_x + ( NSF * m_vecNormal.m_x );
      tw.m_y = m_vecPosition.m_y + ( NSF * m_vecNormal.m_y );
      tw.m_z = m_vecPosition.m_z + ( NSF * m_vecNormal.m_z );

      camera.getTransformation().Transform( tw );

      double x, y;
      tw.perspective( camera.getViewDistance(), dScreenWidthMid, dScreenHeightMid, x, y );

      LineTo( hdc, x, y );
   
   }

   void lineTo( HDC hdc )
   {
      ::LineTo( hdc, m_screenPos.m_x, m_screenPos.m_y );
   }

   void moveTo( HDC hdc )
   {
      ::MoveToEx( hdc, m_screenPos.m_x, m_screenPos.m_y, 0 );
   }

   void calculateScreenSpace( const CCamera & camera )
   {
	   m_vecScreenSpace = m_vecPosition;
      camera.getTransformation().Transform( m_vecScreenSpace );
   }

   void calculateScreenCoordinates(CCamera & camera, double dScreenWidthMid, double dScreenHeightMid)
   {
      m_vecScreenSpace.perspective(camera.getViewDistance(), dScreenWidthMid, dScreenHeightMid, m_screenPos);
   }

   int getNoPolygons() 
   { 
      return m_nPolygons; 
   }

   CVector * getScreenPos() 
   { 
      return &m_screenPos; 
   }

   CVector *  getPosition() 
   { 
      return &m_vecPosition; 
   }

   CVector * getScreenSpace() 
   { 
      return &m_vecScreenSpace; 
   }
   
   void setPosition(CVector & vec) 
   { 
      m_vecPosition = vec; 
   }
   void setScreenSpace(CVector & vec) 
   { 
      m_vecScreenSpace = vec; 
   }

   CVector * getNormal() 
   { 
      return &m_vecNormal; 
   }

   operator POINT() const
   {
	   POINT	pt;
	   pt.x = (short)m_screenPos.m_x;
	   pt.y = (short)m_screenPos.m_y;
	   return pt;
   }

   void dump()
   {
      char szText[1024];
      sprintf( szText, "%+4.0f,%+4.0f\n", m_screenPos.m_x, m_screenPos.m_y );
   }

   void draw( HDC hdc )
   {
      char szText[1024];
      RECT rc;
      sprintf( szText,"%+4.0f,%+4.0f", m_screenPos.m_x, m_screenPos.m_y );

      SIZE size;
      if ( ::GetTextExtentPoint( hdc,
                                 szText,
                                 lstrlen( szText ), 
                                 &size ) != 0 )
      {
         rc.top = m_screenPos.m_y;
         rc.left = m_screenPos.m_x;
         rc.right = size.cx + rc.left;
         rc.bottom = size.cy + rc.top;

         DrawText( hdc, szText, strlen( szText ), &rc, DT_TOP | DT_LEFT );
      }
   }

   void update( double x, double y, double z ) 
   {
      m_vecPosition.m_x = x;
      m_vecPosition.m_y = y;
      m_vecPosition.m_z = z;
   }

   void update( const CVertex & v ) 
   {
      m_vecPosition.m_x = v.m_vecPosition.m_x;
      m_vecPosition.m_y = v.m_vecPosition.m_y;
      m_vecPosition.m_z = v.m_vecPosition.m_z;
   }

   void transform( const CMatrix & mat )
   {
      mat.Transform( m_vecPosition );
   }

private:
	CVector        m_vecPosition;
	CVector        m_vecScreenSpace;
	CVector        m_vecNormal;
   CVector        m_screenPos;
   int            m_nPolygons;
   // A list of all the polygons that have a reference to this vertex.
   PolygonList    m_PolyList;

public:
   unsigned long AddRef()
   {
      return ++m_ulReferenceCount;
   }

   void NoRef()
   {
      m_ulReferenceCount = 0;
   }

   unsigned long Release()
   {
      unsigned long ulRet = --m_ulReferenceCount;
      if ( m_ulReferenceCount <= 0 )
      {
         delete this;
      }
      return ulRet;
   }

private:
   unsigned long  m_ulReferenceCount;
   
   friend ostream & operator << ( ostream& os, const CVertex & v );
};

inline ostream & operator << ( ostream& os, const CVertex & v )
{
   os <<"Position :" << v.m_vecPosition;
	os << "  Screen Position :" << v.m_screenPos << endl;
	//os <<" Normal :" << v.m_vecNormal << endl;

   return os;
}


#endif // !defined(_VERTEX_H__E2CFC026_CEDF_11D1_A27F_006097A05989__INCLUDED_)
