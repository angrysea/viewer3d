// ViewObject.cpp: implementation of the CViewObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ViewObject.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CViewObject::CViewObject() 
           :m_nID(0),
            m_nSurfaces(0)
{
}

CViewObject::~CViewObject() 
{
   for(int i=0; i<m_nSurfaces; i++)
   {
      delete m_SurfaceSet[i];
   }
}

CSurface * CViewObject::getSurface(int i)
{
   return m_SurfaceSet[i];
}


bool CViewObject::load(std::string strFileName, CMatrix &mtrxTransformation)
{
   ifstream is;
   is.open(strFileName.c_str());

   if (!is)
   {
      //ATLTRACE("Could not open file ");
      return false;
   }
   else 
   {
      int nVertices;
      char temp[1024];

      is >> temp;
      m_strName = temp;
      is >> m_nID;
      is >> nVertices;

      resize(nVertices);

      CVertex * pVertex;
      for(int i=0; i<m_nVertices; i++)
      {
         pVertex = new CVertex;
         pVertex->load(is, mtrxTransformation);
         m_ppVertexSet[i] = pVertex;
      }

      is >> m_nSurfaces;

      m_SurfaceSet.erase(m_SurfaceSet.begin(), m_SurfaceSet.end());
      m_SurfaceSet.resize(m_nSurfaces);

      CSurface * pSurface;
      for(i=0; i<m_nSurfaces; i++)
      {
         pSurface = new CSurface(this);
         pSurface->load(is, *this);
         m_SurfaceSet[i] = pSurface;
      }
      is.close();
   }
   return true;
}
