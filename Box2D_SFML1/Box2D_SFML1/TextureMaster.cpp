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
	m_Church = new sf::Texture();
	m_IndoorFloor = new sf::Texture();
	m_IndoorWall = new sf::Texture();
	m_IndoorDoor = new sf::Texture();
	m_BlackBG = new sf::Texture();
	m_HealingMachine = new sf::Texture();
	m_OldMan = new sf::Texture();
	m_GymLeader = new sf::Texture();
	m_DustyBush = new sf::Texture();

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

	if (m_Church->loadFromFile("Resources/Images/Custom Sprites/PokeHospital.png"))
	{
		std::cout << "CubeHospital Texture Loaded!" << std::endl;
	}
	if (m_IndoorFloor->loadFromFile("Resources/Images/Custom Sprites/Floor.png"))
	{
		std::cout << "Indoor Floor Texture Loaded!" << std::endl;
	}
	if (m_IndoorWall->loadFromFile("Resources/Images/Custom Sprites/Wall.png"))
	{
		std::cout << "Indoor Wall Texture Loaded!" << std::endl;
	}
	if (m_IndoorDoor->loadFromFile("Resources/Images/Custom Sprites/FloorDoorway.png"))
	{
		std::cout << "Indoor Doorway Texture Loaded!" << std::endl;
	}
	if (m_BlackBG->loadFromFile("Resources/Images/Custom Sprites/Black.png"))
	{
		std::cout << "BlackBG Texture Loaded!" << std::endl;
	}
	if (m_HealingMachine->loadFromFile("Resources/Images/Custom Sprites/HealingMachine.png"))
	{
		std::cout << "Healing Machine Texture Loaded!" << std::endl;
	}
	if (m_OldMan->loadFromFile("Resources/Images/Player/Old_Man.png"))
	{
		std::cout << "Old Man Texture Loaded!" << std::endl;
	}
	if (m_GymLeader->loadFromFile("Resources/Images/Player/RedGymGuy.png"))
	{
		std::cout << "Gym Leader Texture Loaded!" << std::endl;
	}
	if (m_DustyBush->loadFromFile("Resources/Images/Custom Sprites/DustyBush.png"))
	{
		std::cout << "Dusty Bush Texture Loaded!" << std::endl;
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

	DeletePointer(m_Church);
	m_Church = nullptr;
	DeletePointer(m_IndoorFloor);
	m_IndoorFloor = nullptr;
	DeletePointer(m_IndoorWall);
	m_IndoorWall = nullptr;
	DeletePointer(m_IndoorDoor);
	m_IndoorDoor = nullptr;
	DeletePointer(m_BlackBG);
	m_BlackBG = nullptr;

	DeletePointer(m_HealingMachine);
	m_HealingMachine = nullptr;

	DeletePointer(m_OldMan);
	m_OldMan = nullptr;

	DeletePointer(m_GymLeader);
	m_GymLeader = nullptr;
	DeletePointer(m_DustyBush);
	m_DustyBush = nullptr;

}
