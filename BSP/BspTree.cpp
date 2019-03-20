// CBspTree.cpp: implementation of the CBspTree class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BspTree.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
void CBspTree::merge( CBspTree & aTree )
{
   CBspNode * pNode = aTree.getRoot()->getBackChild();
   if ( *m_pRoot > *(pNode) )
   {
      m_pRoot->push_back( pNode );
   }
   else
   {
      m_pRoot->push_foward( pNode );
   }

   pNode = aTree.getRoot()->getFrontChild();
   if ( *m_pRoot > *(pNode) )
   {
      m_pRoot->push_back( pNode );
   }
   else
   {
      m_pRoot->push_foward( pNode );
   }
}

