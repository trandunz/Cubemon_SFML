#include "CBattleScene.h"
#include "CDebugWindow.h"

CBattleScene::CBattleScene(sf::RenderWindow* _renderWindow, TextureMaster* _textureMaster, sf::Event& _event, ICubemon::CUBEMONTYPE  _enemyType1, ICubemon::CUBEMONTYPE  _enemyType2, ICubemon::CUBEMONTYPE  _enemyType3, bool _custom)
{
	m_RenderWindow = _renderWindow;
	m_TextureMaster = _textureMaster;
	m_Event = &_event;
	m_Font.loadFromFile("Resources/Fonts/ANDYB.TTF");
	m_PreDefEnemyType1 = _enemyType1;
	m_PreDefEnemyType2 = _enemyType2;
	m_PreDefEnemyType3 = _enemyType3;
	m_bCustom = _custom;
}

CBattleScene::~CBattleScene()
{
	SaveCubemonValues();
	
	if (IsPlayerDeath())
	{
		Player::HealAllPokemon();
	}

	CleanupAllPointers();
}

void CBattleScene::Start()
{
	if (!m_bCustom)
	{
		InterceptSceneChange(-1);
	}

	InitUIView();
	InitWorldView();
	InitBackground();

	CreateAudioManager();
	m_AudioManager->PlayMusic(-1);

	CreateGUI();
	if (m_bCustom)
	{
		if (m_GUI->GetCurrentCubemon() == ICubemon::CUBEMONTYPE::THALLIC)
		{
			m_FriendlyCubemon = new CThallic(m_RenderWindow, m_AudioManager);
		}
		else if (m_GUI->GetCurrentCubemon() == ICubemon::CUBEMONTYPE::KINDLING)
		{
			m_FriendlyCubemon = new CKindling(m_RenderWindow, m_AudioManager);
		}
		else if (m_GUI->GetCurrentCubemon() == ICubemon::CUBEMONTYPE::BRUTUS)
		{
			m_FriendlyCubemon = new CBrutus(m_RenderWindow, m_AudioManager);
		}
		else if (m_GUI->GetCurrentCubemon() == ICubemon::CUBEMONTYPE::WIRLSON)
		{
			m_FriendlyCubemon = new CWirlson(m_RenderWindow, m_AudioManager);
		}
		else if (m_GUI->GetCurrentCubemon() == ICubemon::CUBEMONTYPE::DUSTDEVIL)
		{
			m_FriendlyCubemon = new CDustDevil(m_RenderWindow, m_AudioManager);
		}
		else if (m_GUI->GetCurrentCubemon() == ICubemon::CUBEMONTYPE::BLIZZARDBIRD)
		{
			m_FriendlyCubemon = new CBlizzardBird(m_RenderWindow, m_AudioManager);
		}
		if (m_FriendlyCubemon != nullptr)
		{
			m_FriendlyCubemon->SetCurrentHealth(GrabCubemonHealthBasedOnType());
			m_FriendlyCubemon->SetLevel(GrabCubemonLevelBasedOnType());
			m_FriendlyCubemon->SetXP(GrabCubemonEXPBasedOnType());
			m_FriendlyCubemon->SetSpritePos(sf::Vector2f(m_RenderWindow->getView().getCenter().x - m_RenderWindow->getView().getSize().x / 3.5, m_RenderWindow->getView().getCenter().y + m_RenderWindow->getView().getSize().y / 7));
			m_FriendlyCubemon->SetSpriteScale(sf::Vector2f(3, 3));
		}

		InitCustomBattleTypeOne();
		InitCustomBattleTypeTwo();
		InitCustomBattleTypeThree();

		if (m_EnemyCubemonVector.back() != nullptr)
		{
			m_EnemyCubemonVector.back()->SetSpritePos(sf::Vector2f(m_RenderWindow->getView().getCenter().x + m_RenderWindow->getView().getSize().x / 6, m_RenderWindow->getView().getCenter().y - m_RenderWindow->getView().getSize().y / 6));
			m_EnemyCubemonVector.back()->SetSpriteScale(sf::Vector2f(1.5, 1.5));
		}

		m_GUI->InitCubemonHUD(m_FriendlyCubemon, m_EnemyCubemonVector.back());
	}
	else
	{
		InitCubemon();
		m_GUI->InitCubemonHUD(m_FriendlyCubemon, m_EnemyCubemon);
	}
}

