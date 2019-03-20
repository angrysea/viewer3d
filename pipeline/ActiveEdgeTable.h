// ActiveEdgeTable.h: interface for the ActiveEdgeTable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACTIVEEDGETABLE_H__8725A243_1912_11D3_8108_006097A05989__INCLUDED_)
#define AFX_ACTIVEEDGETABLE_H__8725A243_1912_11D3_8108_006097A05989__INCLUDED_

#include "..\pipeline\import.h"
#include "..\pipeline\EdgeTable.h"
#include "..\pipeline\Edge.h"

class PIPELINE_EXPORT CActiveEdgeTable  
{
public:
	CActiveEdgeTable(CEdgeTable & edgeTable)
      :m_ActiveEdges(0),
       m_nEdgeCount(0),
       m_nCurrentY(0),
       m_EdgeTable(edgeTable)
   {
   }

	virtual ~CActiveEdgeTable()
   {
   }

   void scan(unsigned short * &bits)
   {
      CEdgeLink * currentEdge=m_ActiveEdges;

      while(currentEdge!=0)
      {
         CEdge * pEdge=(CEdge *)*(currentEdge);

         int xStart = (int)pEdge->XMin();
         int xEnd = (int)pEdge->XMax();
         
         memset(bits+xStart, 0xff, (xEnd-xStart)+1);

         currentEdge=currentEdge->next();
      }
   }

   void step()
   {
      m_nCurrentY++;
      if(m_ActiveEdges!=0)
      {
         m_ActiveEdges=m_ActiveEdges->step(m_nCurrentY);
      }

      if(m_ActiveEdges!=0)
      {
         if(m_EdgeTable[m_nCurrentY]!=0)
         {
            m_ActiveEdges=m_ActiveEdges->insert(*(m_EdgeTable[m_nCurrentY]));
         }
      }
      else
      {
         if(m_EdgeTable[m_nCurrentY]!=0)
         {
            m_ActiveEdges=m_EdgeTable[m_nCurrentY];
         }
      }
   }

   void reset()
   {
      delete m_ActiveEdges;

      m_ActiveEdges = 0;
      m_nEdgeCount = 0;
      m_nCurrentY = 0;
   }

private:
   long m_nCurrentY;
   long m_nEdgeCount;
   CEdgeLink * m_ActiveEdges;
   CEdgeTable & m_EdgeTable;
};

#endif // !defined(AFX_ACTIVEEDGETABLE_H__8725A243_1912_11D3_8108_006097A05989__INCLUDED_)
