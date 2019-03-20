// CBspTree.h: interface for the CBspTree class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_BSPTREE_H__0ED4E261_D924_11D1_A281_006097A05989__INCLUDED_)
#define _BSPTREE_H__0ED4E261_D924_11D1_A281_006097A05989__INCLUDED_

#include <iostream.h>
#include <string>
#include <vector>
#include "..\primatives\Vertex.h"
#include "..\bsp\BspNode.h"
#include "..\primatives\camera.h"


class BSP_EXPORT CBspTree
{
public:
   CBspTree() 
      :m_pRoot(0) 
   {
   }

   CBspTree(const CBspTree& aCopy) 
      :m_pRoot(0) 
   {
      m_pRoot=new CBspNode(*(aCopy.m_pRoot));
   }

   virtual ~CBspTree()
   { 
      delete m_pRoot; 
   }

   void merge(CBspTree& aTree);

   void operator=(const CBspTree& aCopy) 
   {
      m_pRoot=new CBspNode(*(aCopy.m_pRoot));
   }

   void setRootNode(CBspNode * rootNode) 
   { 
      m_pRoot=rootNode; 
   }

   CBspNode * getRoot()
   { 
      return m_pRoot; 
   }

   void insert(PolygonList& polygonSet)
   {
      try
      {
         if (m_pRoot==0)
         {
            CPolygon * pPoly=polygonSet.back();
            polygonSet.pop_back();
            m_pRoot=new CBspNode(pPoly);
         }
	      m_pRoot->insert(polygonSet);
      }
      catch (exception e)
      {
      }
   }

   bool getPolygonsToRender(const CCamera& camera, PolygonList& polygonList)
   {
      bool bRet=false;
      if ((bRet=clipToFustrum(camera))==true)
      {
         m_pRoot->getVisibleList(polygonList);
      }
      return bRet;
   }

   bool clipToFustrum(const CCamera& camera)
   {
      bool bStatus=true;

      m_pRoot->reset();
   
      try
      {
         bool bFirst=true;
         const CViewFustrum& fustrum=camera.getViewFustrum();
         for (int i=0; i<6 && bStatus==true; i++)
         {
            bStatus=m_pRoot->setPotentiallyViewable(fustrum.getWall(i), bFirst);
            bFirst=false;

            if (bStatus==false)
            {
               throw bStatus;
            }
         }
         m_pRoot->backFaceCull(camera.getFocalPoint());
         m_pRoot->clipToFustrum(fustrum);
      }
      catch(bool e)
      {
         bStatus=e;
      }
      return bStatus;
   }

private:
	CBspNode * m_pRoot;
   friend ostream& operator << (ostream& os, const CBspTree& tree);
};

inline ostream& operator << (ostream& os, const CBspTree& tree)
{
   os << *(tree.m_pRoot);
   return os;
}


#endif // !defined(_BSPTREE_H__0ED4E261_D924_11D1_A281_006097A05989__INCLUDED_)
