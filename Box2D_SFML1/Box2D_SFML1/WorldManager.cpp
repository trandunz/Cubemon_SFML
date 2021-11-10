#include "WorldManager.h"

WorldManager::WorldManager(sf::RenderWindow* _renderWindow, b2World &_world, TextureMaster* _texturemaster)
{
	m_RenderWindow = _renderWindow;
	m_TextureMaster = _texturemaster;
	m_World = &_world;
}

WorldManager::~WorldManager()
{
	std::cout << "World Destroyed" << std::endl;

	CleanupTiles();
	CleanupTrees();
	CleanupGrass();
	DeletePointer(m_background);
	DeletePointer(m_Tile);
	DeletePointer(m_World);
	m_background = nullptr;
	m_Tile = nullptr;
	m_RenderWindow = nullptr;
	m_World = nullptr;
	m_TextureMaster = nullptr;
}

void WorldManager::Start(AudioManager* _audioManager)
{
	InitBackground(*m_TextureMaster->m_BackgroundTexture);

	ImportWorldFromINI();
}

void WorldManager::Update()
{
	for (Tile* tile : m_Tiles)
	{
		tile->Update();
	}

	// World Step
	m_World->Step(1 / 60.f, 60, 60);
}

void WorldManager::Render(sf::Shader* _defaultshader)
{
	m_RenderWindow->draw(*m_background, _defaultshader);

	float Mag1 = 0;
	float dx = 0;
	float dy = 0;
	
	// Everything Else
	for (int i = 0; i < m_Tiles.size(); i++)
	{
		dx = m_Tiles[i]->GetShape()->getPosition().x - m_RenderWindow->getView().getCenter().x;
		dy = m_Tiles[i]->GetShape()->getPosition().y - m_RenderWindow->getView().getCenter().y;
		Mag1 = Mag(dx, dy);
		if (Mag1 < 1200)
		{
			m_RenderWindow->draw(*m_Tiles[i]->GetShape(), _defaultshader);
		}
	}

	// Grass
	for (int i = 0; i < m_Grass.size(); i++)
	{
		dx = m_Grass[i]->GetShape()->getPosition().x - m_RenderWindow->getView().getCenter().x;
		dy = m_Grass[i]->GetShape()->getPosition().y - m_RenderWindow->getView().getCenter().y;
		Mag1 = Mag(dx, dy);
		if (Mag1 < 1200)
		{
			m_RenderWindow->draw(*m_Grass[i]->GetShape(), _defaultshader);
		}
	}

	// Trees
	for (int i = 0; i < m_Trees.size(); i++)
	{
		dx = m_Trees[i]->GetShape()->getPosition().x - m_RenderWindow->getView().getCenter().x;
		dy = m_Trees[i]->GetShape()->getPosition().y - m_RenderWindow->getView().getCenter().y;
		Mag1 = Mag(dx, dy);
		if (Mag1 < 1200)
		{
			m_RenderWindow->draw(*m_Trees[i]->GetShape(), _defaultshader);
		}
	}
}

void WorldManager::RenderOnTop()
{
	sf::Shader* _defaultshader = NULL;
	float Mag1 = 0;
	float dx = 0;
	float dy = 0;
	// Grass
	for (int i = 0; i < m_Grass.size(); i++)
	{
		dx = m_Grass[i]->GetShape()->getPosition().x - m_RenderWindow->getView().getCenter().x;
		dy = m_Grass[i]->GetShape()->getPosition().y - m_RenderWindow->getView().getCenter().y;
		Mag1 = Mag(dx, dy);
		
		if (Mag1 < 1200 && dy >= 0)
		{
			m_RenderWindow->draw(*m_Grass[i]->GetShape(), _defaultshader);
		}
	}

	// Trees
	for (int i = 0; i < m_Trees.size(); i++)
	{
		dx = m_Trees[i]->GetShape()->getPosition().x - m_RenderWindow->getView().getCenter().x;
		dy = m_Trees[i]->GetShape()->getPosition().y - m_RenderWindow->getView().getCenter().y;
		Mag1 = Mag(dx, dy);

		if (Mag1 < 1200 && dy >= 0)
		{
			m_RenderWindow->draw(*m_Trees[i]->GetShape(), _defaultshader);
		}
	}
}

