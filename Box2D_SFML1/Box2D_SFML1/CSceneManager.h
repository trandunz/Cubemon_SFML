#pragma once

#include "CGameScene.h"
#include "CMainMenuScene.h"
#include "CBattleScene.h"

class CSceneManager : NumptyBehavior
{
public:
	CSceneManager(sf::RenderWindow* _renderWindow, sf::Event& _event, TextureMaster* _textureMaster);
	virtual ~CSceneManager();

	virtual void Start();
	virtual void Update();
	virtual void PolledUpdate();
	virtual void Render();

	void ChangeScenes();

	void CheckForMARKASDESTROY();

	void CleanupAllScenes();
	void CleanupGameScenes();
	void CleanupMainMenuScenes();
private:

	std::vector<CGameScene*> m_GameSceneVector = {};
	std::vector<CMainMenuScene*> m_MainMenuSceneVector = {};
	CBattleScene* m_BattleScene = nullptr;

	sf::RenderWindow* m_RenderWindow = nullptr;
	TextureMaster* m_TextureMaster = nullptr;
	sf::Event* m_Event = nullptr;

	sf::Clock m_SceneTimer;
	float m_SceneChangeDelay = 0.5f;

	bool m_GameScene;

	int m_LastFramesScene = 0;
};

