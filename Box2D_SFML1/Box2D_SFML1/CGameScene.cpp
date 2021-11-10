#include "CGameScene.h"

CGameScene::CGameScene(sf::RenderWindow* _renderWindow, TextureMaster* _textureMaster, sf::Event& _event)
{
	m_RenderWindow = _renderWindow;
	m_TextureMaster = _textureMaster;
	m_Event = &_event;
	m_Font.loadFromFile("Resources/Fonts/ANDYB.TTF");
}

CGameScene::~CGameScene()
{
	CleanupAllPointers();
}

void CGameScene::Start()
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
	m_WorldManager->Start(m_AudioManager);
	m_GUI->Start();

	m_ContactListener = new CContactListener();
	if (m_World != nullptr)
	{
		m_World->SetContactListener(m_ContactListener);
	}
}

void CGameScene::Update()
{
	m_MousePos = m_RenderWindow->mapPixelToCoords(sf::Mouse::getPosition(*m_RenderWindow), m_View);
	if (!IsNullptr(m_WorldManager))
	{
		m_WorldManager->Update();
	}
	if (!IsNullptr(m_Player) && !m_Player->m_MARKASDESTROY)
	{
		CenterViewTo(m_Player->GetShape());

		m_Player->Update(m_MousePos);
	}
	else if (IsNullptr(m_Player))
	{
		GameOverScreen();
	}
}

void CGameScene::PolledUpdate()
{
	while (m_RenderWindow->pollEvent(*m_Event))
	{
		if (m_Event->type == sf::Event::Closed)
		{
			m_RenderWindow->close();
			break;
		}
		if (m_Event->type == sf::Event::KeyPressed)
		{
			if (m_Player != nullptr && !m_Player->m_MARKASDESTROY)
			{
				m_Player->PollMovement(*m_Event);
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab))
				{
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::J))
				{
					m_Player->m_MARKASDESTROY = true;
					break;
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::PageUp))
				{
					m_Player->TakeDamage(25.0f);
					break;
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::PageDown))
				{
					m_Player->Heal(25.0f);
					break;
				}
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::K) && m_Player == nullptr)
			{
				CreatePlayer();
				m_Player->Start();
				break;
			}

			if (sf::Event::KeyPressed && m_Event->key.code == sf::Keyboard::N)
			{
				m_WorldManager->CleanupTiles();
				break;
			}
			else if (sf::Event::KeyPressed && m_Event->key.code == sf::Keyboard::M)
			{
				m_WorldManager->ImportWorldFromINI();
				break;
			}
		}
		if (m_Event->type == sf::Event::MouseWheelScrolled)
		{
		}
	}
}

void CGameScene::Render()
{
	m_RenderWindow->setView(m_View);

	sf::Shader* defaultShader = NULL;

	if (m_WorldManager != nullptr)
	{
		m_WorldManager->Render();
	}
	if (m_Player != nullptr && !m_Player->m_MARKASDESTROY)
	{
		m_Player->RenderCubemon();
		m_Player->Render();

		if (m_WorldManager != nullptr)
		{
			m_WorldManager->RenderOnTop();
		}

		// UI
		m_RenderWindow->setView(m_UIView);
		//
		m_GUI->TimerUI();
		m_GUI->HealthAndManaUI(m_RenderWindow, m_UIView, m_Player);
		m_GUI->Render(m_Player);
		//
	}

	m_RenderWindow->draw(m_FadeScreen, defaultShader);
	m_RenderWindow->draw(m_GameOverText, defaultShader);
}

void CGameScene::CheckForMARKASDESTROY()
{
	if (m_Player != nullptr)
	{
		for (auto& pointer : m_Player->GetCubemonVector())
		{
			if (pointer->m_MARKASDESTROY)
			{
				NumptyBehavior::DeletePointer(pointer);
				pointer = nullptr;
			}
		}

		if (m_Player->m_MARKASDESTROY)
		{
			NumptyBehavior::DeletePointer(m_Player);
			m_Player = nullptr;
		}
	}
}