void CBattleScene::Update()
{
	if (m_bCustom)
	{
		m_EnemyCubemonVector.back()->Update();
	}
	else
	{
		if (m_EnemyCubemon)
		{
			m_EnemyCubemon->Update();
		}
	}

	if (m_FriendlyCubemon != nullptr)
	{
		m_FriendlyCubemon->Update();
	}

	if (m_FriendlyCubemon->GetCurrentHealth() <= 0)
	{
		if (IsPlayerDeath())
		{
			ResetPlayerPosition();

			InterceptSceneChange(1);
		}
		else if (m_GUI->m_bChangePokemon == true)
		{
			ChangePokemon(m_GUI->ReturnPokemonChangeType());
		}
		else
		{
			m_GUI->m_bCubedex = true;
		}
	}
	if (m_GUI)
	{
		if (m_GUI->m_bChangePokemon == true)
		{
			ChangePokemon(m_GUI->ReturnPokemonChangeType());
		}
		else if (m_bCustom)
		{
			if (m_EnemyCubemonVector.size() > 0)
			{
				m_EnemyCubemon = m_EnemyCubemonVector.back();
			}
			else
			{
				m_EnemyCubemon = nullptr;
			}
		}
	}

	SaveCubemonValues();
}

void CBattleScene::PolledUpdate()
{
	while (m_RenderWindow->pollEvent(*m_Event))
	{
		if (m_Event->type == sf::Event::Closed)
		{
			m_RenderWindow->close();
			return;
		}
	}
}

void CBattleScene::Render()
{
	m_RenderWindow->draw(m_Background);

	if (m_bCustom)
	{
		m_EnemyCubemonVector.back()->Render();
	}
	else
	{
		if (m_EnemyCubemon != nullptr)
		{
			m_EnemyCubemon->Render();
		}
	}

	if (m_FriendlyCubemon != nullptr)
	{
		m_FriendlyCubemon->Render();
	}

	if (m_GUI != nullptr)
	{
		if (m_bCustom)
		{
			m_GUI->BattleUI(m_UIView, m_WorldView, &m_EnemyCubemonVector);
		}
		else
		{
			m_GUI->BattleUI(m_UIView, m_WorldView);
		}
		
		m_GUI->HandleCubemonHUD(m_UIView, m_WorldView);
	}

	m_FriendlyCubemon->EXPLvlUpCheck();
}

void CBattleScene::CheckForMARKASDESTROY()
{
}

