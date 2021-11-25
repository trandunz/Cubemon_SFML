#include "CDebugWindow.h"

CDebugWindow::CDebugWindow(sf::Event* _event, Player* _player)
{
	m_Event = new sf::Event;
	m_Player = _player;
}

CDebugWindow::~CDebugWindow()
{
	for (auto& item : m_MiscButtons)
	{
		DeletePointer(item);
		item = nullptr;
	}
	m_MiscButtons.erase(std::remove(m_MiscButtons.begin(), m_MiscButtons.end(), nullptr), m_MiscButtons.end());

	for (auto& item : m_InGameButtons)
	{
		DeletePointer(item);
		item = nullptr;
	}
	m_InGameButtons.erase(std::remove(m_InGameButtons.begin(), m_InGameButtons.end(), nullptr), m_InGameButtons.end());

	for (auto& item : m_BattleButtons)
	{
		DeletePointer(item);
		item = nullptr;
	}
	m_BattleButtons.erase(std::remove(m_BattleButtons.begin(), m_BattleButtons.end(), nullptr), m_BattleButtons.end());

	m_Shapes.clear();

	DeletePointer(m_Player);
	m_Player = nullptr;
	m_Event = nullptr;
	DeletePointer(m_Event);
	m_Event = nullptr;
	DeletePointer(m_RenderWindow);
	m_RenderWindow = nullptr;
}

void CDebugWindow::Start()
{
	InitWindow(sf::Style::Titlebar);
	CreateButtons();
}

void CDebugWindow::Update()
{
	PolledUpdate();
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
		if (IsSceneBattle())
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
		if (m_Event->type == sf::Event::MouseButtonPressed && m_Event->key.code == sf::Mouse::Left)
		{
			if (IsSceneGame())
			{
				HandleMiscButtons();
				HandleGameButtons();
			}
			else
			{
				HandleBattleButtons();
				HandleMiscButtons();
			}
		}
	}
}

void CDebugWindow::Render()
{
	m_RenderWindow->clear();
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
		if (IsSceneBattle())
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
	m_RenderWindow->display();
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

	m_RenderWindow->create(sf::VideoMode(300, 600), "DebugConsole", _style, m_Settings);
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
		else if (m_InGameButtons[1]->m_bIsHovering == true)
		{
			LevelUpFirstCubemon();
		}
	}
}

void CDebugWindow::HandleMiscButtons()
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		if (m_MiscButtons[0]->m_bIsHovering == true)
		{
			ForceBattleScene();
		}
		else if (m_MiscButtons[1]->m_bIsHovering == true)
		{
			ForceMedicalScene();
		}
		else if (m_MiscButtons[2]->m_bIsHovering == true)
		{
			ForceMainMenu();
		}
		else if (m_MiscButtons[3]->m_bIsHovering == true)
		{
			ForceGameScene();
		}
		else if (m_MiscButtons[4]->m_bIsHovering == true)
		{
			ForceEndGameBattle();
		}
	}
}

