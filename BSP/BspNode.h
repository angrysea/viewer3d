// BspNode.h: interface for the CBspNode class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_BSPNODE_H__D05E31C5_0B8A_11D2_8210_006097A05989__INCLUDED_)
#define _BSPNODE_H__D05E31C5_0B8A_11D2_8210_006097A05989__INCLUDED_

#include <iostream.h>
#include "..\primatives\Polygon.h"
#include "..\primatives\Vector.h"
#include "..\primatives\Vertex.h"
#include "..\primatives\ViewFustrum.h"

#include "..\bsp\import.h"

class CCamera;

class BSP_EXPORT CBspNode  
{
public:

   //enum EClass { none, back, on, front, spanning };
   enum EViewable { undefined, no, yes, children, backface, clip };

   CBspNode(CPolygon * pPolygon)
      :m_BackChild(0),
       m_pClippedPlane(0),
       m_FrontChild(0),
       m_eViewable(undefined),
       m_pPlane(0)
   {
      m_pPlane=pPolygon;
      m_pPlane->AddRef();
   }

   virtual ~CBspNode()
   {
      if(m_pPlane!=0)
         m_pPlane->Release();
      if(m_pClippedPlane!=0)
         m_pClippedPlane->Release();
      delete m_BackChild;
      delete m_FrontChild;
   }
	
   CBspNode(const CBspNode & aCopy)
     :m_pPlane(0),
      m_pClippedPlane(0),
      m_eViewable(undefined),
      m_BackChild(0),
      m_FrontChild(0)
   {
      m_BackChild=new CBspNode(*(aCopy.m_BackChild));
      m_FrontChild=new CBspNode(*(aCopy.m_FrontChild));
      m_pPlane=aCopy.m_pPlane;
      m_pPlane->AddRef();
   }

   void operator=(const CBspNode & aCopy)
   {
      m_BackChild=new CBspNode(*(aCopy.m_BackChild));
      m_FrontChild=new CBspNode(*(aCopy.m_FrontChild));
      m_pPlane=aCopy.m_pPlane;
      m_pPlane->AddRef();
      m_pClippedPlane=0;
   }
   
   bool operator>(const CBspNode & node)
   {
      CVector & vec=*(node.m_pPlane->at(0)->getPosition());
      return (vec.dotProduct(m_pPlane->getNormal())>-EPSILON);
   }

   bool operator<(const CBspNode & node)
   {
      CVector & vec=*(node.m_pPlane->at(0)->getPosition());
      return (vec.dotProduct(m_pPlane->getNormal())<-EPSILON);
   }

   void reset()
   {
      m_eViewable=undefined;
      if ( m_pClippedPlane != 0 )
      {
         m_pClippedPlane->Release();
         m_pClippedPlane=0;
      }

      if(m_BackChild!=0)
      {
         m_BackChild->reset();
      }

      if(m_FrontChild!=0)
      {
         m_FrontChild->reset();
      }
   }

   CPolygon::EClass split(CPolygon * pPolygon, CPolygon *& pPolyIn, CPolygon *& pPolyOut);

   CPolygon * getPlane() 
   { 
      return m_pPlane; 
   }
   
   CBspNode * getBackChild() const 
   { 
      return m_BackChild; 
   }

   CBspNode * getFrontChild() const 
   { 
      return m_FrontChild; 
   }

   CBspNode * push_back(CBspNode * pNode)
   {
      CBspNode * pRetNode=pNode->m_FrontChild;

      if(m_BackChild!=0)
      {
         if(*m_BackChild>*pNode)
         {
            pRetNode=m_BackChild->push_back(pNode);
         }
         else
         {
            CBspNode * pTemp=m_BackChild;
            m_BackChild=pNode;
         
            if(m_BackChild>pTemp->m_FrontChild)
            {
               m_BackChild->m_FrontChild=0;
            }
            else
            {
               m_BackChild->m_FrontChild=pTemp->m_FrontChild;
               pTemp->m_FrontChild=0;
            }

            m_BackChild->push_back(pTemp);
         }
      }
      else
      {
         m_BackChild=pNode;
      }

      if(pRetNode>0)
      {
         if(*pRetNode<*this)
         {
            pRetNode=push_foward(pRetNode);
         }
      }

      return pRetNode;
   }
   
