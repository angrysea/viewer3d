// Vector.h: interface for the CVector class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_VECTOR_H__E2CFC01E_CEDF_11D1_A27F_006097A05989__INCLUDED_)
#define _VECTOR_H__E2CFC01E_CEDF_11D1_A27F_006097A05989__INCLUDED_

#include <iostream.h>
#include <math.h>

#include "..\primatives\defines.h"
#include "..\primatives\import.h"

#define m_x data[0]
#define m_y data[1]
#define m_z data[2]
#define m_w data[3]

class PRIMATIVES_EXPORT CVector  
{
public:
   CVector( double x = 0, double y = 0, double z = 0, double w = 1 )
   {
      m_x = x; 
      m_y = y; 
      m_z = z; 
      m_w = w;    
   }

   CVector( const CVector & aCopy )
   {
      m_x = aCopy.m_x; 
      m_y = aCopy.m_y; 
      m_z = aCopy.m_z; 
      m_w = aCopy.m_w;
   }

   void perspective( double dViewDistance, double dScreenWidthMid, double dScreenHeightMid, double & nScreenX, double & nScreenY )
   {
      double x, y;

      if ( m_x != 0 )
      {
         x = dViewDistance * (m_x / m_z);
      }
      else
      {
         x = 0;
      }

      if ( m_y != 0 )
      {
         y = dViewDistance * (m_y / m_z);
      }
      else
      {
         y = 0;
      }
      nScreenX = dScreenWidthMid + x;
      nScreenY = dScreenHeightMid + y;
   }

   void perspective(double dViewDistance, double dScreenWidthMid, double dScreenHeightMid, CVector & vecScreen)
   {
      double x, y;

      if ( m_x != 0 )
      {
         x = dViewDistance * (m_x / m_z);
      }
      else
      {
         x = 0;
      }

      if ( m_y != 0 )
      {
         y = dViewDistance * (m_y / m_z);
      }
      else
      {
         y = 0;
      }
      vecScreen.m_x=dScreenWidthMid + x;
      vecScreen.m_y=dScreenHeightMid + y;
      vecScreen.m_z=m_z;
   }

   double magnitude() const
   {
	   return sqrt( (m_x*m_x) + (m_y*m_y) + (m_z*m_z) );
   }

   double distance( const CVector & v ) const
   {
	   return sqrt( (m_x*v.m_x) + (m_y*v.m_y) + (m_z*v.m_z) );
   }

   void load( istream & is)
   {
      is >> m_x;
      is >> m_y;
      is >> m_z;
   }

   void setVector( double x = 0, double y = 0, double z = 0, double w = 0 )
   {
      m_x = x;
      m_y = y;
      m_z = z;
      m_w = w;    
   }

   CVector & operator = ( const CVector & aVec )
   {
      m_x = aVec.m_x;
      m_y = aVec.m_y;
      m_z = aVec.m_z;
      
      return *this;
   }

   bool operator == ( const CVector & aVec )
   {
      return   m_x == aVec.m_x &&
               m_y == aVec.m_y &&
               m_z == aVec.m_z;
   }

   bool operator != ( const CVector & aVec )
   {
      return   m_x != aVec.m_x ||
               m_y != aVec.m_y ||
               m_z != aVec.m_z;
   }

   CVector & operator += ( const CVector & aVec )
   {
      m_x += aVec.m_x;
      m_y += aVec.m_y;
      m_z += aVec.m_z;
      
      return *this;
   }

   CVector operator + ( const CVector & aVec ) const
   {
      CVector r;
      r.m_x = m_x + aVec.m_x;
      r.m_y = m_y + aVec.m_y;
      r.m_z = m_z + aVec.m_z;
      
      return r;
   }

   CVector & operator -= ( const CVector & aVec )
   {
      m_x -= aVec.m_x;
      m_y -= aVec.m_y;
      m_z -= aVec.m_z;
      
      return *this;
   }

   CVector operator - ( const CVector & aVec ) const
   {
      CVector r;
      r.m_x = m_x - aVec.m_x;
      r.m_y = m_y - aVec.m_y;
      r.m_z = m_z - aVec.m_z;
      
      return r;
   }

   CVector operator * ( double d ) const
   {
      CVector r;
      r.m_x = m_x * d;
      r.m_y = m_y * d;
      r.m_z = m_z * d;
      
      return r;
   }

   CVector & operator *= ( double d )
   {
      m_x = m_x * d;
      m_y = m_y * d;
      m_z = m_z * d;
      
      return *this;
   }

   CVector operator / ( double d ) const
   {
      CVector r;
      r.m_x = m_x / d;
      r.m_y = m_y / d;
      r.m_z = m_z / d;
      
      return r;
   }

   CVector & operator /= ( double d )
   {
      m_x = m_x / d;
      m_y = m_y / d;
      m_z = m_z / d;
      
      return *this;
   }

   void crossProduct( const CVector & v1, const CVector & v2 )
   {
      m_x = ( v1.m_y * v2.m_z ) - ( v2.m_y * v1.m_z );
      m_y = ( v1.m_z * v2.m_x ) - ( v2.m_z * v1.m_x );
      m_z = ( v1.m_x * v2.m_y ) - ( v2.m_x * v1.m_y );
   }

   double dotProduct( const CVector & v ) const
   {
	   return ( m_x * v.m_x ) + ( m_y * v.m_y ) + ( m_z * v.m_z );
   }

   CVector & plane( CVector & p, const CVector & v )
   {
      p.setVector( m_x, m_y, m_z, dotProduct( v ) );
      return p;
   }

   CVector & normalize()
   {
	   double fd = magnitude();

	   if ( fd != 0 )
	   {
		   m_x /= fd;
		   m_y /= fd;
		   m_z /= fd;
	   }
      return *this;
   }

   void reverse()
   {
      m_x *= -1;
      m_y *= -1;
      m_z *= -1;
   }

   CVector planeIntersection( const CVector & end, const CVector & pointOnPlane,  const CVector & planeNormal ) const
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

      return results;
   }

   double data[4];

   friend ostream & operator << ( const CVector & v, const ostream& os );

};

inline ostream & operator << ( ostream& os, const CVector & v )
{
   os << " x: " << v.m_x;
   os << " y: " << v.m_y;
   os << " z: " << v.m_z;
   return os;
}

#endif // !defined(_VECTOR_H__E2CFC01E_CEDF_11D1_A27F_006097A05989__INCLUDED_)
