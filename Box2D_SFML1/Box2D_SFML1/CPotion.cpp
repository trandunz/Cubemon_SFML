#include "CPotion.h"

CPotion::CPotion()
{
	m_ItemType = ITEMTYPE::POTION;
}

CPotion::~CPotion()
{
	CleanupAllPointers();
}
