#ifndef __PTRLIST
#define __PTRLIST

class CPtrNode
{
public:
	void*							m_pPtr;
	union
	{
		class CPtrNodeSingleLink*	m_psNext;
		class CPtrNodeDoubleLink*	m_pdNext;

		class CPtrNode*				m_pNext;	// for CPtrList methods
	};

public:
	CPtrNode(void* pItem)
		: m_pPtr(pItem)
	{
	}
};

class CPtrNodeSingleLink : public CPtrNode
{
public:
	CPtrNodeSingleLink(void* pItem)
		: CPtrNode(pItem)
	{
	}
};

class CPtrNodeDoubleLink : public CPtrNode
{
public:
	CPtrNodeDoubleLink*			m_pdPrev;

public:
	CPtrNodeDoubleLink(void* pItem)
		: CPtrNode(pItem)
	{
	}
};

class CPtrList
{
public:
	union
	{
		CPtrNodeSingleLink*		m_psNodes;
		CPtrNodeDoubleLink*		m_pdNodes;

		CPtrNode*				m_pNodes;	// for CPtrList methods
	};

	bool						IsMemberOfList(void* pItem)
	{
		if ( CPtrNode* pCurNode = m_pNodes )
		{
			while ( pCurNode->m_pPtr != pItem )
			{
				pCurNode->m_pNext;
				if ( !pCurNode )
					return false;
			}
			return true;
		}
		return false;
	};

	int							CountElements()
	{
		CPtrNode*	pCurMode = m_pNodes;
		int			nCount = 0;

		while ( pCurMode )
		{
			++nCount;
			pCurMode = pCurMode->m_pNext;
		}
		return nCount;
	};
};

class CPtrListSingleLink : public CPtrList
{
public:
	// TODO: NEEDS NEW/DELETE OVERLOADS & FLUSH METHOD
	CPtrNodeSingleLink*				AddItem(void* pItem)
	{
		CPtrNodeSingleLink*		pNewLink = new CPtrNodeSingleLink(pItem);

		pNewLink->m_psNext = m_psNodes;
		m_psNodes = pNewLink;

		return pNewLink;
	}

	void							DeleteItem(void* pItem)
	{
		CPtrNodeSingleLink*		pCurNode = m_psNodes;
		CPtrNodeSingleLink*		pPrevNode = nullptr;

		if ( pCurNode )
		{
			while ( pCurNode->m_pPtr != pItem )
			{
				pPrevNode = pCurNode;
				pCurNode = pCurNode->m_psNext;
				if ( !pCurNode )
					return;
			}

			if ( m_psNodes == pCurNode )
				m_psNodes = pCurNode->m_psNext;
			else if ( pPrevNode )
				pPrevNode->m_psNext = pCurNode->m_psNext;

			delete pCurNode;
		}
	}
};

class CPtrListDoubleLink : public CPtrList
{
public:
	// TODO: NEEDS NEW/DELETE OVERLOADS & FLUSH METHOD
	CPtrNodeDoubleLink*				AddItem(void* pItem)
	{
		CPtrNodeDoubleLink*		pNewLink = new CPtrNodeDoubleLink(pItem);

		pNewLink->m_pdNext = m_pdNodes;
		pNewLink->m_pdPrev = nullptr;

		if ( m_pdNodes )
			m_pdNodes->m_pdPrev = pNewLink;
		m_pdNodes = pNewLink;

		return pNewLink;
	}

	void							DeleteItem(void* pItem)
	{
		CPtrNodeDoubleLink*		pCurNode = m_pdNodes;

		if ( pCurNode )
		{
			while ( pCurNode->m_pPtr != pItem )
			{
				pCurNode = pCurNode->m_pdNext;
				if ( !pCurNode )
					return;
			}

			if ( m_pdNodes == pCurNode )
				m_pdNodes = pCurNode->m_pdNext;
			if ( pCurNode->m_pdPrev )
				pCurNode->m_pdPrev->m_pdNext = pCurNode->m_pdNext;
			if ( pCurNode->m_pdNext )
				pCurNode->m_pdNext->m_pdPrev = pCurNode->m_pdPrev;
			
			delete pCurNode;
		}
	}
};


#endif