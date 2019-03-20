// Matrix.h: interface for the CMatrix class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_MATRIX_H__E2CFC022_CEDF_11D1_A27F_006097A05989__INCLUDED_)
#define _MATRIX_H__E2CFC022_CEDF_11D1_A27F_006097A05989__INCLUDED_

#include <memory.h>

#include "..\primatives\Vector.h"
#include "..\primatives\import.h"

extern double normal[4][4];
extern double clearMat[4][4];

class PRIMATIVES_EXPORT CMatrix  
{
public:
   CMatrix()
   { 
      reset(); 
   }
   
   CMatrix( double data[4][4] )
   {
      memcpy( m_data, data, sizeof( m_data ) );
   }

   virtual ~CMatrix()
   {
   }
   
   void Project( const CVector & in, CVector & out ) const
   {
      double w =0;

      out.m_x = (m_data[0][0]*in.m_x)+(m_data[1][0]*in.m_y)+(m_data[2][0]*in.m_z)+(m_data[3][0]);
      out.m_y = (m_data[0][1]*in.m_x)+(m_data[1][1]*in.m_y)+(m_data[2][1]*in.m_z)+(m_data[3][1]);
      out.m_z = (m_data[0][2]*in.m_x)+(m_data[1][2]*in.m_y)+(m_data[2][2]*in.m_z)+(m_data[3][2]);
      
      w = (m_data[0][3]*in.m_x)+(m_data[1][3]*in.m_y)+(m_data[2][3]*in.m_z)+(m_data[3][3]);

      out.m_x /= w;
      out.m_y /= w;
      out.m_z /= w;
   
   }

   void Transform( CVector & vect ) const
   {
      CVector v;

      v.m_x = (m_data[0][0]*vect.m_x)+(m_data[1][0]*vect.m_y)+(m_data[2][0]*vect.m_z)+(m_data[3][0]);
      v.m_y = (m_data[0][1]*vect.m_x)+(m_data[1][1]*vect.m_y)+(m_data[2][1]*vect.m_z)+(m_data[3][1]);
      v.m_z = (m_data[0][2]*vect.m_x)+(m_data[1][2]*vect.m_y)+(m_data[2][2]*vect.m_z)+(m_data[3][2]);

      vect = v;
   
   }

   void setTranslation( CVector & vec )
   {
      CMatrix temp;

      temp.m_data[3][0] = vec.m_x;
      temp.m_data[3][1] = vec.m_y; 
      temp.m_data[3][2] = vec.m_z; 

      *this *= temp;
   }

   void setProjection( double fDistance )
   {
      m_data[2][3] = 1 / fDistance; 
      m_data[3][3] = 0; 
   }

   void setScaling( CVector & vec )
   {
      CMatrix temp;

      temp.m_data[0][0] = vec.m_x;
      temp.m_data[1][1] = vec.m_y; 
      temp.m_data[2][2] = vec.m_z; 
      temp.m_data[3][3] = 1; 

      *this *= temp;
   }

   void setRotationX( int theta )
   {
      CMatrix temp;

      temp.m_data[1][1] = g_cos[theta];
      temp.m_data[2][1] = -g_sin[theta];
      temp.m_data[1][2] = g_sin[theta];
      temp.m_data[2][2] = g_cos[theta];

      *this *= temp;
   }

   void setRotationY( int theta )
   {
      CMatrix temp;

      temp.m_data[0][0] = g_cos[theta];
      temp.m_data[2][0] = g_sin[theta];
      temp.m_data[0][2] = -g_sin[theta];
      temp.m_data[2][2] = g_cos[theta];

      *this *= temp;
   }

   void setRotationZ( int theta )
   {
      CMatrix temp;

      temp.m_data[0][0] = g_cos[theta];
      temp.m_data[1][0] = -g_sin[theta];
      temp.m_data[0][1] = g_sin[theta];
      temp.m_data[1][1] = g_cos[theta];

      *this *= temp;
   }

   void reset()
   {
      memcpy( m_data, normal, sizeof( m_data ) );
   }  

   void clear()
   {
      memcpy( m_data, clearMat, sizeof( m_data ) );
   }  

   CMatrix & operator = ( const CMatrix & aMatrix )
   {
      memcpy( m_data, aMatrix.m_data, sizeof( m_data ) );
      return *this;
   }  

   CMatrix & operator *= ( const CMatrix & aMatrix )
   {
      CMatrix temp = *this;
      
      clear();

      for ( int row = 0; row < 3; row++ )
      {
         for ( int col = 0; col < 4; col++ )
         {
            for ( int i = 0; i < 4; i++ )
            {
               m_data[col][row] +=  aMatrix.m_data[i][row] * temp.m_data[col][i];
            }
         }
      }

      m_data[0][3] = 0;
      m_data[1][3] = 0;
      m_data[2][3] = 0;
      m_data[3][3] = 1;

      return *this;
   }

   CMatrix reverse()
   {
      CMatrix temp;
      
      for ( int row = 0; row < 4; row++ )
      {
         for ( int col = 0; col < 4; col++ )
         {
            temp.m_data[row][col] = -temp.m_data[row][col];
         }
      }

      return temp;
   }

   void setCameraTransformation( const CVector &X, const CVector &Y, const CVector & Z )
   {
      CMatrix temp;

      temp.m_data[0][0] = X.m_x;
      temp.m_data[0][1] = X.m_y;
      temp.m_data[0][2] = X.m_z;
      temp.m_data[0][3] = 0;
      temp.m_data[1][0] = Y.m_x;
      temp.m_data[1][1] = Y.m_y;
      temp.m_data[1][2] = Y.m_z;
      temp.m_data[1][3] = 0;
      temp.m_data[2][0] = Z.m_x;
      temp.m_data[2][1] = Z.m_y;
      temp.m_data[2][2] = Z.m_z;
      temp.m_data[2][3] = 0;
      temp.m_data[3][0] = 0;
      temp.m_data[3][1] = 0;
      temp.m_data[3][2] = 0;
      temp.m_data[3][3] = 1;
      *this *= temp;
   }


private:
   double m_data[4][4];

};

#endif // !defined(_MATRIX_H__E2CFC022_CEDF_11D1_A27F_006097A05989__INCLUDED_)
