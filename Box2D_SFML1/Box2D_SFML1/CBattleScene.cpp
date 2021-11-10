#include "CBattleScene.h"

CBattleScene::CBattleScene(sf::RenderWindow* _renderWindow, TextureMaster* _textureMaster, sf::Event& _event)
{
	m_RenderWindow = _renderWindow;
	m_TextureMaster = _textureMaster;
	m_Event = &_event;
	m_Font.loadFromFile("Resources/Fonts/ANDYB.TTF");
}

CBattleScene::~CBattleScene()
{
	CleanupAllPointers();
}

void CBattleScene::Start()
{
	InitUIView();
	InitWorldView();
	InitBackground();

	CreateAudioManager();
	m_AudioManager->PlayMusic(-1);

	CreateGUI();
	InitCubemon();
	m_GUI->InitCubemonHUD(m_FriendlyCubemon, m_EnemyCubemon);
}

void CBattleScene::Update()
{
	if (m_EnemyCubemon != nullptr)
	{
		m_EnemyCubemon->Update();
	}

	if (m_FriendlyCubemon != nullptr)
	{
		m_FriendlyCubemon->Update();
	}
}

void CBattleScene::PolledUpdate()
{
	while (m_RenderWindow->pollEvent(*m_Event))
	{
		if (m_Event->type == sf::Event::Closed)
		{
			m_RenderWindow->close();
			CleanupAllPointers();
			return;
		}
	}
}

void CBattleScene::Render()
{
	m_RenderWindow->draw(m_Background);

	if (m_EnemyCubemon != nullptr)
	{
		m_EnemyCubemon->Render();
	}

	if (m_FriendlyCubemon != nullptr)
	{
		m_FriendlyCubemon->Render();
	}

	if (m_GUI != nullptr)
	{
		m_GUI->BattleUI(m_UIView, m_WorldView);
		m_GUI->HandleCubemonHUD(m_UIView, m_WorldView);
	}
}

void CBattleScene::CheckForMARKASDESTROY()
{
}

void CBattleScene::InitCubemon(ICubemon::CUBEMONTYPE _friendly, ICubemon::CUBEMONTYPE _enemy)
{
	if (_enemy == ICubemon::CUBEMONTYPE::THALLIC)
	{
		m_EnemyCubemon = new CThallic(m_RenderWindow);
	}
	else if (_enemy == ICubemon::CUBEMONTYPE::KINDLING)
	{
		m_EnemyCubemon = new CKindling(m_RenderWindow);
	}
	else
	{
		int ran = INT_MAX;
		srand((unsigned)time(NULL));
		ran = rand() % 2;
		if (ran == 0)
		{
			m_EnemyCubemon = new CThallic(m_RenderWindow);
		}
		else if (ran == 1)
		{
			m_EnemyCubemon = new CKindling(m_RenderWindow);
		}
	}

	if (_friendly == ICubemon::CUBEMONTYPE::THALLIC)
	{
		m_FriendlyCubemon = new CThallic(m_RenderWindow);
	}
	else if (_friendly == ICubemon::CUBEMONTYPE::KINDLING)
	{
		m_FriendlyCubemon = new CKindling(m_RenderWindow);
	}
	else
	{
		if (m_GUI->GetCurrentCubemon() == ICubemon::CUBEMONTYPE::THALLIC)
		{
			m_FriendlyCubemon = new CThallic(m_RenderWindow);
		}
		else if (m_GUI->GetCurrentCubemon() == ICubemon::CUBEMONTYPE::KINDLING)
		{
			m_FriendlyCubemon = new CKindling(m_RenderWindow);
		}
	}

	if (m_EnemyCubemon != nullptr)
	{
		
		m_EnemyCubemon->SetSpritePos(sf::Vector2f(m_RenderWindow->getView().getCenter().x + m_RenderWindow->getView().getSize().x / 6, m_RenderWindow->getView().getCenter().y - m_RenderWindow->getView().getSize().y / 6));
		m_EnemyCubemon->SetSpriteScale(sf::Vector2f(1.5, 1.5));
	}
	if (m_FriendlyCubemon != nullptr)
	{
		
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
	DeletePointer(m_EnemyCubemon);
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
