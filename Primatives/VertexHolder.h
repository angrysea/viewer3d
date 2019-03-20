// VertexHolder.h: interface for the CVertexHolder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_VERTEXHOLDER_H__E2CFC02B_CEDF_11D1_A27F_006097A05989__INCLUDED_)
#define _VERTEXHOLDER_H__E2CFC02B_CEDF_11D1_A27F_006097A05989__INCLUDED_

#include "..\primatives\Vertex.h"
#include "..\primatives\import.h"

class PRIMATIVES_EXPORT CVertexHolder
{
public:

   CVertexHolder() : 
      m_nVertices( 0 ), 
      m_ppVertexSet( 0 ) 
   {
   }

   CVertexHolder( const CVertexHolder & aCopy ) 
      : m_nVertices( aCopy.m_nVertices )
   {
      m_ppVertexSet = new CVertex *[ m_nVertices];
      for ( int i=0; i< m_nVertices; i++ )
      {
         m_ppVertexSet[i] = new CVertex( *(aCopy.m_ppVertexSet[i]) );
      }
   }

   CVertexHolder( const CVertex * pVertexList, int nVertices ) 
      :m_nVertices( nVertices )
   {
      m_ppVertexSet = new CVertex *[m_nVertices];
      
      for ( int i=0; i< m_nVertices; i++ )
      {
         if ( i>0 )
         {
            CVector * v1 = const_cast<CVertex&>(pVertexList[i-1]).getPosition();
            CVector * v2 = const_cast<CVertex&>(pVertexList[i]).getPosition();
         }
         m_ppVertexSet[i] = new CVertex( pVertexList[i] );
      }
       
   }

   virtual ~CVertexHolder() 
   { 
      if ( m_ppVertexSet != 0 )
      {
         for ( int i=0; i<m_nVertices; i++ )
         {
            m_ppVertexSet[i]->Release();
         }
         delete [] m_ppVertexSet; 
         m_ppVertexSet = 0;
      }
   }

   CVertexHolder & operator = ( const CVertexHolder & aCopy )
   {
      m_nVertices = aCopy.m_nVertices;
      
      m_ppVertexSet = new CVertex *[ m_nVertices];
      for ( int i=0; i< m_nVertices; i++ )
      {
         m_ppVertexSet[i] = new CVertex( *(aCopy.m_ppVertexSet[i]) );
      }

      return *this;
   }
   
   CVertex * operator[](int x) const
   {
#if defined( _DEBUG )
      return at(x);
#else
      return m_ppVertexSet[x];
#endif
   }

   CVertex * at(int i) const
   {
      if ( i <= m_nVertices )
      {
         return m_ppVertexSet[i];
      }
      else
      {
         return 0;
      }
   }

   void resize( int x )
   {
      CVertex ** ppTemp = m_ppVertexSet;
      m_ppVertexSet = new CVertex *[x];

      if ( ppTemp != 0 )
      {
         memcpy( m_ppVertexSet, ppTemp, sizeof(CVertex*)*m_nVertices );
         delete ppTemp;
      }
      m_nVertices = x;
   }

   void setVertex( CVertex * pVertex, int i )
   {
      if ( i <= m_nVertices )
      {
         m_ppVertexSet[i] = pVertex;
      }
      else
      {
         throw exception( "out of range" );
      }
   }

   void add( CVertexHolder * pHolder ) 
   {
      int start = m_nVertices;
      resize( m_nVertices + pHolder->m_nVertices );
      
      for ( int i=0; i< m_nVertices; i++ )
      {
         m_ppVertexSet[i+start] = pHolder->m_ppVertexSet[i];
      }
       
   }

   int count() const 
   { 
      return m_nVertices; 
   }

protected:
   int            m_nVertices;
   CVertex **     m_ppVertexSet;
};

#endif // !defined(_VERTEXHOLDER_H__E2CFC02B_CEDF_11D1_A27F_006097A05989__INCLUDED_)
