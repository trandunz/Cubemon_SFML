#pragma once
#ifndef _TEXTUREMASTER_H__
#define _TEXTUREMASTER_H__

#include "NumptyBehavior.h"

class TextureMaster : public NumptyBehavior
{
public:
	TextureMaster();
	virtual ~TextureMaster();

	sf::Texture* m_BackgroundTexture = nullptr;
	sf::Texture* m_GrassTexture = nullptr;
	sf::Texture* m_DirtBlock_BG = nullptr;
	sf::Texture* m_Grass = nullptr;
	sf::Texture* m_DustyBush = nullptr;
	sf::Texture* m_Tree = nullptr;
	sf::Texture* m_Water = nullptr;
	sf::Texture* m_Rock = nullptr;
	sf::Texture* m_CaveEntrance = nullptr;
	sf::Texture* m_Church = nullptr;
	sf::Texture* m_IndoorFloor = nullptr;
	sf::Texture* m_IndoorWall = nullptr;
	sf::Texture* m_IndoorDoor = nullptr;
	sf::Texture* m_BlackBG = nullptr;
	sf::Texture* m_HealingMachine = nullptr;
	sf::Texture* m_OldMan = nullptr;
	sf::Texture* m_GymLeader = nullptr;

	sf::Texture* DirtToGrassSheet = nullptr;
	sf::Texture* GrassToDirtSheet = nullptr;
	sf::Texture* DirtToWaterSheet = nullptr;
	sf::Texture* WaterToDirtSheet = nullptr;
	sf::Texture* GrassToWaterSheet = nullptr;
	sf::Texture* WaterToGrassSheet = nullptr;
};

#endif