#include "CDebugWindow.h"

CDebugWindow::CDebugWindow(sf::Event* _event, Player* _player = nullptr)
{
	m_Event = _event;
	m_Player = _player;
	Start();
}

CDebugWindow::~CDebugWindow()
{
	for (auto& item : m_MiscButtons)
	{
		DeletePointer(item);
		item = nullptr;
	}
	m_MiscButtons.clear();

	for (auto& item : m_InGameButtons)
	{
		DeletePointer(item);
		item = nullptr;
	}
	m_InGameButtons.clear();

	for (auto& item : m_BattleButtons)
	{
		DeletePointer(item);
		item = nullptr;
	}
	m_BattleButtons.clear();

	m_InGameButtons.resize(0);	// SUS
	m_BattleButtons.resize(0);	// SUS
	m_MiscButtons.resize(0);	// SUS

	m_Shapes.clear();

	DeletePointer(m_Player);
	m_Player = nullptr;
	m_Event = nullptr;
	DeletePointer(m_RenderWindow);
	m_RenderWindow = nullptr;
}

void CDebugWindow::Start()
{
	InitWindow(sf::Style::Titlebar);
}

void CDebugWindow::Update()
{
	if (IsSceneGame())
	{
		for (auto& item : m_MiscButtons)
		{
			item->Update();
		}
		for (auto& item : m_InGameButtons)
		{
			item->Update();
		}
	}
	else
	{
		if (IsSceneBattle)
		{
			for (auto& item : m_BattleButtons)
			{
				item->Update();
			}
		}
		for (auto& item : m_MiscButtons)
		{
			item->Update();
		}
	}
}

void CDebugWindow::PolledUpdate()
{
	while (m_RenderWindow->pollEvent(*m_Event))
	{
		if (m_Event->type == sf::Event::MouseButtonPressed)
		{
			if (IsSceneGame())
			{
				HandleMiscButtons();
				HandleGameButtons();
			}
			else
			{
				if (IsSceneBattle)
				{
					HandleBattleButtons();
				}
				HandleMiscButtons();
			}
		}
	}
}

void CDebugWindow::Render()
{
	if (IsSceneGame())
	{
		for (auto& item : m_MiscButtons)
		{
			item->Render();
		}
		for (auto& item : m_InGameButtons)
		{
			item->Render();
		}
	}
	else
	{
		if (IsSceneBattle)
		{
			for (auto& item : m_BattleButtons)
			{
				item->Render();
			}
		}
		for (auto& item : m_MiscButtons)
		{
			item->Render();
		}
	}
}

void CDebugWindow::InitBackGround()
{
}

void CDebugWindow::InitWindow(sf::Uint32 _style)
{
	if (NumptyBehavior::IsNullptr(m_RenderWindow))
	{
		m_RenderWindow = new sf::RenderWindow();
	}

	// Render Window Settings
	sf::ContextSettings m_Settings;
	m_Settings.antialiasingLevel = 8;

	m_RenderWindow->create(sf::VideoMode(100, 200), "DebugConsole", _style, m_Settings);
	m_RenderWindow->setFramerateLimit(60);
	m_RenderWindow->setVerticalSyncEnabled(true);
	m_RenderWindow->setKeyRepeatEnabled(false);
	m_RenderWindow->setMouseCursorVisible(true);
}

bool CDebugWindow::IsSceneGame()
{
	if (ReturnSceneChange() == 1 || ReturnSceneChange() == 2)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool CDebugWindow::IsSceneBattle()
{
	if (ReturnSceneChange() == -1)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void CDebugWindow::HandleGameButtons()
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		if (m_InGameButtons[0]->m_bIsHovering == true)
		{
			HealAllPokemon();
		}
		else if(m_InGameButtons[1]->m_bIsHovering == true)
		{
			AddRandomPokemonToParty();
		}
		else if (m_InGameButtons[2]->m_bIsHovering == true)
		{
			LevelUpFirstCubemon();
		}
		else if (m_InGameButtons[3]->m_bIsHovering == true)
		{
			if (m_Player != nullptr)
			{
				m_Player->ToggleDebugSprint();
			}
		}
	}
}

void CDebugWindow::HandleMiscButtons()
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		if (m_MiscButtons[0]->m_bIsHovering == true)
		{
			AddRandomPokemonToParty();
		}
		else if (m_MiscButtons[1]->m_bIsHovering == true)
		{
			AddThallicToParty();
		}
		else if (m_MiscButtons[2]->m_bIsHovering == true)
		{
			AddKindlingToParty();
		}
		else if (m_MiscButtons[3]->m_bIsHovering == true)
		{
			AddBrutusToParty();
		}
		else if (m_MiscButtons[4]->m_bIsHovering == true)
		{
			ForceBattleScene();
		}
		else if (m_MiscButtons[5]->m_bIsHovering == true)
		{
			ForceMedicalScene();
		}
		else if (m_MiscButtons[6]->m_bIsHovering == true)
		{
			ForceMainMenu();
		}
		else if (m_MiscButtons[7]->m_bIsHovering == true)
		{
			ForceGameScene();
		}
	}
}

void CDebugWindow::HandleBattleButtons()
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
	}
}

void CDebugWindow::HealAllPokemon()
{
	Player::HealAllPokemon();
}