void CDebugWindow::HandleBattleButtons(GUI* _gui)
{
	if (IsSceneBattle() && _gui != nullptr)
	{
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			if (m_MiscButtons[0]->m_bIsHovering == true)
			{
				// Level Up Friendly
				_gui->GetFriendlyCubemon()->LevelUp();
			}
			else if (m_MiscButtons[1]->m_bIsHovering == true)
			{
				// One shot Enemy
				_gui->GetEnemyCubemon()->TakeDamage(_gui->GetEnemyCubemon()->GetCurrentHealth());
			}
			else if (m_MiscButtons[2]->m_bIsHovering == true)
			{
				// Level Up Enemy
				_gui->GetEnemyCubemon()->LevelUp();
			}
			else if (m_MiscButtons[3]->m_bIsHovering == true)
			{
				// Force Heal Friendly
				_gui->GetFriendlyCubemon()->Heal(100);
			}
			else if (m_MiscButtons[4]->m_bIsHovering == true)
			{
				// Force Heal Enemy
				_gui->GetEnemyCubemon()->Heal(100);
			}
		}
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

void CDebugWindow::ForceEndGameBattle()
{
	InterceptSceneChange(-2);
}

void CDebugWindow::CreateIngameButtons()
{
	InitButtonTextures();

	m_InGameButtons.push_back(new CButtons(m_RenderWindow));
	m_InGameButtons.back()->SetFontSize(256);
	m_InGameButtons.back()->SetLabel("");
	m_InGameButtons.back()->SetHoverTex(&m_HealHoverTexture);
	m_InGameButtons.back()->SetIdleTex(&m_HealTexture);
	m_InGameButtons.back()->SetClickTex(&m_HealHoverTexture);
	m_InGameButtons.back()->SetPosition(m_RenderWindow->getView().getCenter().x, m_RenderWindow->getView().getCenter().y + m_InGameButtons.back()->Sprite.getGlobalBounds().width * -2);
	m_InGameButtons.back()->SetSpritePos(m_RenderWindow->getView().getCenter().x - 80, m_RenderWindow->getView().getCenter().y + 30 + m_InGameButtons.back()->Sprite.getGlobalBounds().width * -2);
	m_InGameButtons.back()->m_tLabel.setOutlineThickness(3);

	m_InGameButtons.push_back(new CButtons(m_RenderWindow));
	m_InGameButtons.back()->SetFontSize(256);
	m_InGameButtons.back()->SetLabel("");
	m_InGameButtons.back()->SetHoverTex(&m_UpgradeHoverTexture);
	m_InGameButtons.back()->SetIdleTex(&m_UpgradeTexture);
	m_InGameButtons.back()->SetClickTex(&m_UpgradeHoverTexture);
	m_InGameButtons.back()->SetPosition(m_RenderWindow->getView().getCenter().x, m_RenderWindow->getView().getCenter().y + m_InGameButtons.back()->Sprite.getGlobalBounds().width * 1);
	m_InGameButtons.back()->SetSpritePos(m_RenderWindow->getView().getCenter().x - 80, m_RenderWindow->getView().getCenter().y + 30 + m_InGameButtons.back()->Sprite.getGlobalBounds().width * 1);
	m_InGameButtons.back()->m_tLabel.setOutlineThickness(3);
}

void CDebugWindow::CreateButtons()
{
	CreateIngameButtons();
	CreateMiscButttons();
	CreateBattleButtons();
}

void CDebugWindow::CreateMiscButttons()
{
	// Battle
	m_MiscButtons.push_back(new CButtons(m_RenderWindow));
	m_MiscButtons.back()->SetFontSize(256);
	m_MiscButtons.back()->SetLabel("");
	m_MiscButtons.back()->SetHoverTex(&m_BattleHoverTexture);
	m_MiscButtons.back()->SetIdleTex(&m_BattleTexture);
	m_MiscButtons.back()->SetClickTex(&m_BattleHoverTexture);
	m_MiscButtons.back()->SetPosition(m_RenderWindow->getView().getCenter().x, m_RenderWindow->getView().getCenter().y + m_MiscButtons.back()->Sprite.getGlobalBounds().width * -6);
	m_MiscButtons.back()->SetSpritePos(m_RenderWindow->getView().getCenter().x, m_RenderWindow->getView().getCenter().y + 30 + m_MiscButtons.back()->Sprite.getGlobalBounds().width * -6);
	m_MiscButtons.back()->m_tLabel.setOutlineThickness(3);

	// Medical
	m_MiscButtons.push_back(new CButtons(m_RenderWindow));
	m_MiscButtons.back()->SetFontSize(256);
	m_MiscButtons.back()->SetLabel("");
	m_MiscButtons.back()->SetHoverTex(&m_HealthHoverTexture);
	m_MiscButtons.back()->SetIdleTex(&m_HealthTexture);
	m_MiscButtons.back()->SetClickTex(&m_HealthHoverTexture);
	m_MiscButtons.back()->SetPosition(m_RenderWindow->getView().getCenter().x, m_RenderWindow->getView().getCenter().y + m_MiscButtons.back()->Sprite.getGlobalBounds().width * -4);
	m_MiscButtons.back()->SetSpritePos(m_RenderWindow->getView().getCenter().x, m_RenderWindow->getView().getCenter().y + 30 + m_MiscButtons.back()->Sprite.getGlobalBounds().width * -4);
	m_MiscButtons.back()->m_tLabel.setOutlineThickness(3);

	// Main
	m_MiscButtons.push_back(new CButtons(m_RenderWindow));
	m_MiscButtons.back()->SetFontSize(256);
	m_MiscButtons.back()->SetLabel("");
	m_MiscButtons.back()->SetHoverTex(&m_MainMenuHoverTexture);
	m_MiscButtons.back()->SetIdleTex(&m_MainMenuTexture);
	m_MiscButtons.back()->SetClickTex(&m_MainMenuHoverTexture);
	m_MiscButtons.back()->SetPosition(m_RenderWindow->getView().getCenter().x, m_RenderWindow->getView().getCenter().y + m_MiscButtons.back()->Sprite.getGlobalBounds().width * 4);
	m_MiscButtons.back()->SetSpritePos(m_RenderWindow->getView().getCenter().x, m_RenderWindow->getView().getCenter().y + 30 + m_MiscButtons.back()->Sprite.getGlobalBounds().width * 4);
	m_MiscButtons.back()->m_tLabel.setOutlineThickness(3);

	// Game
	m_MiscButtons.push_back(new CButtons(m_RenderWindow));
	m_MiscButtons.back()->SetFontSize(256);
	m_MiscButtons.back()->SetLabel("");
	m_MiscButtons.back()->SetHoverTex(&m_GameHoverTexture);
	m_MiscButtons.back()->SetIdleTex(&m_GameTexture);
	m_MiscButtons.back()->SetClickTex(&m_GameHoverTexture);
	m_MiscButtons.back()->SetPosition(m_RenderWindow->getView().getCenter().x, m_RenderWindow->getView().getCenter().y + m_MiscButtons.back()->Sprite.getGlobalBounds().width * 6);
	m_MiscButtons.back()->SetSpritePos(m_RenderWindow->getView().getCenter().x, m_RenderWindow->getView().getCenter().y + 30 + m_MiscButtons.back()->Sprite.getGlobalBounds().width * 6);
	m_MiscButtons.back()->m_tLabel.setOutlineThickness(3);

	// Game
	m_MiscButtons.push_back(new CButtons(m_RenderWindow));
	m_MiscButtons.back()->SetFontSize(256);
	m_MiscButtons.back()->SetLabel("");
	m_MiscButtons.back()->SetHoverTex(&m_EndGameHoverTexture);
	m_MiscButtons.back()->SetIdleTex(&m_EndGameTexture);
	m_MiscButtons.back()->SetClickTex(&m_EndGameHoverTexture);
	m_MiscButtons.back()->SetPosition(m_RenderWindow->getView().getCenter().x, m_RenderWindow->getView().getCenter().y + m_MiscButtons.back()->Sprite.getGlobalBounds().width * 0);
	m_MiscButtons.back()->SetSpritePos(m_RenderWindow->getView().getCenter().x, m_RenderWindow->getView().getCenter().y + 30 + m_MiscButtons.back()->Sprite.getGlobalBounds().width * 0);
	m_MiscButtons.back()->m_tLabel.setOutlineThickness(3);
}

void CDebugWindow::CreateBattleButtons()
{
	// Battle
	m_MiscButtons.push_back(new CButtons(m_RenderWindow));
	m_MiscButtons.back()->SetFontSize(256);
	m_MiscButtons.back()->SetLabel("");
	m_MiscButtons.back()->SetHoverTex(&m_BattleHoverTexture);
	m_MiscButtons.back()->SetIdleTex(&m_BattleTexture);
	m_MiscButtons.back()->SetClickTex(&m_BattleHoverTexture);
	m_MiscButtons.back()->SetPosition(m_RenderWindow->getView().getCenter().x, m_RenderWindow->getView().getCenter().y + m_MiscButtons.back()->Sprite.getGlobalBounds().width * -6);
	m_MiscButtons.back()->SetSpritePos(m_RenderWindow->getView().getCenter().x, m_RenderWindow->getView().getCenter().y + 30 + m_MiscButtons.back()->Sprite.getGlobalBounds().width * -6);
	m_MiscButtons.back()->m_tLabel.setOutlineThickness(3);

	// Medical
	m_MiscButtons.push_back(new CButtons(m_RenderWindow));
	m_MiscButtons.back()->SetFontSize(256);
	m_MiscButtons.back()->SetLabel("");
	m_MiscButtons.back()->SetHoverTex(&m_HealthHoverTexture);
	m_MiscButtons.back()->SetIdleTex(&m_HealthTexture);
	m_MiscButtons.back()->SetClickTex(&m_HealthHoverTexture);
	m_MiscButtons.back()->SetPosition(m_RenderWindow->getView().getCenter().x, m_RenderWindow->getView().getCenter().y + m_MiscButtons.back()->Sprite.getGlobalBounds().width * -4);
	m_MiscButtons.back()->SetSpritePos(m_RenderWindow->getView().getCenter().x, m_RenderWindow->getView().getCenter().y + 30 + m_MiscButtons.back()->Sprite.getGlobalBounds().width * -4);
	m_MiscButtons.back()->m_tLabel.setOutlineThickness(3);

	// Main
	m_MiscButtons.push_back(new CButtons(m_RenderWindow));
	m_MiscButtons.back()->SetFontSize(256);
	m_MiscButtons.back()->SetLabel("");
	m_MiscButtons.back()->SetHoverTex(&m_MainMenuHoverTexture);
	m_MiscButtons.back()->SetIdleTex(&m_MainMenuTexture);
	m_MiscButtons.back()->SetClickTex(&m_MainMenuHoverTexture);
	m_MiscButtons.back()->SetPosition(m_RenderWindow->getView().getCenter().x, m_RenderWindow->getView().getCenter().y + m_MiscButtons.back()->Sprite.getGlobalBounds().width * 4);
	m_MiscButtons.back()->SetSpritePos(m_RenderWindow->getView().getCenter().x, m_RenderWindow->getView().getCenter().y + 30 + m_MiscButtons.back()->Sprite.getGlobalBounds().width * 4);
	m_MiscButtons.back()->m_tLabel.setOutlineThickness(3);

	// Game
	m_MiscButtons.push_back(new CButtons(m_RenderWindow));
	m_MiscButtons.back()->SetFontSize(256);
	m_MiscButtons.back()->SetLabel("");
	m_MiscButtons.back()->SetHoverTex(&m_GameHoverTexture);
	m_MiscButtons.back()->SetIdleTex(&m_GameTexture);
	m_MiscButtons.back()->SetClickTex(&m_GameHoverTexture);
	m_MiscButtons.back()->SetPosition(m_RenderWindow->getView().getCenter().x, m_RenderWindow->getView().getCenter().y + m_MiscButtons.back()->Sprite.getGlobalBounds().width * 6);
	m_MiscButtons.back()->SetSpritePos(m_RenderWindow->getView().getCenter().x, m_RenderWindow->getView().getCenter().y + 30 + m_MiscButtons.back()->Sprite.getGlobalBounds().width * 6);
	m_MiscButtons.back()->m_tLabel.setOutlineThickness(3);

	// Game
	m_MiscButtons.push_back(new CButtons(m_RenderWindow));
	m_MiscButtons.back()->SetFontSize(256);
	m_MiscButtons.back()->SetLabel("");
	m_MiscButtons.back()->SetHoverTex(&m_EndGameHoverTexture);
	m_MiscButtons.back()->SetIdleTex(&m_EndGameTexture);
	m_MiscButtons.back()->SetClickTex(&m_EndGameHoverTexture);
	m_MiscButtons.back()->SetPosition(m_RenderWindow->getView().getCenter().x, m_RenderWindow->getView().getCenter().y + m_MiscButtons.back()->Sprite.getGlobalBounds().width * 0);
	m_MiscButtons.back()->SetSpritePos(m_RenderWindow->getView().getCenter().x, m_RenderWindow->getView().getCenter().y + 30 + m_MiscButtons.back()->Sprite.getGlobalBounds().width * 0);
	m_MiscButtons.back()->m_tLabel.setOutlineThickness(3);
}

void CDebugWindow::InitButtonTextures()
{
	m_BattleTexture.loadFromFile("Resources/Images/Debug/Battle.png");
	m_BattleHoverTexture.loadFromFile("Resources/Images/Debug/Battle_Hover.png");
	m_GameTexture.loadFromFile("Resources/Images/Debug/Game.png");
	m_GameHoverTexture.loadFromFile("Resources/Images/Debug/Game_Hover.png");
	m_HealTexture.loadFromFile("Resources/Images/Debug/Heal.png");
	m_HealHoverTexture.loadFromFile("Resources/Images/Debug/Heal_Hover.png");
	m_HealthTexture.loadFromFile("Resources/Images/Debug/Health.png");
	m_HealthHoverTexture.loadFromFile("Resources/Images/Debug/Health_Hover.png");
	m_MainMenuTexture.loadFromFile("Resources/Images/Debug/MainMenu.png");
	m_MainMenuHoverTexture.loadFromFile("Resources/Images/Debug/MainMenu_Hover.png");
	m_UpgradeTexture.loadFromFile("Resources/Images/Debug/Upgrade.png");
	m_UpgradeHoverTexture.loadFromFile("Resources/Images/Debug/Upgrade_Hover.png");
	m_EndGameTexture.loadFromFile("Resources/Images/Debug/EndGame.png");
	m_EndGameHoverTexture.loadFromFile("Resources/Images/Debug/EndGame_Hover.png");
}
