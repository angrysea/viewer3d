   // EdgeTable.h: interface for the EdgeTable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EDGETABLE_H__D9B99793_1821_11D3_8108_006097A05989__INCLUDED_)
#define AFX_EDGETABLE_H__D9B99793_1821_11D3_8108_006097A05989__INCLUDED_

#include "..\primatives\Vector.h"
#include "..\primatives\Vertex.h"
#include "..\primatives\Polygon.h"
#include "..\pipeline\import.h"
#include "..\pipeline\Edge.h"

class PIPELINE_EXPORT CEdgeTable
{
public:
	CEdgeTable(int nLines)
      :m_nLines(nLines)
   {
      m_EdgeTable = new CEdgeLink *[nLines];
      memset(m_EdgeTable,0x00,sizeof(CEdgeLink *)*nLines);
   }

	virtual ~CEdgeTable()
   {
      for(int i=0;i<m_nLines;i++)
      {
         m_EdgeTable[i];
      }
      delete [] m_EdgeTable;
   }

   void insert(CPolygon & polygon)
   {
      int h=polygon.count()-1;
      if(h>0)
      {
         CVector * vh, * vi;
         CEdge * pEdge=0;
         int nIndex=0;
         for(int i=0;i<polygon.count();i++)
         {
            vh = polygon[h]->getScreenPos();
            vi = polygon[i]->getScreenPos();
            if(vi->m_y!=vh->m_y)
            {
               nIndex=vi->m_y<vh->m_y?vi->m_y:vh->m_y;
         
               pEdge = new CEdge(*vh, *vi, &polygon);

               if(m_EdgeTable[nIndex]==0)
               {
                  m_EdgeTable[nIndex]=new CEdgeLink(pEdge);
               }
               else
               {
                  m_EdgeTable[nIndex]->insert(pEdge);
               }
            }
            h=i;
         }
      }
   }

   CEdgeLink * operator[](int i)
   {
      return m_EdgeTable[i];
   }

public:
   int m_nLines;
   CEdgeLink ** m_EdgeTable;
};

#endif // !defined(AFX_EDGETABLE_H__D9B99793_1821_11D3_8108_006097A05989__INCLUDED_)