void CBattleScene::InitCubemon(ICubemon::CUBEMONTYPE _friendly, ICubemon::CUBEMONTYPE _enemy)
{
	if (_enemy == ICubemon::CUBEMONTYPE::THALLIC)
	{
		m_EnemyCubemon = new CThallic(m_RenderWindow, m_AudioManager);
	}
	else if (_enemy == ICubemon::CUBEMONTYPE::KINDLING)
	{
		m_EnemyCubemon = new CKindling(m_RenderWindow, m_AudioManager);
	}
	else if (_enemy == ICubemon::CUBEMONTYPE::BRUTUS)
	{
		m_EnemyCubemon = new CBrutus(m_RenderWindow, m_AudioManager);
	}
	else if (_enemy == ICubemon::CUBEMONTYPE::WIRLSON)
	{
		m_EnemyCubemon = new CWirlson(m_RenderWindow, m_AudioManager);
	}
	else if (_enemy == ICubemon::CUBEMONTYPE::DUSTDEVIL)
	{
		m_EnemyCubemon = new CDustDevil(m_RenderWindow, m_AudioManager);
	}
	else if (_enemy == ICubemon::CUBEMONTYPE::BLIZZARDBIRD)
	{
		m_EnemyCubemon = new CBlizzardBird(m_RenderWindow, m_AudioManager);
	}
	else
	{
		int ran = INT_MAX;
		srand((unsigned)time(NULL));
		srand(rand());
		ran = rand() % 6;
		if (ran == 0)
		{
			m_EnemyCubemon = new CThallic(m_RenderWindow, m_AudioManager);
			m_EnemyCubemon->SetLevel(ran + 1);
		}
		else if (ran == 1)
		{
			m_EnemyCubemon = new CKindling(m_RenderWindow, m_AudioManager);
			m_EnemyCubemon->SetLevel(ran + 1);
		}
		else if (ran == 2)
		{
			m_EnemyCubemon = new CBrutus(m_RenderWindow, m_AudioManager);
			m_EnemyCubemon->SetLevel(ran + 1);
		}
		else if (ran == 3)
		{
			m_EnemyCubemon = new CWirlson(m_RenderWindow, m_AudioManager);
			m_EnemyCubemon->SetLevel(ran + 1);
		}
		else if (ran == 4)
		{
			m_EnemyCubemon = new CDustDevil(m_RenderWindow, m_AudioManager);
			m_EnemyCubemon->SetLevel(ran + 1);
		}
		else if (ran == 5)
		{
			m_EnemyCubemon = new CBlizzardBird(m_RenderWindow, m_AudioManager);
			m_EnemyCubemon->SetLevel(ran + 1);
		}
	}

	if (_friendly == ICubemon::CUBEMONTYPE::THALLIC)
	{
		m_FriendlyCubemon = new CThallic(m_RenderWindow, m_AudioManager);
	}
	else if (_friendly == ICubemon::CUBEMONTYPE::KINDLING)
	{
		m_FriendlyCubemon = new CKindling(m_RenderWindow, m_AudioManager);
	}
	else if (_friendly == ICubemon::CUBEMONTYPE::BRUTUS)
	{
		m_FriendlyCubemon = new CBrutus(m_RenderWindow, m_AudioManager);
	}
	else if (_friendly == ICubemon::CUBEMONTYPE::WIRLSON)
	{
		m_FriendlyCubemon = new CWirlson(m_RenderWindow, m_AudioManager);
	}
	else if (_friendly == ICubemon::CUBEMONTYPE::DUSTDEVIL)
	{
		m_FriendlyCubemon = new CDustDevil(m_RenderWindow, m_AudioManager);
	}
	else if (_friendly == ICubemon::CUBEMONTYPE::BLIZZARDBIRD)
	{
		m_FriendlyCubemon = new CBlizzardBird(m_RenderWindow, m_AudioManager);
	}
	else
	{
		if (m_GUI->GetCurrentCubemon() == ICubemon::CUBEMONTYPE::THALLIC)
		{
			m_FriendlyCubemon = new CThallic(m_RenderWindow, m_AudioManager);
		}
		else if (m_GUI->GetCurrentCubemon() == ICubemon::CUBEMONTYPE::KINDLING)
		{
			m_FriendlyCubemon = new CKindling(m_RenderWindow, m_AudioManager);
		}
		else if (m_GUI->GetCurrentCubemon() == ICubemon::CUBEMONTYPE::BRUTUS)
		{
			m_FriendlyCubemon = new CBrutus(m_RenderWindow, m_AudioManager);
		}
		else if (m_GUI->GetCurrentCubemon() == ICubemon::CUBEMONTYPE::WIRLSON)
		{
			m_FriendlyCubemon = new CWirlson(m_RenderWindow, m_AudioManager);
		}
		else if (m_GUI->GetCurrentCubemon() == ICubemon::CUBEMONTYPE::DUSTDEVIL)
		{
			m_FriendlyCubemon = new CDustDevil(m_RenderWindow, m_AudioManager);
		}
		else if (m_GUI->GetCurrentCubemon() == ICubemon::CUBEMONTYPE::BLIZZARDBIRD)
		{
			m_FriendlyCubemon = new CBlizzardBird(m_RenderWindow, m_AudioManager);
		}
	}

	if (m_EnemyCubemon != nullptr)
	{
		m_EnemyCubemon->SetSpritePos(sf::Vector2f(m_RenderWindow->getView().getCenter().x + m_RenderWindow->getView().getSize().x / 6, m_RenderWindow->getView().getCenter().y - m_RenderWindow->getView().getSize().y / 6));
		m_EnemyCubemon->SetSpriteScale(sf::Vector2f(1.5, 1.5));
	}
	if (m_FriendlyCubemon != nullptr)
	{
		m_FriendlyCubemon->SetCurrentHealth(GrabCubemonHealthBasedOnType());
		m_FriendlyCubemon->SetLevel(GrabCubemonLevelBasedOnType());
		m_FriendlyCubemon->SetXP(GrabCubemonEXPBasedOnType());
		m_FriendlyCubemon->SetSpritePos(sf::Vector2f(m_RenderWindow->getView().getCenter().x - m_RenderWindow->getView().getSize().x / 3.5, m_RenderWindow->getView().getCenter().y + m_RenderWindow->getView().getSize().y / 7));
		m_FriendlyCubemon->SetSpriteScale(sf::Vector2f(3, 3));
	}
}

