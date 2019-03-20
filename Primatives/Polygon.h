// Polygon.h: interface for the CPolygon class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_POLYGON_H__E2CFC02B_CEDF_11D1_A27F_006097A05989__INCLUDED_)
#define _POLYGON_H__E2CFC02B_CEDF_11D1_A27F_006097A05989__INCLUDED_

#include <iostream.h>

#include "..\primatives\Camera.h"
#include "..\primatives\Vector.h"
#include "..\primatives\VertexHolder.h"
#include "..\primatives\import.h"

class CSurface;
class CViewObject;

class PRIMATIVES_EXPORT CPolygon : public CVertexHolder
{
public:
   CPolygon( CSurface * pSurface )
      :CVertexHolder(),
       m_ulReferenceCount( 1 ),
       m_pSurface( pSurface )
   {
   }

   CPolygon( CSurface * pSurface, const CVertex * pVertexList, int nVertices ) 
      :CVertexHolder( pVertexList, nVertices ),
       m_ulReferenceCount( 1 ),
       m_pSurface( pSurface )
   {
      computeCenter();
   }

   CPolygon( const CPolygon & aCopy ) 
     :CVertexHolder( aCopy ),
      m_ulReferenceCount( 1 ),
      m_pSurface       ( aCopy.m_pSurface ),
      m_dDotProduct    ( aCopy.m_dDotProduct ),
	   m_vecPolyNormal  ( aCopy.m_vecPolyNormal ),
      m_vecCenter      ( aCopy.m_vecCenter )
   {
   }

   CPolygon & operator = ( const CPolygon & aCopy )
   {
      m_dDotProduct    = aCopy.m_dDotProduct;
	   m_vecPolyNormal  = aCopy.m_vecPolyNormal;
      m_vecCenter      = aCopy.m_vecCenter;

      CVertexHolder::operator = ( aCopy );
      return *this;
   }

   void calculateScreenSpace( const CCamera & camera )
   {
      for ( int i = 0; i < m_nVertices; i++ )
      {
         m_ppVertexSet[i]->calculateScreenSpace( camera );
      }
   }


   void calculateScreenCoordinates(CCamera & camera, double dScreenWidthMid, double dScreenHeightMid)
   {
      for ( int i = 0; i < m_nVertices; i++ )
      {
         m_ppVertexSet[i]->calculateScreenCoordinates(camera, dScreenWidthMid, dScreenHeightMid);
      }
   }

   void calculateNormal()
   {
      if ( m_nVertices > 2 )
      {
         CVector * p1, * p2, * p3;

         p1 = m_ppVertexSet[0]->getPosition();
         p2 = m_ppVertexSet[1]->getPosition();
         p3 = m_ppVertexSet[2]->getPosition();

         if ( p1 != 0 && p2 != 0 && p3 != 0 )
         {
            CVector v1, v2;
            v1 = *p1 - *p2; 
            v2 = *p3 - *p2; 

            m_vecPolyNormal.crossProduct( v1, v2 );

            m_vecPolyNormal.normalize();
         }
      }
   }

   CSurface * getSurface() const
   { 
      return m_pSurface; 
   }

   void load( istream& is, CViewObject &theViewObject );
   
   int getNoVertexes() const 
   { 
      return m_nVertices; 
   }
   
   const CVector & getNormal() const 
   { 
      return m_vecPolyNormal; 
   }

	CVector & plane( CVector & p )
   {
      return m_vecPolyNormal.plane( p, *(m_ppVertexSet[0]->getPosition()) );
   }

   void transform( const CMatrix & mat )
   {
      for ( int i = 0; i < m_nVertices; i++ )
      {
         m_ppVertexSet[i]->transform( mat );
      }
      calculateNormal();
   }

   void computeCenter()
   {
	   m_vecCenter.m_x = m_vecCenter.m_y = m_vecCenter.m_z = 0.0;
	   for( unsigned short i = 0; i < m_nVertices; i++ )
	   {
         CVector * v;
         v = m_ppVertexSet[i]->getPosition();
		   m_vecCenter.m_x += v->m_x;
		   m_vecCenter.m_y += v->m_y;
		   m_vecCenter.m_z += v->m_z;
	   }
	   m_vecCenter.m_x /= m_nVertices;
	   m_vecCenter.m_y /= m_nVertices;
	   m_vecCenter.m_z /= m_nVertices;
   }

   enum EClass { none, back, on, front, spanning };

   EClass classify(const CPolygon & polygon) const
   {
      short nCount=count();
      short nFront=0, nBack=0;

      EClass eClassification=none;

      for (short i=0; i<nCount; i++)
      {
         if(polygon.getNormal().dotProduct(*(at(i)->getPosition()) - polygon.getCenter())<-EPSILON)
		   {
            nFront++;
         }
         else
         {
            nBack++;
         }
      }

      if(nFront>0)
      {
         if(nBack>0)
         {
            eClassification=spanning;
         }
         else
         {
            eClassification=front;
         }
      }
      else if(nBack>0)
      {
         eClassification=back;
      }
      return eClassification;
   }

   EClass clipPlane(const CPolygon & polygon, CPolygon * & pClippedPlane)
   {												
      pClippedPlane=0;

      short nCount=count();
      CVertex * frontVectors=new CVertex[nCount*2];
      nCount--;

      CVector & vecLast=*(at(nCount)->getPosition());
      CVector & vec1=polygon.getCenter() - vecLast;

	   double dot1=vec1.dotProduct(polygon.getNormal());
      EClass eClassification=on;

      if(dot1>-EPSILON)
      {
         eClassification=front;
      }
      else 
      {
         eClassification=back;
      }
   
      short nFront=0;

      for (short i=0; i<nCount; i++)
      {
         CVector & vec=*(at(i)->getPosition());
         CVector & vec2=polygon.getCenter() - vec;
         double dot2=vec2.dotProduct(polygon.getNormal());

         if(dot2>-EPSILON)
		   {
            if(eClassification==back)
            {
               eClassification=spanning;
            }

            if(dot1<-EPSILON)
            {
				   frontVectors[nFront++]=vecLast.planeIntersection(vec, polygon.getCenter(), polygon.getNormal());
            }
            frontVectors[nFront++]=vec;
         }
         else //if(dot2<-EPSILON)
         {
            if(eClassification==front)
            {
               eClassification=spanning;
            }
            if(dot1>-EPSILON)
            {
				   frontVectors[nFront++]=vecLast.planeIntersection(vec, polygon.getCenter(), polygon.getNormal());
            }
         }

         vec1=vec2;
         vecLast=vec;
         dot1=dot2;
      }

	   if(eClassification==spanning)
      {
         if(nFront>0)
         {
			   pClippedPlane=new CPolygon(pClippedPlane->getSurface(), frontVectors, nFront);
            pClippedPlane->calculateNormal();
         }
	   }
      delete []frontVectors;

      return eClassification;
   }																												

   const CVector & getCenter() const 
   { 
      return m_vecCenter; 
   }

private:
   double      m_dDotProduct;
	CVector     m_vecPolyNormal;
	CVector     m_vecCenter;
   CSurface *  m_pSurface;

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

   friend PRIMATIVES_EXPORT ostream& operator << ( ostream& os, const CPolygon & p );

};

#endif // !defined(_POLYGON_H__E2CFC02B_CEDF_11D1_A27F_006097A05989__INCLUDED_)
