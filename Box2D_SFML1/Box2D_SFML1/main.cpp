// Local Includes
#include "CSceneManager.h"
#include "CDebugWindow.h"

// Forward Declaration
void Start();
void Update();
void Render();

void CreateRenderWindow(sf::Uint32 _style);
void CleanupAllPointers();

// Main Render Window
static sf::RenderWindow* m_RenderWindow = nullptr;

static TextureMaster* m_TextureMaster = nullptr;

static sf::Font m_Font;

// Main Event
static sf::Event m_Event;
CSceneManager* m_SceneManager = nullptr;
CDebugWindow* m_DebugWindow = nullptr;

sf::Clock m_DebugWindowOpenTimer;

int main()
{
	CreateRenderWindow(sf::Style::Default);

	Start();
	Update();

	// Cleanup
	CleanupAllPointers();

	// Return NULL
	return NULL;
}

void Start()
{
	m_Font.loadFromFile("Resources/Fonts/ANDYB.TTF");
	m_TextureMaster = new TextureMaster;
	m_SceneManager = new CSceneManager(m_RenderWindow, m_Event, m_TextureMaster);
	m_SceneManager->Start();
}

void Update()
{
	while (m_RenderWindow->isOpen())
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && m_DebugWindowOpenTimer.getElapsedTime().asSeconds() >= 0.25f)
		{
			if (m_DebugWindow != nullptr)
			{
				NumptyBehavior::DeletePointer(m_DebugWindow);
				m_DebugWindow = nullptr;
			}
			else if (m_DebugWindow == nullptr)
			{
				m_DebugWindow = new CDebugWindow(&m_Event);
				m_DebugWindow->Start();
			}
			m_DebugWindowOpenTimer.restart();
		}
		
		m_SceneManager->Update();

		m_SceneManager->PolledUpdate();

		m_SceneManager->CheckForMARKASDESTROY();

		if (m_DebugWindow)
		{
			m_DebugWindow->Update();
		}

		// Render
		Render();
	}
}

void Render()
{
	//
	m_RenderWindow->clear();

	m_SceneManager->Render();

	m_RenderWindow->display();
	//


	//
	if (m_DebugWindow)
	{
		m_DebugWindow->Render();
	}
	//
}

void CreateRenderWindow(sf::Uint32 _style)
{
	if (NumptyBehavior::IsNullptr(m_RenderWindow))
	{
		m_RenderWindow = new sf::RenderWindow();
	}

	// Render Window Settings
	sf::ContextSettings m_Settings;
	m_Settings.antialiasingLevel = 8;

	m_RenderWindow->create(sf::VideoMode(1920, 1080), "Cube Land", _style, m_Settings);
	m_RenderWindow->setFramerateLimit(60);
	m_RenderWindow->setVerticalSyncEnabled(true);
	m_RenderWindow->setKeyRepeatEnabled(false);
	m_RenderWindow->setMouseCursorVisible(true);
}

void CleanupAllPointers()
{
	NumptyBehavior::DeletePointer(m_DebugWindow);
	m_DebugWindow = nullptr;
	NumptyBehavior::DeletePointer(m_SceneManager);
	m_SceneManager = nullptr;
	NumptyBehavior::DeletePointer(m_TextureMaster);
	m_TextureMaster = nullptr;
	NumptyBehavior::DeletePointer(m_RenderWindow);
	m_RenderWindow = nullptr;
}