void CBattleScene::CreateAudioManager()
{
	NumptyBehavior::DeletePointer(m_AudioManager);
	m_AudioManager = nullptr;
	m_AudioManager = new AudioManager;
}

void CBattleScene::CreateGUI()
{
	DeletePointer(m_GUI);
	m_GUI = nullptr;
	m_GUI = new GUI(m_RenderWindow, m_TextureMaster, m_Font);
	m_GUI->InitBattleUI();
}

void CBattleScene::CleanupAllPointers()
{
	DeletePointer(m_AudioManager);
	DeletePointer(m_GUI);


	if (!m_bCustom)
	{
		DeletePointer(m_EnemyCubemon);
	}
	else
	{
		for (auto& item : m_EnemyCubemonVector)
		{
			DeletePointer(item);
			item = nullptr;
		}
		m_EnemyCubemonVector.erase(std::remove(m_EnemyCubemonVector.begin(), m_EnemyCubemonVector.end(), nullptr), m_EnemyCubemonVector.end());
	}

	DeletePointer(m_FriendlyCubemon);

	m_EnemyCubemon = nullptr;
	m_FriendlyCubemon = nullptr;
	m_GUI = nullptr;
	m_AudioManager = nullptr;
	m_TextureMaster = nullptr;
	m_Event = nullptr;
	m_RenderWindow = nullptr;
}

void CBattleScene::InitUIView()
{
	m_RenderWindow->setView(m_RenderWindow->getDefaultView());
	m_UIView.setSize((sf::Vector2f)m_RenderWindow->getSize());
	m_UIView.setCenter(m_RenderWindow->getView().getCenter());
}

void CBattleScene::InitWorldView()
{
	m_RenderWindow->setView(m_RenderWindow->getDefaultView());
	m_WorldView.setSize((sf::Vector2f)m_RenderWindow->getSize());
	m_WorldView.setCenter(m_RenderWindow->getView().getCenter());
}

void CBattleScene::InitBackground()
{
	m_BackgroundTex.loadFromFile("Resources/Images/Backgrounds/BattleBG.png");
	m_Background.setSize(sf::Vector2f(1920, 1080));
	m_Background.setFillColor(sf::Color::White);
	m_Background.setOrigin(1920/2, 1080/2);
	m_Background.setPosition(m_RenderWindow->getView().getCenter().x, m_RenderWindow->getView().getCenter().y);
	m_Background.setTexture(&m_BackgroundTex);
	m_Background.setScale(1, 1);
}