void WorldManager::InitBackground(sf::Texture& _texture)
{
	std::cout << "Background Created" << std::endl;
	m_background = new sf::Sprite();
	m_background->setTexture(_texture, true);
	m_background->setTextureRect(sf::IntRect(0,0,10000,10000));

	m_background->setOrigin(m_background->getGlobalBounds().width / 2, m_background->getGlobalBounds().height / 2);
	m_background->setScale(7, 7);
	m_background->setPosition(sf::Vector2f(0, 2000));
}

void WorldManager::CleanupTiles()
{
	for (auto& pointer : m_Tiles)
	{
		DeletePointer(pointer);
		pointer = nullptr;
	}
	m_Tiles.erase(std::remove(m_Tiles.begin(), m_Tiles.end(), nullptr), m_Tiles.end());
}

void WorldManager::CleanupTrees()
{
	for (auto& pointer : m_Trees)
	{
		DeletePointer(pointer);
		pointer = nullptr;
	}
	m_Trees.erase(std::remove(m_Trees.begin(), m_Trees.end(), nullptr), m_Trees.end());
}

void WorldManager::CleanupGrass()
{
	for (auto& pointer : m_Grass)
	{
		DeletePointer(pointer);
		pointer = nullptr;
	}
	m_Grass.erase(std::remove(m_Grass.begin(), m_Grass.end(), nullptr), m_Grass.end());
}

void WorldManager::GrabTileTypes(std::vector<char>& _tileTypes, std::string _path)
{
	char m_Type = 0;
	std::ifstream file;
	std::string currentLine;
	file.open("Resources/Output/" + _path);
	if (file.is_open())
	{
		while (file.get(m_Type))
		{
			if (m_Type == ',')
			{
			}
			else
			{
				_tileTypes.push_back(m_Type);
			}
		}
		file.close();
	}
}

