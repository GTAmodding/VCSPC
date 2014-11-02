#ifndef __MENUSYSTEM
#define __MENUSYSTEM

#define NUM_ITEMS_IN_MENUSYSTEM		12
#define NUM_COLUMNS_IN_MENUSYSTEM	4

class CMenuSystem
{
public:
	struct Menu
	{
		unsigned char		m_nColoursInGrid[64];
		unsigned char		m_nType;
		char				m_aFields[NUM_COLUMNS_IN_MENUSYSTEM][NUM_ITEMS_IN_MENUSYSTEM][10];
		int					m_nNumber[2][NUM_COLUMNS_IN_MENUSYSTEM][NUM_ITEMS_IN_MENUSYSTEM];
		char				m_aColumnTitle[NUM_COLUMNS_IN_MENUSYSTEM][10];
		char				m_aTitle[10];
		bool				m_bActive[NUM_ITEMS_IN_MENUSYSTEM];
		bool				m_bHighlighted[NUM_ITEMS_IN_MENUSYSTEM];
		unsigned char		m_nOrientation[NUM_COLUMNS_IN_MENUSYSTEM];
		unsigned char		m_nHeaderOrientation[NUM_COLUMNS_IN_MENUSYSTEM];
		unsigned char		m_nNumItems;
		unsigned char		m_nNumColumns;
		bool				m_bInteractive;
		bool				m_bDPadInput;
		float				m_fWidth[NUM_COLUMNS_IN_MENUSYSTEM];
		float				m_fPosX;
		float				m_fPosY;
		bool				m_bBackground;
		unsigned char		m_nSelectedMenuItem;
		unsigned char		m_nAcceptedItem;
	};

private:
	static Menu** const		MenuInUse;

public:
	static void				DisplayStandardMenu(unsigned char nMenuID, bool bActiveColour);
};

static_assert(sizeof(CMenuSystem::Menu) == 0x418, "Wrong size: CMenuSystem::Menu");



#endif