void CBattleScene::SaveCubemonValues()
{
	std::ifstream file;
	std::vector<int> m_Types{};
	std::vector<int> m_Lvls{};
	std::vector<int> m_HPs{};
	std::vector<int> m_XPs{};
	char type = 0;
	char lvl = 0;
	file.open("Resources/Output/CubemonData.ini");
	if (file.is_open())
	{
		while (file.get(type))
		{
			if (type == ',') {}
			else
			{
				m_Types.push_back(((int)type) - ASCIIOFFSET);
			}
		}
	}
	file.close();

	std::string currentLine;
	file.open("Resources/Output/CubemonLvlData.ini");
	if (file.is_open())
	{
		while (std::getline(file, currentLine))
		{
			std::size_t pos = currentLine.find('=');
			std::string value = currentLine.substr(pos + 1);

			m_Lvls.push_back(std::stoi(value));
		}

	}
	file.close();
	for (int i = 0; i < m_Types.size(); i++)
	{
		if (m_Types[i] == (int)m_FriendlyCubemon->m_CubeType)
		{
			m_Lvls[i] = (int)m_FriendlyCubemon->GetLvl();
		}
	}

	file.open("Resources/Output/CubemonXPData.ini");
	if (file.is_open())
	{
		while (std::getline(file, currentLine))
		{
			std::size_t pos = currentLine.find('=');
			std::string value = currentLine.substr(pos + 1);

			m_XPs.push_back(std::stoi(value));
		}

	}
	file.close();
	for (int i = 0; i < m_Types.size(); i++)
	{
		if (m_Types[i] == (int)m_FriendlyCubemon->m_CubeType)
		{
			m_XPs[i] = (int) m_FriendlyCubemon->GetXP();
		}
	}

	file.open("Resources/Output/CubemonHPData.ini");
	if (file.is_open())
	{
		while (std::getline(file, currentLine))
		{
			std::size_t pos = currentLine.find('=');
			std::string value = currentLine.substr(pos + 1);

			m_HPs.push_back(std::stoi(value));
		}

	}
	file.close();
	for (int i = 0; i < m_HPs.size(); i++)
	{
		if (m_Types[i] == (int)m_FriendlyCubemon->m_CubeType)
		{
			m_HPs[i] = (int) m_FriendlyCubemon->GetCurrentHealth();
		}
	}

	std::ofstream oFile;

	oFile.open("Resources/Output/CubemonData.ini");
	if (oFile.is_open())
	{
		oFile.clear();
		for (auto& cubemon : m_Types)
		{
			oFile << cubemon << ",";
		}
	}
	oFile.close();

	oFile.open("Resources/Output/CubemonLvlData.ini");
	if (oFile.is_open())
	{
		oFile.clear();
		for (int i = 0; i < m_Types.size(); i++)
		{
			oFile << m_Lvls[i] << std::endl;
		}
	}
	oFile.close();

	oFile.open("Resources/Output/CubemonHPData.ini");
	if (oFile.is_open())
	{
		oFile.clear();
		for (int i = 0; i < m_Types.size(); i++)
		{
			oFile << m_HPs[i] << std::endl;
		}
	}
	oFile.close();

	oFile.open("Resources/Output/CubemonXPData.ini");
	if (oFile.is_open())
	{
		oFile.clear();
		for (int i = 0; i < m_Types.size(); i++)
		{
			oFile << m_XPs[i] << std::endl;
		}
	}
	oFile.close();
}

int CBattleScene::GrabCubemonLevelBasedOnType()
{
	std::ifstream file;
	std::vector<int> m_Types;
	std::vector<int> m_Lvls;
	char type = 0;
	file.open("Resources/Output/CubemonData.ini");
	if (file.is_open())
	{
		while (file.get(type))
		{
			if (type == ',') {}
			else
			{
				m_Types.push_back(((int)type) - ASCIIOFFSET);
			}
		}
		file.close();
	}
	std::string currentLine;
	file.open("Resources/Output/CubemonLvlData.ini");
	if (file.is_open())
	{
		while (std::getline(file, currentLine))
		{
			std::size_t pos = currentLine.find('=');
			std::string value = currentLine.substr(pos + 1);

			m_Lvls.push_back(std::stoi(value));
		}

	}
	file.close();
	for (int i = 0; i < m_Lvls.size(); i++)
	{
		if (m_Types[i] == (int)m_FriendlyCubemon->m_CubeType)
		{
			return m_Lvls[i];
		}
	}
}

int CBattleScene::GrabCubemonHealthBasedOnType()
{
	std::ifstream file;
	std::vector<int> m_Types;
	std::vector<int> m_HPs;
	char type = 0;
	int HP = 0;
	file.open("Resources/Output/CubemonData.ini");
	if (file.is_open())
	{
		while (file.get(type))
		{
			if (type == ',') {}
			else
			{
				m_Types.push_back(((int)type) - ASCIIOFFSET);
			}
		}
		file.close();
	}
	std::string currentLine;
	file.open("Resources/Output/CubemonHPData.ini");
	if (file.is_open())
	{
		while (std::getline(file, currentLine))
		{
			std::size_t pos = currentLine.find('=');
			std::string value = currentLine.substr(pos + 1);

			m_HPs.push_back(std::stoi(value));
		}

	}
	file.close();
	for (int i = 0; i < m_HPs.size(); i++)
	{
		if (m_Types[i] == (int)m_FriendlyCubemon->m_CubeType)
		{
			return m_HPs[i];
		}
	}
}

