// CList.h: interface for the CList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(GFXLIST_H__D05E31C8_0B8A_11D2_8210_006097A05989__INCLUDED_)
#define GFXLIST_H__D05E31C8_0B8A_11D2_8210_006097A05989__INCLUDED_

#include "..\primatives\import.h"

template <class t> class CListIterator;

template <class t>
class PRIMATIVES_EXPORT CList  
{
public:

   template <class t>
   struct SNode
   {
	   t nodeType;
      SNode	* pNextNode;
   };

   CList() 
      :m_ListHead( 0 )
   {
   }
   
   CList( const CList & aCopy )
   {
      m_ListHead = 0;
      if ( aCopy.m_ListHead > 0 )
      {
         SNode<t> * pTempCopy = aCopy.m_ListHead;
         SNode<t> * pTemp;
   
         while( pTempCopy != 0 )
         {
            pTemp = new SNode<t>;
	         pTemp->nodeType  = pTempCopy->nodeType;
            pTemp->pNextNode = pTempCopy->pNextNode;
            if ( m_ListHead == 0 )
            {
               m_ListHead = pTemp;
            }
            pTempCopy = pTempCopy->pNextNode;
         }
      }
   }
   
   virtual ~CList()
   {
      while( m_ListHead )
      {
         SNode<t> * pTemp = m_ListHead;
         m_ListHead = m_ListHead->pNextNode;
         delete pTemp;
      }
   }

   CList & operator = ( const CList & aCopy )
   {
      m_ListHead = 0;
      if ( aCopy.m_ListHead > 0 )
      {
         SNode<t> * pTempCopy = aCopy.m_ListHead;
         SNode<t> * pTemp;
   
         while( pTempCopy != 0 )
         {
            pTemp = new SNode<t>;
	         pTemp->nodeType  = pTempCopy->nodeType;
            pTemp->pNextNode = pTempCopy->pNextNode;
            if ( m_ListHead == 0 )
            {
               m_ListHead = pTemp;
            }
            pTempCopy = pTempCopy->pNextNode;
         }
      }
   }

   void push( t nodeType )
   {
      SNode<t> * p = new SNode<t>;

      p->nodeType = nodeType;
      p->pNextNode = m_ListHead;
      m_ListHead = p;
   }

	t pop()
   {
      t ret;
      if ( m_ListHead == 0 )
      {
         throw exception( "Empty List" );
      }
      ret = m_ListHead->nodeType;
      SNode<t> * pTemp = m_ListHead;
      m_ListHead = pTemp->pNextNode;
      delete pTemp;

      return ret;
   }

	void append( CList &polygonList )
   {
      if ( aCopy.m_ListHead > 0 )
      {
         SNode<t> * pTempCopy = aCopy.m_ListHead;
         SNode<t> * pTemp = m_ListHead;

         while( pTemp != 0 )
         {
            pTemp = pTemp->pNextNode;
         }
   
         while( pTempCopy != 0 )
         {
            pTemp = new SNode<t>;
	         pTemp->nodeType  = pTempCopy->nodeType;
            pTemp->pNextNode = pTempCopy->pNextNode;
            if ( m_ListHead == 0 )
            {
               m_ListHead = pTemp;
            }
            pTempCopy = pTempCopy->pNextNode;
         }
      }
   }

	bool isEmpty() const
   {
      return m_ListHead == 0;
   }

private:
   SNode<t> *  m_ListHead;

   friend class CListIterator<t>;


};

template <class t>
class PRIMATIVES_EXPORT CListIterator
{
public:
   CListIterator( const CList<t> & aList ) 
      : m_ListHead( aList.m_ListHead ), 
        m_Current( aList.m_ListHead ) { }
   
   bool begin() { return m_Current == m_ListHead; }

   void reset() { m_Current = m_ListHead; }

   void next() 
   { 
      if ( m_Current->pNextNode != 0 )
      {
         CList<t>::SNode<t> * pTemp = m_Current;
         m_Current = m_Current->pNextNode;
      }
      else
      {
         m_Current = 0;
      }
   }

   CList<t>::SNode<t> * current() { return m_Current; }
   
   t & operator *() { return m_Current->nodeType; }
   void operator++(){ next(); }
   void operator++(int){ next(); }
   bool operator==( int i )
   { 
      if ( m_Current != 0 )
      {
         return i == (int)m_Current->nodeType; 
      }
      else
      {
         return i == 0;
      }
   }
   bool operator!=( int i )
   { 
      if ( m_Current != 0 )
      {
         return i != (int)m_Current->nodeType; 
      }
      else
      {
         return i != 0;
      }
   }
   
   bool end(){ return m_Current->next == 0; }

private:
   CList<t>::SNode<t> * m_Current;
   CList<t>::SNode<t> * m_ListHead;
};

#endif // !defined(GFXLIST_H__D05E31C8_0B8A_11D2_8210_006097A05989__INCLUDED_)
