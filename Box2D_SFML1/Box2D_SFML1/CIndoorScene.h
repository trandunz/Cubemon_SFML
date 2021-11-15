#pragma once
#include "CGameScene.h"
class CIndoorScene : public CGameScene
{
public:
	inline CIndoorScene(sf::RenderWindow* _renderWindow, TextureMaster* _textureMaster, sf::Event& _event)
	{
		m_RenderWindow = _renderWindow;
		m_TextureMaster = _textureMaster;
		m_Event = &_event;
		m_Font.loadFromFile("Resources/Fonts/ANDYB.TTF");
	}

	inline virtual ~CIndoorScene() 
	{
		CleanupAllPointers();
	};

	inline virtual void Start() override
	{
		CreateAudioManager();
		CreateB2World();
		CreatePlayer();
		CreateGUI();
		CreateWorldManager();

		PlayMusic();
		InitUIView();
		InitView();
		m_Player->Start();
		m_WorldManager->Start(m_AudioManager, "World.ini");
		m_GUI->Start();

		m_ContactListener = new CContactListener();
		if (m_World != nullptr)
		{
			m_World->SetContactListener(m_ContactListener);
		}
	}

	inline virtual void Update()
	{
		CGameScene::Update();
	}
	inline virtual void PolledUpdate()
	{
		CGameScene::PolledUpdate();
	}

};