void CDebugWindow::AddRandomPokemonToParty()
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
	m_Types.push_back((rand() % 3) + 1);

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
	m_Lvls.push_back(1);

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
	m_XPs.push_back(0);

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
	m_HPs.push_back(100);

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
		for (int i = 0; i < m_Lvls.size(); i++)
		{
			oFile << m_Lvls[i] << std::endl;
		}
	}
	oFile.close();

	oFile.open("Resources/Output/CubemonHPData.ini");
	if (oFile.is_open())
	{
		oFile.clear();
		for (int i = 0; i < m_HPs.size(); i++)
		{
			oFile << m_HPs[i] << std::endl;
		}
	}
	oFile.close();

	oFile.open("Resources/Output/CubemonXPData.ini");
	if (oFile.is_open())
	{
		oFile.clear();
		for (int i = 0; i < m_XPs.size(); i++)
		{
			oFile << m_XPs[i] << std::endl;
		}
	}
	oFile.close();
}

void CDebugWindow::AddThallicToParty()
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
	m_Types.push_back(1);

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
	m_Lvls.push_back(1);

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
	m_XPs.push_back(0);

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
	m_HPs.push_back(100);

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
		for (int i = 0; i < m_Lvls.size(); i++)
		{
			oFile << m_Lvls[i] << std::endl;
		}
	}
	oFile.close();

	oFile.open("Resources/Output/CubemonHPData.ini");
	if (oFile.is_open())
	{
		oFile.clear();
		for (int i = 0; i < m_HPs.size(); i++)
		{
			oFile << m_HPs[i] << std::endl;
		}
	}
	oFile.close();

	oFile.open("Resources/Output/CubemonXPData.ini");
	if (oFile.is_open())
	{
		oFile.clear();
		for (int i = 0; i < m_XPs.size(); i++)
		{
			oFile << m_XPs[i] << std::endl;
		}
	}
	oFile.close();
}

void CDebugWindow::AddKindlingToParty()
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
	m_Types.push_back(2);

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
	m_Lvls.push_back(1);

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
	m_XPs.push_back(0);

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
	m_HPs.push_back(100);

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
		for (int i = 0; i < m_Lvls.size(); i++)
		{
			oFile << m_Lvls[i] << std::endl;
		}
	}
	oFile.close();

	oFile.open("Resources/Output/CubemonHPData.ini");
	if (oFile.is_open())
	{
		oFile.clear();
		for (int i = 0; i < m_HPs.size(); i++)
		{
			oFile << m_HPs[i] << std::endl;
		}
	}
	oFile.close();

	oFile.open("Resources/Output/CubemonXPData.ini");
	if (oFile.is_open())
	{
		oFile.clear();
		for (int i = 0; i < m_XPs.size(); i++)
		{
			oFile << m_XPs[i] << std::endl;
		}
	}
	oFile.close();
}

void CDebugWindow::AddBrutusToParty()
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
	m_Types.push_back(3);

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
	m_Lvls.push_back(1);

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
	m_XPs.push_back(0);

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
	m_HPs.push_back(100);

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
		for (int i = 0; i < m_Lvls.size(); i++)
		{
			oFile << m_Lvls[i] << std::endl;
		}
	}
	oFile.close();

	oFile.open("Resources/Output/CubemonHPData.ini");
	if (oFile.is_open())
	{
		oFile.clear();
		for (int i = 0; i < m_HPs.size(); i++)
		{
			oFile << m_HPs[i] << std::endl;
		}
	}
	oFile.close();

	oFile.open("Resources/Output/CubemonXPData.ini");
	if (oFile.is_open())
	{
		oFile.clear();
		for (int i = 0; i < m_XPs.size(); i++)
		{
			oFile << m_XPs[i] << std::endl;
		}
	}
	oFile.close();
}

void CDebugWindow::LevelUpFirstCubemon()
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

	bool firstLine = true;
	std::string currentLine;
	file.open("Resources/Output/CubemonLvlData.ini");
	if (file.is_open())
	{
		while (std::getline(file, currentLine))
		{
			std::size_t pos = currentLine.find('=');
			std::string value = currentLine.substr(pos + 1);

			if (firstLine)
			{
				m_Lvls.push_back(std::stoi(value) + 1);
			}
			else
			{
				m_Lvls.push_back(std::stoi(value));
			}
			
			firstLine = false;
		}

	}
	file.close();

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
		for (int i = 0; i < m_Lvls.size(); i++)
		{
			oFile << m_Lvls[i] << std::endl;
		}
	}
	oFile.close();

	oFile.open("Resources/Output/CubemonHPData.ini");
	if (oFile.is_open())
	{
		oFile.clear();
		for (int i = 0; i < m_HPs.size(); i++)
		{
			oFile << m_HPs[i] << std::endl;
		}
	}
	oFile.close();

	oFile.open("Resources/Output/CubemonXPData.ini");
	if (oFile.is_open())
	{
		oFile.clear();
		for (int i = 0; i < m_XPs.size(); i++)
		{
			oFile << m_XPs[i] << std::endl;
		}
	}
	oFile.close();
}

void CDebugWindow::ForceBattleScene()
{
	InterceptSceneChange(-1);
}

void CDebugWindow::ForceMedicalScene()
{
	InterceptSceneChange(2);
}

void CDebugWindow::ForceMainMenu()
{
	InterceptSceneChange(0);
}

void CDebugWindow::ForceGameScene()
{
	InterceptSceneChange(1);
}
