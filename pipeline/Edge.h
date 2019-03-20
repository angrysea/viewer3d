// Edge.h: interface for the CEdge class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EDGE_H__D9B99794_1821_11D3_8108_006097A05989__INCLUDED_)
#define AFX_EDGE_H__D9B99794_1821_11D3_8108_006097A05989__INCLUDED_

#include "..\primatives\Vector.h"
#include "..\pipeline\import.h"

class CPolygon;

class PIPELINE_EXPORT CEdge  
{
public:
	CEdge(CVector & p1, CVector & p2, CPolygon * pPolygon)
      :m_pPolygon(pPolygon),
       m_dSlope(0)
   {
      double dx;
      double dy;

      dx=p1.m_x-p2.m_x;
      dy=p1.m_y-p2.m_y;

      if(dy!=0 && dx!=0)
      {
         m_dSlope=dy/dx;
      }

      if(p1.m_y < p2.m_y)
      {
         m_Ymax=p2.m_y;
         m_Xmin=p1.m_x;
      }
      else
      {
         m_Ymax=p1.m_y;
         m_Xmin=p2.m_x;
      }

      m_Xmax=m_Xmin;
   }

	virtual ~CEdge()
   {
   }

   operator CPolygon *()
   {
      return m_pPolygon;
   }

   CPolygon * getPolygon()
   {
      return m_pPolygon;
   }

   double YMax()
   {
      return m_Ymax;
   }

   double XMin()
   {
      return m_Xmin;
   }

   double XMax()
   {
      return m_Xmax;
   }

   void step(long y)
   {
      m_Xmax+=m_dSlope*y;
   }

private:
   double m_Ymax;
   double m_Xmin;
   double m_Xmax;
   double m_dSlope;
   CPolygon * m_pPolygon;
};

class CEdgeLink
{
public:
   CEdgeLink(CEdge * pEdge)
      :m_pEdge(pEdge),
       m_pNext(0)
   {
   }

   virtual ~CEdgeLink()
   {
      delete m_pNext;
      delete m_pEdge;
   }
   
   operator CEdge *()
   {
      return m_pEdge;
   }
    
   void next(CEdgeLink * pNext)
   {
      m_pNext=pNext;
   }

   CEdgeLink * next()
   {
      return m_pNext;
   }

   CEdgeLink * step(long y)
   {
      CEdgeLink * ret = this;

      //If Max is in range to process.
      if(m_pEdge->YMax()<=y)
      {
         if(m_pNext!=0)
         {
            m_pNext=m_pNext->step(y);
         }
      }
      else
      {
         if(m_pNext!=0)
         {
            ret=m_pNext->step(y);
         }
         else
         {
            ret=0;
         }
      }
      
      return ret;
   }

   CEdgeLink * insert(CEdgeLink & edgeLink)
   {
      //Assume that the new edge will be inserted after
      CEdgeLink * ret = this;
      CEdge * pNewEdge = (CEdge*)edgeLink;

      //Check to see who has the lower x value.
      if(m_pEdge->XMin()>pNewEdge->XMin())
      {
         //This will remain in it's place and will insert the
         //new edge into the next link

         //check to see if there is already a next link
         if(m_pNext!=0)
         {
            m_pNext=m_pNext->insert(edgeLink);
         }
         else
         {
            // No we will be the next link
            m_pNext=&edgeLink;
         }
      }
      else
      {
         // insert the current link in after the new link
         // return the new link as the aet
         ret=&edgeLink;
         
         // Check to see if there already is a next
         if(edgeLink.m_pNext==0)
         {
            //Set this as the next link
            edgeLink.m_pNext = this;
         }
         else
         {
            // insert this into the next link
            edgeLink.m_pNext = edgeLink.m_pNext->insert(*this);
         }
      }
      return ret;
   }

   void insert(CEdge * pEdge)
   {
      CEdge * myEdge = 0;
      
      if(m_pEdge!=0&&m_pEdge->YMax()<pEdge->YMax())
      {
         myEdge=m_pEdge;
      }
      else
      {
         myEdge=pEdge;
         pEdge=m_pEdge;
      }

      if(m_pNext==0)
      {
         m_pNext = new CEdgeLink(pEdge);
      }
      else
      {
         m_pNext->insert(pEdge);
      }
      m_pEdge=myEdge;
   }

private:
   CEdge * m_pEdge;
   CEdgeLink * m_pNext;
};

#endif // !defined(AFX_EDGE_H__D9B99794_1821_11D3_8108_006097A05989__INCLUDED_)
