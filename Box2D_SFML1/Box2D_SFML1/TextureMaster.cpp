#include "TextureMaster.h"

TextureMaster::TextureMaster()
{
	// Textures
	m_BackgroundTexture = new sf::Texture();
	m_BackgroundTexture->setSmooth(true);
	m_BackgroundTexture->setRepeated(true);

	m_GrassTexture = new sf::Texture();
	m_GrassTexture->setRepeated(true);

	m_DirtBlock_BG = new sf::Texture();

	m_Grass = new sf::Texture();
	m_Rock = new sf::Texture();
	m_Tree = new sf::Texture();
	m_CaveEntrance = new sf::Texture();

	m_Water = new sf::Texture();
	m_Water->setRepeated(true);

	DirtToGrassSheet = new sf::Texture();
	GrassToDirtSheet = new sf::Texture();
	DirtToWaterSheet = new sf::Texture();
	WaterToDirtSheet = new sf::Texture();
	GrassToWaterSheet = new sf::Texture();
	WaterToGrassSheet = new sf::Texture();

	if (m_BackgroundTexture->loadFromFile("Resources/Images/Backgrounds/OriBG.jpg"))
	{
		std::cout << "Background Texture Loaded!" << std::endl;
	}
	if (m_GrassTexture->loadFromFile("Resources/Images/Custom Sprites/Lush_Grass_Sheet.png"))
	{
		std::cout << "Grass Texture Loaded!" << std::endl;
	}
	if (m_DirtBlock_BG->loadFromFile("Resources/Images/Custom Sprites/Dirt_Sheet.png"))
	{
		std::cout << "Dirt_BG Texture Loaded!" << std::endl;
	}
	if (m_Grass->loadFromFile("Resources/Images/Custom Sprites/Lush_Bush_Sheet_02.png"))
	{
		std::cout << "Grass Texture Loaded!" << std::endl;
	}
	if (m_Tree->loadFromFile("Resources/Images/Tree.png"))
	{
		std::cout << "Tree Texture Loaded!" << std::endl;
	}
	if (m_Water->loadFromFile("Resources/Images/Custom Sprites/Water_01.png"))
	{
		std::cout << "Water Texture Loaded!" << std::endl;
	}
	if (m_Rock->loadFromFile("Resources/Images/Rock.png"))
	{
		std::cout << "Rock Texture Loaded!" << std::endl;
	}
	if (m_CaveEntrance->loadFromFile("Resources/Images/CaveEntrance.png"))
	{
		std::cout << "Cave Entrance Texture Loaded!" << std::endl;
	}

	if (DirtToGrassSheet->loadFromFile("Resources/Images/Custom Sprites/Dirt_Lush_Grass_Sheet.png"))
	{
		std::cout << "DirtToGrassSheet Texture Loaded!" << std::endl;
	}
	if (GrassToDirtSheet->loadFromFile("Resources/Images/Custom Sprites/Lush_Grass_Dirt_Sheet.png"))
	{
		std::cout << "GrassToDirtSheet Texture Loaded!" << std::endl;
	}
	if (DirtToWaterSheet->loadFromFile("Resources/Images/Custom Sprites/Dirt_Water_Sheet.png"))
	{
		std::cout << "DirtToWaterSheet Texture Loaded!" << std::endl;
	}
	if (WaterToDirtSheet->loadFromFile("Resources/Images/Custom Sprites/Water_Dirt_Sheet.png"))
	{
		std::cout << "WaterToDirtSheet Texture Loaded!" << std::endl;
	}
	if (GrassToWaterSheet->loadFromFile("Resources/Images/Custom Sprites/Lush_Grass_Water_Sheet.png"))
	{
		std::cout << "GrassToWaterSheet Texture Loaded!" << std::endl;
	}
	if (WaterToGrassSheet->loadFromFile("Resources/Images/Custom Sprites/Water_Lush_Grass_Sheet.png"))
	{
		std::cout << "WaterToGrassSheet Texture Loaded!" << std::endl;
	}
}

TextureMaster::~TextureMaster()
{
	DeletePointer(m_BackgroundTexture);
	m_BackgroundTexture = nullptr;
	DeletePointer(m_GrassTexture);
	m_GrassTexture = nullptr;
	DeletePointer(m_DirtBlock_BG);
	m_DirtBlock_BG = nullptr;
	DeletePointer(m_Grass);
	m_Grass = nullptr;
	DeletePointer(m_Tree);
	m_Tree = nullptr;
	DeletePointer(m_Water);
	m_Water = nullptr;
	DeletePointer(m_Rock);
	m_Rock = nullptr;
	DeletePointer(m_CaveEntrance);
	m_CaveEntrance = nullptr;

	DeletePointer(DirtToGrassSheet);
	DirtToGrassSheet = nullptr;
	DeletePointer(GrassToDirtSheet);
	GrassToDirtSheet = nullptr;
	DeletePointer(DirtToWaterSheet);
	DirtToWaterSheet = nullptr;
	DeletePointer(WaterToDirtSheet);
	WaterToDirtSheet = nullptr;
	DeletePointer(GrassToWaterSheet);
	GrassToWaterSheet = nullptr;
	DeletePointer(WaterToGrassSheet);
	WaterToGrassSheet = nullptr;
}
