#include "IItem.h"

void IItem::Start()
{
	CreateSprite(m_ItemType);
	CreateTexture(m_ItemType);
}

void IItem::Update()
{
}

void IItem::Render()
{
	m_RenderWindow->draw(*m_Shape);
}

void IItem::CleanupAllPointers()
{
	m_RenderWindow = nullptr;
	DeletePointer(m_Shape);
	m_Shape = nullptr;
	DeletePointer(m_Texture);
	m_Texture = nullptr;
}

void IItem::CreateSprite(ITEMTYPE _itemType)
{
	DeletePointer(m_Shape);
	DeletePointer(m_Texture);
	m_Shape = nullptr;
	m_Texture = nullptr;
	m_Shape = new sf::Sprite();
	CreateTexture(_itemType);
}

void IItem::CreateTexture(ITEMTYPE _itemType)
{
	DeletePointer(m_Texture);
	m_Texture = nullptr;
	m_Texture = new sf::Texture();

	switch (_itemType)
	{
	case ITEMTYPE::UNASSIGNED:
	{
		break;
	}
	case ITEMTYPE::POTION:
	{
		LoadTexture(m_Texture, "Potion.png");
		break;
	}
	case ITEMTYPE::MONCUBE:
	{
		LoadTexture(m_Texture, "MonCube.png");
		break;
	}
	case ITEMTYPE::RING:
	{
		LoadTexture(m_Texture, "Ring.png");
		break;
	}
	}
}

void IItem::SetPosition(sf::Vector2f _position)
{
	
	m_Shape->setPosition(_position);
}
