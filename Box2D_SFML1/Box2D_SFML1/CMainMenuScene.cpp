#include "CMainMenuScene.h"

CMainMenuScene::CMainMenuScene(sf::RenderWindow* _renderWindow, sf::Event& _event)
{
	m_RenderWindow = _renderWindow;
	m_Event = &_event;
}

CMainMenuScene::~CMainMenuScene()
{
	DestoryMainMenu();
	CleanupAllPointers();
}

void CMainMenuScene::Start()
{
	InterceptSceneChange(0);
	Player::ResetPlayerData();
	CreateMainMenu();
}

void CMainMenuScene::Update()
{
	m_MainMenu->Update();
}

void CMainMenuScene::PolledUpdate()
{
	m_MainMenu->PolledUpdate(*m_Event);
}

void CMainMenuScene::Render()
{
	m_RenderWindow->draw(*m_MainMenu);
}

void CMainMenuScene::CleanupAllPointers()
{
	NumptyBehavior::DeletePointer(m_MainMenu);
	m_MainMenu = nullptr;
	m_RenderWindow = nullptr;
	NumptyBehavior::DeletePointer(m_WorldManager);
	m_WorldManager = nullptr;
	NumptyBehavior::DeletePointer(m_GUI);
	m_GUI = nullptr;
	NumptyBehavior::DeletePointer(m_AudioManager);
	m_AudioManager = nullptr;
	m_TextureMaster = nullptr;
	m_Event = nullptr;
}

void CMainMenuScene::CreateMainMenu()
{
	DeletePointer(m_MainMenu);
	m_MainMenu = new CMainMenu(m_RenderWindow, m_Font, "Backgrounds/OriBG.jpg");
}

void CMainMenuScene::DestoryMainMenu()
{
	DeletePointer(m_MainMenu);
	m_MainMenu = nullptr;
}
