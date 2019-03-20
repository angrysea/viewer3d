// CCamera.h: interface for the CCamera class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_CAMERA_H__986638C5_D461_11D1_A280_006097A05989__INCLUDED_)
#define _CAMERA_H__986638C5_D461_11D1_A280_006097A05989__INCLUDED_

#include <iostream.h>
#include <fstream.h>

#include "..\primatives\Matrix.h"
#include "..\primatives\Vector.h"
#include "..\primatives\ViewFustrum.h"
#include "..\primatives\import.h"

class PRIMATIVES_EXPORT CCamera  
{
public:
   CCamera()
   {
   }

   ~CCamera()
   {
   }

   void load( istream& is )
   {
      m_vecLocation.load( is );

      is >> m_nHorizontal;
      is >> m_nVertical;
      is >> m_nRotation;
      is >> m_fDistance;
      is >> m_fViewDistance;

      m_ViewFustrum.init( m_fDistance );
      updateTransform();

   }

   void rotate( int horz, int vert )
   {
      m_nHorizontal+=horz;
      if ( m_nHorizontal < 0 )
      {
         m_nHorizontal +=359;
      }
      else if ( m_nHorizontal >= 360 )
      {
         m_nHorizontal -=359;
      }
      m_nVertical-=vert;
      if ( m_nVertical < 0 )
      {
         m_nVertical +=359;
      }
      else if ( m_nVertical >= 360 )
      {
         m_nVertical -=359;
      }
      updateTransform();
   }

   void twist( int rot )
   {
      m_nRotation-=rot;
      if ( m_nRotation < 0 )
      {
         m_nRotation +=359;
      }
      else if ( m_nRotation >= 360 )
      {
         m_nRotation -=359;
      }
      updateTransform();
   }

   void moveCamera( int x, int y, int z )
   {
      CMatrix m;
      CVector VPN;
      VPN = m_vecLocation - m_vecFocalPoint;
      VPN.normalize();

      CVector VUP( m_vecLocation - m_vecFocalPoint );
      VUP.normalize();
      VUP.m_y++;
      VUP.normalize();

      CVector u;
      u.crossProduct( VUP, VPN );
      u.normalize();
      
      VPN *= z;
      VUP *= y;
      u   *= x;
      
      m_vecLocation += VPN;
      m_vecLocation += VUP;
      m_vecLocation += u;

      updateTransform();
   }

   void updateTransform()
   {
      // Calculate the current focal point based on camera location
      // and the angles it is pointed at.
      CMatrix m;
      m_vecFocalPoint = CVector( 0, 0, -m_fDistance );
      m.setRotationX( m_nHorizontal );
      m.setRotationY( m_nVertical );
      m.setTranslation( m_vecLocation );
      m.Transform( m_vecFocalPoint );
      
      m_ViewFustrum.transform( m );

      // First the Z Vector which is the VPN
      CVector Z;
      Z = m_vecLocation - m_vecFocalPoint;
      Z.normalize();

      CVector VUP( m_vecLocation - m_vecFocalPoint );
      VUP.normalize();
      VUP.m_y++;
      VUP.normalize();

      // Adjust for rotation of camera.
      if ( m_nRotation != 0 )
      {
         m.reset();
         m.setRotationZ( m_nRotation );
         m.Transform( VUP );
      }

      // The X vector is the normalized cross product of VUP and Z
      CVector X;
      X.crossProduct( VUP, Z );
      X.normalize();
      
      CVector Y;
      Y.crossProduct( Z, X );
      
      // Reset the matrix from last time
      m_mtxTransform.reset();
      // Do the translation
      m_vecFocalPoint.reverse();
      m_mtxTransform.setTranslation( m_vecFocalPoint );
      m_vecFocalPoint.reverse();
      // Set the camera transform
      m_mtxTransform.setCameraTransformation( X, Y, Z );

   }

   CVector getFocalPoint() const
   { 
      return m_vecFocalPoint; 
   }

   const CMatrix & getTransformation() const 
   { 
      return m_mtxTransform; 
   }

   double getViewDistance() const 
   { 
      return m_fViewDistance; 
   }

   const CVector & getLocation() const
   { 
      return m_vecLocation; 
   }

   int getVertical() const
   { 
      return m_nVertical; 
   }

   int getHorizontal() const
   { 
      return m_nHorizontal; 
   }

   operator CMatrix & () 
   { 
      return m_mtxTransform; 
   }

   const CViewFustrum & getViewFustrum() const
   {
      return m_ViewFustrum;
   }

private:
   CVector        m_vecLocation;
   int            m_nVertical;
   int            m_nHorizontal;
   int            m_nRotation;
   double         m_fDistance;
   double         m_fViewDistance;

   CMatrix        m_mtxTransform;
   CVector        m_vecFocalPoint;
   CVector        m_VPN;
   CViewFustrum   m_ViewFustrum;
   
   friend PRIMATIVES_EXPORT ostream& operator << ( ostream& os, const CCamera & f );
};

inline PRIMATIVES_EXPORT ostream& operator << ( ostream& os, const CCamera & f )
{
   os << "Camera information " << endl;
   os << "Location: " << f.m_vecLocation << endl;
   os << "Vert: " << f.m_nVertical << " Horz: " << f.m_nHorizontal << " Rot: " << f.m_nRotation << " Dist: " << f.m_fDistance << " View Dist: " << f.m_fViewDistance << endl;

   os << "Focal point: " << f.m_vecFocalPoint << endl;
   os << "VPN: " << f.m_VPN << endl;
   os << f.m_ViewFustrum;

   return os;
}

#endif // !defined(_CAMERA_H__986638C5_D461_11D1_A280_006097A05989__INCLUDED_)