void WorldManager::ProcessTileTypes(std::vector<char>& _tileTypes)
{
	int iteratorX = -INISIZE;
	int iteratorY = -INISIZE;
	std::vector<char>::iterator it = _tileTypes.begin();
	while (it != _tileTypes.end())
	{
		if (*it == '0')
		{
			m_Tiles.push_back(new Tile(m_RenderWindow, *m_World, m_TextureMaster->m_DirtBlock_BG, sf::Vector2f((float)iteratorX * TILESIZE, (float)iteratorY * TILESIZE), sf::Vector2f(TILESIZE, TILESIZE), "Wall"));
			SelectRandomTypeFrom3(m_Tiles.back());
		}
		else if (*it == '~') // -- Water
		{
			m_Tiles.push_back(new Tile(m_RenderWindow, *m_World, m_TextureMaster->m_Water, sf::Vector2f((float)iteratorX * TILESIZE, (float)iteratorY * TILESIZE), sf::Vector2f(TILESIZE, TILESIZE), "Floor"));
		}
		else if (*it == 'i') // -- Tree
		{
			m_Tiles.push_back(new Tile(m_RenderWindow, *m_World, m_TextureMaster->m_GrassTexture, sf::Vector2f((float)iteratorX * TILESIZE, (float)iteratorY * TILESIZE), sf::Vector2f(TILESIZE, TILESIZE), "Floor"));
			SelectRandomTypeFrom3(m_Tiles.back());
			m_Trees.push_back(new Tile(m_RenderWindow, *m_World, m_TextureMaster->m_Tree, sf::Vector2f((float)iteratorX * TILESIZE, (float)iteratorY * TILESIZE), sf::Vector2f(TILESIZE, 1), "Wall"));
		}
		else if (*it == 'o') // -- Grass
		{
			m_Tiles.push_back(new Tile(m_RenderWindow, *m_World, m_TextureMaster->m_GrassTexture, sf::Vector2f((float)iteratorX * TILESIZE, (float)iteratorY * TILESIZE), sf::Vector2f(TILESIZE, TILESIZE), "Floor"));
			SelectRandomTypeFrom3(m_Tiles.back());
		}
		else if (*it == 'p') // -- Path
		{
			m_Tiles.push_back(new Tile(m_RenderWindow, *m_World, m_TextureMaster->m_DirtBlock_BG, sf::Vector2f((float)iteratorX * TILESIZE, (float)iteratorY * TILESIZE), sf::Vector2f(TILESIZE, TILESIZE), "Floor"));
			SelectRandomTypeFrom3(m_Tiles.back());
		}
		else if (*it == 'l') // -- Tree
		{
			m_Tiles.push_back(new Tile(m_RenderWindow, *m_World, m_TextureMaster->m_DirtBlock_BG, sf::Vector2f((float)iteratorX * TILESIZE, (float)iteratorY * TILESIZE), sf::Vector2f(TILESIZE, TILESIZE), "Floor"));
			SelectRandomTypeFrom3(m_Tiles.back());
			m_Trees.push_back(new Tile(m_RenderWindow, *m_World, m_TextureMaster->m_Tree, sf::Vector2f((float)iteratorX * TILESIZE, (float)iteratorY * TILESIZE), sf::Vector2f(TILESIZE, 1), "Wall"));
		}
		else if (*it == '?') // -- Grass on Grass
		{
			m_Tiles.push_back(new Tile(m_RenderWindow, *m_World, m_TextureMaster->m_GrassTexture, sf::Vector2f((float)iteratorX * TILESIZE, (float)iteratorY * TILESIZE), sf::Vector2f(TILESIZE, TILESIZE), "Floor"));
			SelectRandomTypeFrom3(m_Tiles.back());
			m_Grass.push_back(new Tile(m_RenderWindow, *m_World, m_TextureMaster->m_Grass, sf::Vector2f((float)iteratorX * TILESIZE, (float)iteratorY * TILESIZE), sf::Vector2f(1, 1), "Event"));
			SelectFirstFromSheet(m_Grass.back());
		}
		else if (*it == 's') // -- Grass On Path
		{
			m_Tiles.push_back(new Tile(m_RenderWindow, *m_World, m_TextureMaster->m_DirtBlock_BG, sf::Vector2f((float)iteratorX * TILESIZE, (float)iteratorY * TILESIZE), sf::Vector2f(TILESIZE, TILESIZE), "Floor"));
			SelectRandomTypeFrom3(m_Tiles.back());
			m_Grass.push_back(new Tile(m_RenderWindow, *m_World, m_TextureMaster->m_Grass, sf::Vector2f((float)iteratorX * TILESIZE, (float)iteratorY * TILESIZE), sf::Vector2f(1, 1), "Event"));
			SelectFirstFromSheet(m_Grass.back());
		}
		else if (*it == '^') // -- Rockface
		{
			m_Tiles.push_back(new Tile(m_RenderWindow, *m_World, m_TextureMaster->m_Rock, sf::Vector2f((float)iteratorX * TILESIZE, (float)iteratorY * TILESIZE), sf::Vector2f(TILESIZE, TILESIZE), "Wall"));
		}
		else if (*it == 'c') // -- Cave Entrance
		{
			m_Grass.push_back(new Tile(m_RenderWindow, *m_World, m_TextureMaster->m_CaveEntrance, sf::Vector2f((float)iteratorX * TILESIZE, ((float)iteratorY * TILESIZE) - 25) , sf::Vector2f(TILESIZE, TILESIZE), "Event"));
		}

		// Dirt Water Sheets
		else if (*it == '1') // BL
		{
			m_Tiles.push_back(new Tile(m_RenderWindow, *m_World, m_TextureMaster->DirtToWaterSheet, sf::Vector2f((float)iteratorX * TILESIZE, ((float)iteratorY * TILESIZE) - 25), sf::Vector2f(TILESIZE, TILESIZE), "Wall"));
			m_Tiles.back()->GetShape()->setTextureRect(sf::IntRect(0,200,100,100));
			m_Tiles.back()->GetShape()->setOrigin(50, 50);
		}
		else if (*it == '2') // BM
		{
			m_Tiles.push_back(new Tile(m_RenderWindow, *m_World, m_TextureMaster->DirtToWaterSheet, sf::Vector2f((float)iteratorX * TILESIZE, ((float)iteratorY * TILESIZE) - 25), sf::Vector2f(TILESIZE, TILESIZE), "Wall"));
			m_Tiles.back()->GetShape()->setTextureRect(sf::IntRect(100,200,100,100));
			m_Tiles.back()->GetShape()->setOrigin(50, 50);
		}
		else if (*it == '3') // BR
		{
			m_Tiles.push_back(new Tile(m_RenderWindow, *m_World, m_TextureMaster->DirtToWaterSheet, sf::Vector2f((float)iteratorX * TILESIZE, ((float)iteratorY * TILESIZE) - 25), sf::Vector2f(TILESIZE, TILESIZE), "Wall"));
			m_Tiles.back()->GetShape()->setTextureRect(sf::IntRect(200, 200,100,100));
			m_Tiles.back()->GetShape()->setOrigin(50, 50);
		}
		else if (*it == '4') // ML
		{
			m_Tiles.push_back(new Tile(m_RenderWindow, *m_World, m_TextureMaster->DirtToWaterSheet, sf::Vector2f((float)iteratorX * TILESIZE, ((float)iteratorY * TILESIZE) - 25), sf::Vector2f(TILESIZE, TILESIZE), "Wall"));
			m_Tiles.back()->GetShape()->setTextureRect(sf::IntRect(0,100,100,100));
			m_Tiles.back()->GetShape()->setOrigin(50, 50);
		}
		else if (*it == '5') // MM
		{
			m_Tiles.push_back(new Tile(m_RenderWindow, *m_World, m_TextureMaster->DirtToWaterSheet, sf::Vector2f((float)iteratorX * TILESIZE, ((float)iteratorY * TILESIZE) - 25), sf::Vector2f(TILESIZE, TILESIZE), "Wall"));
			m_Tiles.back()->GetShape()->setTextureRect(sf::IntRect(100,100,100,100));
			m_Tiles.back()->GetShape()->setOrigin(50, 50);
		}
		else if (*it == '6') // MR
		{
			m_Tiles.push_back(new Tile(m_RenderWindow, *m_World, m_TextureMaster->DirtToWaterSheet, sf::Vector2f((float)iteratorX * TILESIZE, ((float)iteratorY * TILESIZE) - 25), sf::Vector2f(TILESIZE, TILESIZE), "Wall"));
			m_Tiles.back()->GetShape()->setTextureRect(sf::IntRect(200, 100, 100,100));
			m_Tiles.back()->GetShape()->setOrigin(50, 50);
		}
		else if (*it == '7') // TL
		{
			m_Tiles.push_back(new Tile(m_RenderWindow, *m_World, m_TextureMaster->DirtToWaterSheet, sf::Vector2f((float)iteratorX * TILESIZE, ((float)iteratorY * TILESIZE) - 25), sf::Vector2f(TILESIZE, TILESIZE), "Wall"));
			m_Tiles.back()->GetShape()->setTextureRect(sf::IntRect(0,0,100,100));
			m_Tiles.back()->GetShape()->setOrigin(50, 50);
		}
		else if (*it == '8') // TM
		{
			m_Tiles.push_back(new Tile(m_RenderWindow, *m_World, m_TextureMaster->DirtToWaterSheet, sf::Vector2f((float)iteratorX * TILESIZE, ((float)iteratorY * TILESIZE) - 25), sf::Vector2f(TILESIZE, TILESIZE), "Wall"));
			m_Tiles.back()->GetShape()->setTextureRect(sf::IntRect(100,0,100,100));
			m_Tiles.back()->GetShape()->setOrigin(50, 50);
		}
		else if (*it == '9') // TR
		{
			m_Tiles.push_back(new Tile(m_RenderWindow, *m_World, m_TextureMaster->DirtToWaterSheet, sf::Vector2f((float)iteratorX * TILESIZE, ((float)iteratorY * TILESIZE) - 25), sf::Vector2f(TILESIZE, TILESIZE), "Wall"));
			m_Tiles.back()->GetShape()->setTextureRect(sf::IntRect(200,0,100,100));
			m_Tiles.back()->GetShape()->setOrigin(50, 50);
		}

		if (iteratorX == INISIZE)
		{
			iteratorX = -INISIZE;
			iteratorY++;
		}
		else
		{
			iteratorX++;
		}

		it++;
	}
}

void WorldManager::SelectRandomTypeFrom3(Tile* _tile)
{
	unsigned i = 1 + rand() % 3;

	if (i == 1)
	{
		_tile->GetShape()->setTextureRect(sf::IntRect(0, 0, 100, 100));
	}
	else if (i == 2)
	{
		_tile->GetShape()->setTextureRect(sf::IntRect(100, 0, 100, 100));
	}
	else if (i == 3)
	{
		_tile->GetShape()->setTextureRect(sf::IntRect(200, 0, 100, 100));
	}
	_tile->GetShape()->setOrigin(50, 50);
}

void WorldManager::SelectFirstFromSheet(Tile* _tile)
{
	_tile->GetShape()->setTextureRect(sf::IntRect(0, 0, 100, 100));
	_tile->GetShape()->setOrigin(50, 50);
}

void WorldManager::ImportWorldFromINI()
{
	CleanupTiles();

	std::vector<char> m_TileTypes;
	GrabTileTypes(m_TileTypes, "Level_Ash.ini");

	ProcessTileTypes(m_TileTypes);
}
