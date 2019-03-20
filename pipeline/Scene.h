// Scene.h: interface for the CScene class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_SCENE_H__E2CFC02D_CEDF_11D1_A27F_006097A05989__INCLUDED_)
#define _SCENE_H__E2CFC02D_CEDF_11D1_A27F_006097A05989__INCLUDED_

#include "..\primatives\Camera.h"
#include "..\primatives\Matrix.h"
#include "..\primatives\ViewObject.h"
#include "..\bsp\BspTree.h"
#include "..\pipeline\import.h"

class PIPELINE_EXPORT CScene
{
public:
   CScene(double dScreenWidthMid=512, double dScreenHeightMid=384) 
       : m_pClosestVertex(0), 
         m_bDrawVertexNormals(true),
         m_dScreenWidthMid(dScreenWidthMid),
         m_dScreenHeightMid(dScreenHeightMid)
   {
   }

   ~CScene();
   
   void render();
   void buildTree();
   bool load( std::string ViewObjectName );

   void findClosestVertex       ( int x, int y );
   void moveVertex              ( CVector & v );

   std::string getName() 
   { 
      return m_strName; 
   }
   
   CCamera * getCamera() 
   { 
      return &m_Camera; 
   }

   int getNoViewObjects() const
   { 
      return m_nViewObjects; 
   }

   CViewObject const * getViewObject( int i ) const
   { 
      return m_ViewObjectSet.at(i); 
   }

   void save( const std::string & fileName );

private:
   std::string    m_strName;
   ViewObjectSet  m_ViewObjectSet;
   int            m_nID;
   int            m_nViewObjects;
   bool           m_bDrawVertexNormals;
   CCamera        m_Camera;
   CVertex *      m_pClosestVertex;
   CViewObject*   m_pClosestViewObject;
   CBspTree       m_bspTree;
   double         m_dScreenWidthMid;
   double         m_dScreenHeightMid;
};

#endif // !defined(_SCENE_H__E2CFC02D_CEDF_11D1_A27F_006097A05989__INCLUDED_)
