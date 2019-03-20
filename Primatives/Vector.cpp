// Vertex.cpp: implementation of the CVertex class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include <fstream.h>

#include "Vector.h"

CVector CVector::planeIntersection( const CVector & end, const CVector & pointOnPlane,  const CVector & planeNormal ) const
{
   CVector line1;
   CVector line2;

   line1.m_x = end.m_x - m_x;
   line1.m_y = end.m_y - m_y;
   line1.m_z = end.m_z - m_z;

   line2.m_x = m_x - pointOnPlane.m_x;
   line2.m_y = m_y - pointOnPlane.m_y;
   line2.m_z = m_z - pointOnPlane.m_z;

   double t = ( ( line2.m_x * planeNormal.m_x ) + ( line2.m_y * planeNormal.m_y ) + ( line2.m_z * planeNormal.m_z ) )  / 
              ( ( line1.m_x * -planeNormal.m_x ) + ( line1.m_y * -planeNormal.m_y ) + ( line1.m_z * -planeNormal.m_z ) );
   
   CVector results;

   results.m_x = m_x + (line1.m_x * t);
   results.m_y = m_y + (line1.m_y * t);
   results.m_z = m_z + (line1.m_z * t);

   if (  (results.m_x == end.m_x && 
         results.m_y == end.m_y &&
         results.m_z == end.m_z) ||
         (results.m_x == m_x && 
         results.m_y == m_y &&
         results.m_z == m_z) )
   {
      ofstream os;
      os.open( "e:\\destiny\\data\\dump.dat" );

      os << "Previous" << *this << endl;
      os << "End" << end << endl;
      os << "Result" << results << endl;
      os.close();
   }


   return results;
}
