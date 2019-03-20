// Scene.cpp: implementation of the CScene class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <math.h>

#include "..\primatives\Vector.h"
#include "..\primatives\Vertex.h"
#include "..\primatives\Polygon.h"
#include "..\primatives\Surface.h"

#include "Scene.h"
#include "EdgeTable.h"
#include "ActiveEdgeTable.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CScene::~CScene()
{
   for( int i = 0; i<m_nViewObjects; i++ )
   {
      delete m_ViewObjectSet.at(i);
	}
}

void CScene::render()
{
   const int nLines = 1024;
   unsigned short bits[769];
   unsigned short *pBits=bits;
   PolygonList polygonList;
   CEdgeTable et(nLines);
   if ( m_bspTree.getPolygonsToRender( m_Camera, polygonList ) == true )
   {
      //ofstream os;
      //os.open( "e:\\destiny\\data\\scenesp2.data" );

      for( PolygonList::iterator it = polygonList.begin(); it != polygonList.end(); it++ )
      {
         (*it)->calculateScreenSpace( m_Camera );
         (*it)->calculateScreenCoordinates( m_Camera,m_dScreenWidthMid,m_dScreenHeightMid);
         //os << *(*it);
         et.insert(*(*it));
      }

      //os.close();

      HDC hdc=GetDC(GetFocus());
      CActiveEdgeTable activeEdgeTable(et);
      for(int i=0;i<nLines;i++)
      {
         memset(bits,0x00,sizeof(bits));
         activeEdgeTable.scan(pBits);
         
         for(int x=0;x<767;x++)
         {
            if(bits[x]!=0x00)
            {
               SetPixel(hdc,x+1,i+1,RGB(255,255,255));
            }
         }
         activeEdgeTable.step();
      }
   }
}

bool CScene::load( std::string strFileName )
{
   ifstream is;
   is.open( strFileName.c_str() );

   if ( !is )
   {
      //ATLTRACE( "Could not open file " );
      return false;
   }
   else
   {
      char temp[1024];
      is >> temp;
      m_strName = temp;
      is >> m_nID;
      is >> m_nViewObjects;
      
      m_Camera.load( is );
      
      CViewObject * pViewObject;
      std::string strViewObjectName;
      
      int xtran;
      int ytran;
      int ztran;
      
      int xRot;
      int yRot;
      int zRot;
      
      int xScale;
      int yScale;
      int zScale;
 
      CMatrix mat;

      m_ViewObjectSet.resize( m_nViewObjects );
      for ( int i = 0; i < m_nViewObjects; i++ )
      {
         is >> temp;
         strViewObjectName = temp;
         is >> xtran;
         is >> ytran;
         is >> ztran;
         is >> xRot;
         is >> yRot;
         is >> zRot;
         is >> xScale;
         is >> yScale;
         is >> zScale;

         if ( xtran != 0 || ytran != 0 || ztran != 0 )
         {
            CVector v( xtran, ytran, ztran );
            mat.setTranslation( v );
         }

         if ( xRot != 0 )
         {
            mat.setRotationX( xRot );
         }

         if ( yRot != 0 )
         {
            mat.setRotationY( yRot );
         }

         if ( zRot != 0 )
         {
            mat.setRotationZ( zRot );
         }

         if ( xScale != 0 || yScale != 0 || zScale )
         {
            CVector v( xScale, yScale, zScale );
            mat.setScaling( v );
         }

         pViewObject = new CViewObject;
         pViewObject->load( strViewObjectName, mat );
         mat.reset();
         m_ViewObjectSet[i] = pViewObject;
         pViewObject->calculateNormals();

      }
      is.close();

   }

   buildTree();

   return true;
}

void CScene::moveVertex( CVector & v )
{
   if ( m_pClosestVertex != 0 )
   {
      CVector *pvecPosition = m_pClosestVertex->getPosition();
 
      pvecPosition->m_x = -v.m_x;
      pvecPosition->m_y = -v.m_y;
   }
}


void CScene::findClosestVertex( int x, int y )
{
   double nDist = 0.0, nClosest;
   nClosest = 1.0e50;
   for( int i = 0; i<m_nViewObjects; i++ )
   {
      CViewObject * pCurrentViewObject = m_ViewObjectSet[i];

      int nVertices = pCurrentViewObject->getNoVertices();

      for( int j = 1; j<nVertices; j++ )
      {
         CVertex * pCurrentVertex = pCurrentViewObject->at( j );
         if ( pCurrentVertex != 0 )
         {
            CVector * pScreenPos = pCurrentVertex -> getScreenPos();
            nDist = exp( abs(x - pScreenPos->m_x) ) + exp( abs(y - pScreenPos->m_y) );
            if ( nDist <= nClosest )
            {
               nClosest = nDist;
               m_pClosestViewObject = pCurrentViewObject;
               m_pClosestVertex = pCurrentVertex;
               m_pClosestVertex->dump();
            }
         }
      }
   }
}

void CScene::buildTree()
{
   for( int i = 0; i<m_nViewObjects; i++ )
   {
      CViewObject * pCurrentViewObject = m_ViewObjectSet.at(i);
      int nSurfaces = pCurrentViewObject->getNoSurfaces();

      PolygonList polyList;
      for( int j = 0; j<nSurfaces; j++ )
      {
         CSurface * pCurrentSurface = pCurrentViewObject->getSurface( j );
         if ( pCurrentSurface != 0 )
         {
            int nPolygons = pCurrentSurface->getNoPolygons();
            for( int k = 0; k < nPolygons; k++ )
            {
               polyList.push_back( pCurrentSurface->getPolygon( k ) );
            }
         }
      }
      m_bspTree.insert( polyList );
	}
}


void CScene::save( const std::string & fileName )
{
   ofstream os;
   os.open( fileName.c_str() );

   os << m_Camera << endl;
   os << m_bspTree;

   os.close();
}
