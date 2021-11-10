#pragma once
#ifndef _WORLDMANAGER_H__
#define _WORLDMANAGER_H__

#define INISIZE 40
#define TILESIZE 100

#include "NumptyBehavior.h"

// Local Includes
#include "TextureMaster.h"
#include "Player.h"
#include "Tile.h"

class WorldManager : public NumptyBehavior
{
public:
	WorldManager(sf::RenderWindow* _renderWindow, b2World& _world, TextureMaster* _texturemaste);
	virtual ~WorldManager();

	virtual void Start(AudioManager* _audioManager);
	virtual void Update();
	virtual void Render(sf::Shader* _defaultshader = NULL);
	void RenderOnTop();

	void InitBackground(sf::Texture& _texture); 

	void ImportWorldFromINI();
	void CleanupTiles();
	void CleanupTrees();
	void CleanupGrass();

private:
	void GrabTileTypes(std::vector<char>& _tileTypes, std::string _path);
	void ProcessTileTypes(std::vector<char>& _tileTypes);

	void SelectRandomTypeFrom3(Tile* _tile);
	void SelectFirstFromSheet(Tile* _tile);

	std::vector<Tile*> m_Tiles = {};
	std::vector<Tile*> m_Trees = {};
	std::vector<Tile*> m_Grass = {};
	Tile* m_Tile = nullptr;

	TextureMaster* m_TextureMaster = nullptr;
	sf::RenderWindow* m_RenderWindow = nullptr;
	sf::Sprite* m_background = nullptr;
	b2World* m_World = nullptr;
};
#endif