std::vector<int> CBattleScene::GrabCubemonHealth()
{
	std::ifstream file;
	std::vector<int> m_HPs;
	int HP = 0;
	std::string currentLine;
	file.open("Resources/Output/CubemonHPData.ini");
	if (file.is_open())
	{
		while (std::getline(file, currentLine))
		{
			std::size_t pos = currentLine.find('=');
			std::string value = currentLine.substr(pos + 1);

			m_HPs.push_back(std::stoi(value));
		}

	}
	file.close();
	return m_HPs;
}

std::vector<int> CBattleScene::GrabCubemonTypes()
{
	std::ifstream file;
	std::vector<int> m_Types;
	char type = 0;
	file.open("Resources/Output/CubemonData.ini");
	if (file.is_open())
	{
		while (file.get(type))
		{
			if (type == ',') {}
			else
			{
				m_Types.push_back(((int)type) - ASCIIOFFSET);
			}
		}
		file.close();
	}
	return m_Types;
}

int CBattleScene::GrabCubemonEXPBasedOnType()
{
	std::ifstream file;
	std::vector<int> m_Types;
	std::vector<int> m_XPs;
	char type = 0;
	file.open("Resources/Output/CubemonData.ini");
	if (file.is_open())
	{
		while (file.get(type))
		{
			if (type == ',') {}
			else
			{
				m_Types.push_back(((int)type) - ASCIIOFFSET);
			}
		}
		file.close();
	}
	std::string currentLine;
	file.open("Resources/Output/CubemonXPData.ini");
	if (file.is_open())
	{
		while (std::getline(file, currentLine))
		{
			std::size_t pos = currentLine.find('=');
			std::string value = currentLine.substr(pos + 1);

			m_XPs.push_back(std::stoi(value));
		}

	}
	file.close();
	for (int i = 0; i < m_XPs.size(); i++)
	{
		if (m_Types[i] == (int)m_FriendlyCubemon->m_CubeType)
		{
			return m_XPs[i];
		}
	}
}

bool CBattleScene::IsPlayerDeath()
{
	for (auto& item : GrabCubemonHealth())
	{
		if (item > 0)
		{
			return false;
		}
	}
	return true;
}

void CBattleScene::ResetPlayerPosition()
{
	std::ofstream file;

	file.open("Resources/Output/PlayerData.ini");
	if (file.is_open())
	{
		file.clear();
		file << "x =" << 0 << std::endl << "y =" << 0;
	}
	file.close();
}

void CBattleScene::CreateNewlyChangedPokemon(int _newType)
{
	if (m_GUI->GetCurrentCubemon() == ICubemon::CUBEMONTYPE::THALLIC)
	{
		m_FriendlyCubemon = new CThallic(m_RenderWindow, m_AudioManager);
	}
	else if (m_GUI->GetCurrentCubemon() == ICubemon::CUBEMONTYPE::KINDLING)
	{
		m_FriendlyCubemon = new CKindling(m_RenderWindow, m_AudioManager);
	}
	else if (m_GUI->GetCurrentCubemon() == ICubemon::CUBEMONTYPE::BRUTUS)
	{
		m_FriendlyCubemon = new CBrutus(m_RenderWindow, m_AudioManager);
	}
	else if (m_GUI->GetCurrentCubemon() == ICubemon::CUBEMONTYPE::WIRLSON)
	{
		m_FriendlyCubemon = new CWirlson(m_RenderWindow, m_AudioManager);
	}
	else if (m_GUI->GetCurrentCubemon() == ICubemon::CUBEMONTYPE::DUSTDEVIL)
	{
		m_FriendlyCubemon = new CDustDevil(m_RenderWindow, m_AudioManager);
	}
	else if (m_GUI->GetCurrentCubemon() == ICubemon::CUBEMONTYPE::BLIZZARDBIRD)
	{
		m_FriendlyCubemon = new CBlizzardBird(m_RenderWindow, m_AudioManager);
	}
	if (m_FriendlyCubemon != nullptr)
	{
		m_FriendlyCubemon->SetCurrentHealth(GrabCubemonHealthBasedOnType());
		m_FriendlyCubemon->SetLevel(GrabCubemonLevelBasedOnType());
		m_FriendlyCubemon->SetXP(GrabCubemonEXPBasedOnType());
		m_FriendlyCubemon->SetSpritePos(sf::Vector2f(m_RenderWindow->getView().getCenter().x - m_RenderWindow->getView().getSize().x / 3.5, m_RenderWindow->getView().getCenter().y + m_RenderWindow->getView().getSize().y / 7));
		m_FriendlyCubemon->SetSpriteScale(sf::Vector2f(3, 3));
	}
}