   CBspNode * push_foward(CBspNode * pNode)
   {
      CBspNode * pRetNode=pNode->m_BackChild;

      if(m_FrontChild!=0)
      {
         if(*m_FrontChild<*pNode)
         {
            pRetNode=m_FrontChild->push_foward(pNode);
         }
         else
         {
            CBspNode * pTemp=m_FrontChild;
            m_BackChild=pNode;
         
            if(m_FrontChild<pTemp->m_BackChild)
            {
               m_FrontChild->m_BackChild=0;
            }
            else
            {
               m_FrontChild->m_BackChild=pTemp->m_BackChild;
               pTemp->m_BackChild=0;
            }

            m_FrontChild->push_back(pTemp);
         }
      }
      else
      {
         m_FrontChild=pNode;
      }

      if(pRetNode>0)
      {
         if(*pRetNode>*this)
         {
            pRetNode=push_back(pRetNode);
         }
      }

      return pRetNode;
   }

   void insert(PolygonList & polygonList)
   {																			
	   CPolygon * pPolygon=0;
      PolygonList frontPolygons;
      PolygonList backPolygons;

      CPolygon * pBackPolygon;
      CPolygon * pFrontPolygon;

      for (PolygonList::iterator it=polygonList.begin(); it!=polygonList.end(); it++)
      {
         pPolygon=*(it);

         pBackPolygon=0;
         pFrontPolygon=0;
         CPolygon::EClass classification=split(pPolygon, pBackPolygon, pFrontPolygon);

         if(classification==CPolygon::back || classification==CPolygon::spanning)
         {
            if(pBackPolygon!=0)
            {
				   backPolygons.push_back(pBackPolygon);
            }
         }

			if(classification==CPolygon::front || classification==CPolygon::spanning)
         {
            if(pFrontPolygon!=0)
            {
				   frontPolygons.push_back(pFrontPolygon);
            }
         }
	   }	
   
	   if(frontPolygons.size()!=0)
      {
         if(m_FrontChild==0)
         {
            pFrontPolygon=frontPolygons.back();
            frontPolygons.pop_back();
            m_FrontChild=new CBspNode(pFrontPolygon);
         }
		   m_FrontChild->insert(frontPolygons);
      }

      if(backPolygons.size()!=0)											
      {
         if(m_BackChild==0)
         {
            pBackPolygon=backPolygons.back();
            backPolygons.pop_back();
            m_BackChild=new CBspNode(pBackPolygon);
         }
		   m_BackChild->insert(backPolygons);
      }
   }																			

   bool setPotentiallyViewable(const CPolygon & plane, bool bFirst=true)
   {
      bool bRet=false;

      if(bFirst==false && m_eViewable==no)
      {
         // We have already determined this is not viewable no need 
         // to continue checking.
         return false;
      }
   
      CPolygon::EClass eClassification=m_pPlane->classify(plane);

      if(eClassification!=CPolygon::back)
      {
         // This node is potentially viewable and so may be both sets of children
         if(eClassification==CPolygon::spanning)
         {
            m_eViewable=clip;
         }
         else
         {
            m_eViewable=yes;
         }

         if(m_BackChild!=0)
         {
            m_BackChild->setPotentiallyViewable(plane, bFirst);
         }
         if(m_FrontChild!=0)
         {
            m_FrontChild->setPotentiallyViewable(plane, bFirst);
         }
         bRet=true;
      }
      else
      {
         m_eViewable=no;
         // I'm not viewable but depending on which way I am facing relative to 
         // the plane one of my children might be.
         CVector & vec=*(m_pPlane->at(0)->getPosition());
         if(vec.dotProduct(plane.getNormal())>0)
         {
            if(m_FrontChild!=0)
            {
               if((bRet=m_FrontChild->setPotentiallyViewable(plane, bFirst))==true)
               {
                  m_eViewable=children;
               }
            }
         }
         else
         {
            if(m_BackChild!=0)
            {
               if((bRet=m_BackChild->setPotentiallyViewable(plane, bFirst))==true)
               {
                  m_eViewable=children;
               }
            }
         }
      }
      return bRet;
   }

