#pragma once
#include "NumptyBehavior.h"
class IItem : public NumptyBehavior
{
public:
	enum class ITEMTYPE
	{
		UNASSIGNED = 0,

		POTION,
		MONCUBE,
		RING,
	};

	virtual void Start();
	virtual void Update();
	virtual void Render();

	virtual void CleanupAllPointers();

	ITEMTYPE m_ItemType = ITEMTYPE::UNASSIGNED;

protected:
	void CreateSprite(ITEMTYPE _itemType = ITEMTYPE::UNASSIGNED);
	void CreateTexture(ITEMTYPE _itemType = ITEMTYPE::UNASSIGNED);
	void SetPosition(sf::Vector2f _position);
	
	sf::RenderWindow* m_RenderWindow = nullptr;
	sf::Sprite* m_Shape = nullptr;
	sf::Texture* m_Texture = nullptr;
};