void CBattleScene::InitCustomBattleTypeOne()
{
	switch (m_PreDefEnemyType1)
	{
	case ICubemon::CUBEMONTYPE::THALLIC:
	{
		m_EnemyCubemonVector.push_back(new CThallic(m_RenderWindow, m_AudioManager));
		break;
	}
	case ICubemon::CUBEMONTYPE::KINDLING:
	{
		m_EnemyCubemonVector.push_back(new CKindling(m_RenderWindow, m_AudioManager));
		break;
	}
	case ICubemon::CUBEMONTYPE::BRUTUS:
	{
		m_EnemyCubemonVector.push_back(new CBrutus(m_RenderWindow, m_AudioManager));
		break;
	}
	case ICubemon::CUBEMONTYPE::WIRLSON:
	{
		m_EnemyCubemonVector.push_back(new CWirlson(m_RenderWindow, m_AudioManager));
		break;
	}
	case ICubemon::CUBEMONTYPE::DUSTDEVIL:
	{
		m_EnemyCubemonVector.push_back(new CDustDevil(m_RenderWindow, m_AudioManager));
		break;
	}
	case ICubemon::CUBEMONTYPE::BLIZZARDBIRD:
	{
		m_EnemyCubemonVector.push_back(new CBlizzardBird(m_RenderWindow, m_AudioManager));
		break;
	}
	default:
		break;
	}
	m_EnemyCubemonVector.back()->SetLevel(m_FriendlyCubemon->GetLvl() + 3);
}

void CBattleScene::InitCustomBattleTypeTwo()
{
	switch (m_PreDefEnemyType2)
	{
	case ICubemon::CUBEMONTYPE::THALLIC:
	{
		m_EnemyCubemonVector.push_back(new CThallic(m_RenderWindow, m_AudioManager));
		break;
	}
	case ICubemon::CUBEMONTYPE::KINDLING:
	{
		m_EnemyCubemonVector.push_back(new CKindling(m_RenderWindow, m_AudioManager));
		break;
	}
	case ICubemon::CUBEMONTYPE::BRUTUS:
	{
		m_EnemyCubemonVector.push_back(new CBrutus(m_RenderWindow, m_AudioManager));
		break;
	}
	case ICubemon::CUBEMONTYPE::WIRLSON:
	{
		m_EnemyCubemonVector.push_back(new CWirlson(m_RenderWindow, m_AudioManager));
		break;
	}
	case ICubemon::CUBEMONTYPE::DUSTDEVIL:
	{
		m_EnemyCubemonVector.push_back(new CDustDevil(m_RenderWindow, m_AudioManager));
		break;
	}
	case ICubemon::CUBEMONTYPE::BLIZZARDBIRD:
	{
		m_EnemyCubemonVector.push_back(new CBlizzardBird(m_RenderWindow, m_AudioManager));
		break;
	}
	default:
		break;
	}
	m_EnemyCubemonVector.back()->SetLevel(m_FriendlyCubemon->GetLvl() + 2);
}

void CBattleScene::InitCustomBattleTypeThree()
{
	switch (m_PreDefEnemyType3)
	{
	case ICubemon::CUBEMONTYPE::THALLIC:
	{
		m_EnemyCubemonVector.push_back(new CThallic(m_RenderWindow, m_AudioManager));
		break;
	}
	case ICubemon::CUBEMONTYPE::KINDLING:
	{
		m_EnemyCubemonVector.push_back(new CKindling(m_RenderWindow, m_AudioManager));
		break;
	}
	case ICubemon::CUBEMONTYPE::BRUTUS:
	{
		m_EnemyCubemonVector.push_back(new CBrutus(m_RenderWindow, m_AudioManager));
		break;
	}
	case ICubemon::CUBEMONTYPE::WIRLSON:
	{
		m_EnemyCubemonVector.push_back(new CWirlson(m_RenderWindow, m_AudioManager));
		break;
	}
	case ICubemon::CUBEMONTYPE::DUSTDEVIL:
	{
		m_EnemyCubemonVector.push_back(new CDustDevil(m_RenderWindow, m_AudioManager));
		break;
	}
	case ICubemon::CUBEMONTYPE::BLIZZARDBIRD:
	{
		m_EnemyCubemonVector.push_back(new CBlizzardBird(m_RenderWindow, m_AudioManager));
		break;
	}
	default:
		break;
	}
	m_EnemyCubemonVector.back()->SetLevel(m_FriendlyCubemon->GetLvl() + 1);
}

