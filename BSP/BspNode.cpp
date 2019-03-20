// BspNode.cpp: implementation of the CBspNode class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "BspNode.h"
#include "BspTree.h"
#include "..\primatives\Surface.h"
#include "..\primatives\ViewObject.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPolygon::EClass 
CBspNode::split(  CPolygon * pPolygon, 
                  CPolygon *& pPolyBack,
                  CPolygon *& pPolyFront )
{												
   short nCount = pPolygon->count();
   CPolygon::EClass eClassification = CPolygon::on;
   CVertex * frontVectors = 0;
   CVertex * backVectors = 0;

   try
   {
      frontVectors = new CVertex[nCount*2];
      backVectors = new CVertex[nCount*2];

      nCount--;
      CVector vecLast = *(pPolygon->at( nCount )->getPosition());
      CVector vec1 = m_pPlane->getCenter() - vecLast;
	   double dot1 = vec1.dotProduct( m_pPlane->getNormal() );

      if ( dot1 > -EPSILON )
      {
         eClassification = CPolygon::front;
      }
      else //if ( dot1 < -EPSILON )
      {
         eClassification = CPolygon::back;
      }
   
      short nFront = 0, nBack = 0;

      for ( short i = 0; i < nCount; i++ )
      {
         CVector vec = *(pPolygon->at( i )->getPosition());
         CVector vec2 = m_pPlane->getCenter() - vec;
         double dot2 = vec2.dotProduct( m_pPlane->getNormal() );

         if ( dot2 > -EPSILON )
		   {
            if ( eClassification == CPolygon::back )
            {
               eClassification = CPolygon::spanning;
            }

            if ( dot1 < -EPSILON )
            {
				   frontVectors[nFront++] = 
                  backVectors[nBack++] = 
                     vecLast.planeIntersection( vec, pPolygon->getCenter(), m_pPlane->getNormal() );
            }
            frontVectors[nFront++] = vec;
         }
         else //if ( dot2 < -EPSILON )
         {
            if ( eClassification == CPolygon::front )
            {
               eClassification = CPolygon::spanning;
            }

            if ( dot1 > -EPSILON )
            {
				   backVectors[nBack++] = 
                  frontVectors[nFront++] = 
                     vecLast.planeIntersection( vec, pPolygon->getCenter(), m_pPlane->getNormal() );
            }
            backVectors[nBack++] = vec;
         }

         vec1 = vec2;
         vecLast = vec;
         dot1 = dot2;
      }

	   switch ( eClassification )
	   {
		   case CPolygon::front:
			   pPolyFront = pPolygon;
			   break;

         case CPolygon::back:
			   pPolyBack = pPolygon;
			   break;

         case CPolygon::spanning:
            //pPolygon->getSurface()->removePolygon( pPolygon );

            if ( nFront > 0 )
            {
		         pPolyFront = new CPolygon( pPolygon->getSurface(), frontVectors, nFront );
               pPolyFront->NoRef();
               pPolyFront->calculateNormal();
               //pPolyFront->getSurface()->addPolygon( pPolyFront );
               //pPolyFront->getSurface()->getViewObject()->add( pPolyFront );
            }

            if ( nBack > 0 )
            {
   		      pPolyBack = new CPolygon( pPolygon->getSurface(), backVectors, nBack );
               pPolyBack->NoRef();
               pPolyBack->calculateNormal();
               //pPolyBack->getSurface()->addPolygon( pPolyBack );
               //pPolyBack->getSurface()->getViewObject()->add( pPolyBack );
            }
			   break;
      }																										
   }
   catch(...)
   {
   }
   
   delete [] frontVectors;
   delete [] backVectors;

	return eClassification;																				
}																												
