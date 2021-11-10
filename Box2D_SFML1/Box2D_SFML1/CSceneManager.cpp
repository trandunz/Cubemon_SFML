#include "CSceneManager.h"

CSceneManager::CSceneManager(sf::RenderWindow* _renderWindow, sf::Event& _event, TextureMaster* _textureMaster)
{
	m_RenderWindow = _renderWindow;
	m_Event = &_event;
	m_TextureMaster = _textureMaster;
	m_GameScene = false;

	m_MainMenuSceneVector.push_back(new CMainMenuScene(m_RenderWindow, *m_Event));
}

CSceneManager::~CSceneManager()
{
	CleanupAllScenes();

	m_RenderWindow = nullptr;
	m_TextureMaster = nullptr;
	m_Event = nullptr;
}

void CSceneManager::Start()
{
	if (!m_GameScene && m_MainMenuSceneVector.size() > 0 && m_BattleScene == nullptr)
	{
		m_MainMenuSceneVector.back()->Start();
	}
	else if (m_GameSceneVector.size() > 0 && m_BattleScene == nullptr)
	{
		m_GameSceneVector.back()->Start();
	}
	else if (m_BattleScene != nullptr)
	{
		m_BattleScene->Start();
	}
}

void CSceneManager::Update()
{
	if (!m_GameScene && m_MainMenuSceneVector.size() > 0 && m_BattleScene == nullptr)
	{
		m_MainMenuSceneVector.back()->Update();
	}
	else if (m_GameSceneVector.size() > 0 && m_BattleScene == nullptr)
	{
		m_GameSceneVector.back()->Update();
	}
	else if (m_BattleScene != nullptr)
	{
		m_BattleScene->Update();
	}
}

void CSceneManager::PolledUpdate()
{
	if (!m_GameScene && m_MainMenuSceneVector.size() > 0)
	{
		m_MainMenuSceneVector.back()->PolledUpdate();
	}
	else if (m_GameSceneVector.size() > 0)
	{
		m_GameSceneVector.back()->PolledUpdate();
	}
	if (m_BattleScene != nullptr)
	{
		m_BattleScene->PolledUpdate();
	}

	ChangeScenes();
}

void CSceneManager::Render()
{
	if (!m_GameScene && m_MainMenuSceneVector.size() > 0 && m_BattleScene == nullptr)
	{
		m_MainMenuSceneVector.back()->Render();
	}
	else if (m_GameSceneVector.size() > 0 && m_BattleScene == nullptr)
	{
		m_GameSceneVector.back()->Render();
	}
	else if (m_BattleScene != nullptr)
	{
		m_BattleScene->Render();
	}
}

void CSceneManager::ChangeScenes()
{
	if (m_SceneTimer.getElapsedTime().asSeconds() >= m_SceneChangeDelay && m_LastFramesScene != ReturnSceneChange())
	{
		if (ReturnSceneChange() == 1)
		{
			CleanupAllScenes();
			m_GameSceneVector.push_back(new CGameScene(m_RenderWindow, m_TextureMaster, *m_Event));
		}
		else if (ReturnSceneChange() == -1)
		{
			CleanupAllScenes();
			m_BattleScene = new CBattleScene(m_RenderWindow, m_TextureMaster, *m_Event);
		}
		else if (ReturnSceneChange() == 0)
		{
			CleanupAllScenes();
			m_MainMenuSceneVector.push_back(new CMainMenuScene(m_RenderWindow, *m_Event));
		}

		Start();
		m_SceneTimer.restart();
	}

	m_LastFramesScene = ReturnSceneChange();

	if (sf::Event::KeyPressed && m_Event->key.code == sf::Keyboard::F1 && m_SceneTimer.getElapsedTime().asSeconds() >= m_SceneChangeDelay)
	{
		InterceptSceneChange(0);
		return;
	}
	else if (sf::Event::KeyPressed && m_Event->key.code == sf::Keyboard::F2 && m_SceneTimer.getElapsedTime().asSeconds() >= m_SceneChangeDelay)
	{
		InterceptSceneChange(1);
		return;
	}
	else if (sf::Event::KeyPressed && m_Event->key.code == sf::Keyboard::F3 && m_SceneTimer.getElapsedTime().asSeconds() >= m_SceneChangeDelay)
	{
		InterceptSceneChange(-1);
		return;
	}
	else if (sf::Event::KeyPressed && m_Event->key.code == sf::Keyboard::Numpad2 && m_SceneTimer.getElapsedTime().asSeconds() >= m_SceneChangeDelay)
	{
		Player::ResetPlayerData();
		return;
	}
}

void CSceneManager::CheckForMARKASDESTROY()
{
	if (m_GameSceneVector.size() > 0)
	{
		m_GameSceneVector.back()->CheckForMARKASDESTROY();
	}
	if (m_MainMenuSceneVector.size() > 0)
	{
		m_MainMenuSceneVector.back()->CheckForMARKASDESTROY();
	}
	if (m_BattleScene != nullptr)
	{
		m_BattleScene->CheckForMARKASDESTROY();
	}
}

void CSceneManager::CleanupAllScenes()
{
	DeletePointer(m_BattleScene);
	m_BattleScene = nullptr;
	CleanupGameScenes();
	CleanupMainMenuScenes();
}

void CSceneManager::CleanupGameScenes()
{
	for (auto& pointer : m_GameSceneVector)
	{
		DeletePointer(pointer);
		pointer = nullptr;
	}
	m_GameSceneVector.erase(std::remove(m_GameSceneVector.begin(), m_GameSceneVector.end(), nullptr), m_GameSceneVector.end());

}

void CSceneManager::CleanupMainMenuScenes()
{
	for (auto& pointer : m_MainMenuSceneVector)
	{
		DeletePointer(pointer);
		pointer = nullptr;
	}
	m_MainMenuSceneVector.erase(std::remove(m_MainMenuSceneVector.begin(), m_MainMenuSceneVector.end(), nullptr), m_MainMenuSceneVector.end());
}