void CGameScene::CreateB2World()
{
	NumptyBehavior::DeletePointer(m_WorldManager);
	m_WorldManager = nullptr;
	NumptyBehavior::DeletePointer(m_World);
	m_World = nullptr;
	m_World = new b2World(m_Gravity);
}

void CGameScene::CreateWorldManager()
{
	NumptyBehavior::DeletePointer(m_WorldManager);
	m_WorldManager = nullptr;
	m_WorldManager = new WorldManager(m_RenderWindow,*m_World, m_TextureMaster);
}

void CGameScene::CreatePlayer()
{
	DeletePointer(m_Player);
	m_Player = nullptr;
	m_Player = new Player(m_RenderWindow, *m_World, m_AudioManager, m_TextureMaster);
}

void CGameScene::CreateAudioManager()
{
	NumptyBehavior::DeletePointer(m_AudioManager);
	m_AudioManager = nullptr;
	m_AudioManager = new AudioManager;
}

void CGameScene::CreateGUI()
{
	NumptyBehavior::DeletePointer(m_GUI);
	m_GUI = nullptr;
	m_GUI = new GUI(m_RenderWindow, m_TextureMaster, m_Font);
}

void CGameScene::PlayMusic()
{
	m_AudioManager->PlayMusic();
}

void CGameScene::InitGameOver()
{
	m_Font.loadFromFile("Resources/Fonts/ANDYB.TTF");
	m_FadeScreen.setSize(sf::Vector2f(30000, 30000));
	m_FadeScreen.setOrigin(sf::Vector2f(15000, 15000));
	m_FadeScreen.setFillColor(sf::Color::Transparent);
	m_GameOverText.setCharacterSize(1000);
	m_GameOverText.setString("Game Over");
	m_GameOverText.setFont(m_Font);
	m_GameOverText.setOrigin(m_GameOverText.getGlobalBounds().width / 2, m_GameOverText.getGlobalBounds().height / 2);
	m_GameOverText.setFillColor(sf::Color::Transparent);
	m_GameOverText.setOutlineColor(sf::Color::Transparent);
}

void CGameScene::GameOverScreen()
{
	float elapsedtime = m_FadeTimer.getElapsedTime().asSeconds() / m_PlayerRespawnTime;

	if (m_DeathTimer.getElapsedTime().asSeconds() >= m_PlayerRespawnTime)
	{
		m_FadeScreen.setFillColor(sf::Color::Transparent);
		m_GameOverText.setFillColor(sf::Color::Transparent);
	}
	else
	{
		m_FadeScreen.setFillColor(sf::Color(0, 0, 0, (sf::Uint8) elapsedtime * 255));
		m_GameOverText.setFillColor(sf::Color(255, 0, 0, (sf::Uint8) elapsedtime * 255));
	}
}

void CGameScene::CenterViewTo(sf::Sprite* _object)
{
	m_View.setCenter(_object->getPosition());
	m_RenderWindow->setView(m_View);
}

void CGameScene::InitUIView()
{
	m_UIView = sf::View(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(m_RenderWindow->getSize().x, m_RenderWindow->getSize().y));
}

void CGameScene::CleanupAllPointers()
{
	NumptyBehavior::DeletePointer(m_Player);
	m_Player = nullptr;
	m_RenderWindow = nullptr;
	NumptyBehavior::DeletePointer(m_WorldManager);
	m_WorldManager = nullptr;
	NumptyBehavior::DeletePointer(m_GUI);
	m_GUI = nullptr;
	NumptyBehavior::DeletePointer(m_AudioManager);
	m_AudioManager = nullptr;
	m_TextureMaster = nullptr;
	m_Event = nullptr;
	NumptyBehavior::DeletePointer(m_ContactListener);
	m_ContactListener = nullptr;
}

void CGameScene::InitView()
{
	m_View = sf::View(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(m_RenderWindow->getSize().x, m_RenderWindow->getSize().y));
	m_View.zoom(ZOOM);
}