void CBattleScene::ChangePokemon(int _newType)
{
	m_GUI->HandleINISwaps(_newType);
	DeletePointer(m_FriendlyCubemon);
	m_FriendlyCubemon = nullptr;
	CreateNewlyChangedPokemon(_newType);
	DeletePointer(m_GUI);
	m_GUI = nullptr;
	CreateGUI();
	InitPlayerCubemonFromINI();
	m_GUI->InitCubemonHUD(m_FriendlyCubemon, m_EnemyCubemon);
}

void CBattleScene::InitPlayerCubemonFromINI()
{
	if (m_GUI->GetCurrentCubemon() == ICubemon::CUBEMONTYPE::THALLIC)
	{
		m_FriendlyCubemon = new CThallic(m_RenderWindow, m_AudioManager);
	}
	else if (m_GUI->GetCurrentCubemon() == ICubemon::CUBEMONTYPE::KINDLING)
	{
		m_FriendlyCubemon = new CKindling(m_RenderWindow, m_AudioManager);
	}
	else if (m_GUI->GetCurrentCubemon() == ICubemon::CUBEMONTYPE::BRUTUS)
	{
		m_FriendlyCubemon = new CBrutus(m_RenderWindow, m_AudioManager);
	}
	else if (m_GUI->GetCurrentCubemon() == ICubemon::CUBEMONTYPE::WIRLSON)
	{
		m_FriendlyCubemon = new CWirlson(m_RenderWindow, m_AudioManager);
	}
	else if (m_GUI->GetCurrentCubemon() == ICubemon::CUBEMONTYPE::DUSTDEVIL)
	{
		m_FriendlyCubemon = new CDustDevil(m_RenderWindow, m_AudioManager);
	}
	else if (m_GUI->GetCurrentCubemon() == ICubemon::CUBEMONTYPE::BLIZZARDBIRD)
	{
		m_FriendlyCubemon = new CBlizzardBird(m_RenderWindow, m_AudioManager);
	}
	if (m_FriendlyCubemon != nullptr)
	{
		m_FriendlyCubemon->SetCurrentHealth(GrabCubemonHealthBasedOnType());
		m_FriendlyCubemon->SetLevel(GrabCubemonLevelBasedOnType());
		m_FriendlyCubemon->SetXP(GrabCubemonEXPBasedOnType());
		m_FriendlyCubemon->SetSpritePos(sf::Vector2f(m_RenderWindow->getView().getCenter().x - m_RenderWindow->getView().getSize().x / 3.5, m_RenderWindow->getView().getCenter().y + m_RenderWindow->getView().getSize().y / 7));
		m_FriendlyCubemon->SetSpriteScale(sf::Vector2f(3, 3));
	}
}

int CBattleScene::GrabCubemonHealthBasedOnType(int _type)
{
	std::ifstream file;
	std::vector<int> m_Types;
	std::vector<int> m_HPs;
	char type = 0;
	int HP = 0;
	file.open("Resources/Output/CubemonData.ini");
	if (file.is_open())
	{
		while (file.get(type))
		{
			if (type == ',') {}
			else
			{
				m_Types.push_back(((int)type) - ASCIIOFFSET);
			}
		}
		file.close();
	}
	std::string currentLine;
	file.open("Resources/Output/CubemonHPData.ini");
	if (file.is_open())
	{
		while (std::getline(file, currentLine))
		{
			std::size_t pos = currentLine.find('=');
			std::string value = currentLine.substr(pos + 1);

			m_HPs.push_back(std::stoi(value));
		}

	}
	file.close();
	for (int i = 0; i < m_HPs.size(); i++)
	{
		if (m_Types[i] == _type)
		{
			return m_HPs[i];
		}
	}
}