   void clipToFustrum(const CViewFustrum & fustrum)
   {
      switch (m_eViewable)
      {
         case clip:
         {
            for (int i=0; i<6; i++)
            {
               CPolygon * pClippedPlane;
               CPolygon::EClass eClassification = CPolygon::none;

               if(m_pClippedPlane!=0)
               {
                  eClassification=m_pClippedPlane->clipPlane(fustrum.getWall(i), pClippedPlane);
               }
               else
               {
                  eClassification=m_pPlane->clipPlane(fustrum.getWall(i), pClippedPlane);
               }

               if(eClassification==spanning)
               {
                  delete m_pClippedPlane;
                  m_pClippedPlane=pClippedPlane;
               }

               m_eViewable=yes;
            }
         }
         // Fall-thru

         case children:
            if(m_FrontChild!=0)
            {
               m_FrontChild->clipToFustrum(fustrum);
            }
            if(m_BackChild!=0)
            {
               m_BackChild->clipToFustrum(fustrum);
            }
            break;

         case no:
         default:
            break;
      }
   }

   void backFaceCull(const CVector & focalPoint)
   {
      switch(m_eViewable)
      {
         case yes:
         case clip:
         {
            const CVector & vec=focalPoint - m_pPlane->getCenter();
            if(vec.dotProduct(m_pPlane->getNormal())<0)
            {
               m_eViewable=backface;
            }
         }
         // Fall-thru

         case children:
            if(m_FrontChild!=0)
            {
               m_FrontChild->backFaceCull(focalPoint);
            }
            if(m_BackChild!=0)
            {
               m_BackChild->backFaceCull(focalPoint);
            }
            break;

         case no:
         default:
            break;
      }
   }

   void getVisibleList(PolygonList & polygonList)
   {
      if(m_eViewable!=no)
      {
         if(m_FrontChild!=0)
         {
            m_FrontChild->getVisibleList(polygonList);
         }

         if(m_eViewable==yes)
         {
            if(m_pClippedPlane!=0)
            {
               polygonList.push_back(m_pClippedPlane);
            }
            else
            {
               polygonList.push_back(m_pPlane);
            }
         }

         if(m_BackChild!=0)
         {
            m_BackChild->getVisibleList(polygonList);
         }
      }
   }

private:
   CBspNode *  m_BackChild;
   CBspNode *  m_FrontChild;
   CPolygon *  m_pPlane;
   CPolygon *  m_pClippedPlane;
   EViewable   m_eViewable;
   
   friend ostream& operator << (ostream& os, const CBspNode & node);
};

inline ostream& operator << (ostream& os, const CBspNode & node)
{
   os << "Start node  visible state: ";
   switch (node.m_eViewable)
   {
      case CBspNode::yes:
         os << "yes ";
         break;

      case CBspNode::backface:
         os << "backface ";
         break;
      
      case CBspNode::clip:
         os << "clipped ";
         break;

      case CBspNode::children:
         os << "children ";
         break;

      case CBspNode::no:
      default:
         os << "no ";
         break;
   }

   os << endl;

   if(node.m_pPlane!=0)
   {
      os << "Node Polygon: " << endl; 
      os << *(node.m_pPlane);
   }

   if(node.m_BackChild!=0)
   {
      os << "Back Child: " << endl; 
      os << *(node.m_BackChild);
   }

   if(node.m_FrontChild!=0)
   {
      os << "Front Child: " << endl; 
      os << *(node.m_FrontChild);
   }

   os << endl;

   return os;
}

#endif // !defined(_BSPNODE_H__D05E31C5_0B8A_11D2_8210_006097A05989__INCLUDED_)
