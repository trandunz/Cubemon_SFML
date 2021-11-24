#include "CBlizzardBird.h"

CBlizzardBird::CBlizzardBird(sf::RenderWindow* _renderWindow, AudioManager* _audioManager)
{
	m_AudioManager = _audioManager;
	m_RenderWindow = _renderWindow;
	m_SpriteTexture = new sf::Texture();
	m_CubeType = CUBEMONTYPE::BLIZZARDBIRD;

	Start();
}

CBlizzardBird::CBlizzardBird(sf::RenderWindow* _renderWindow, b2World* _world, b2Body& _playerBody)
{
	m_RenderWindow = _renderWindow;
	m_World = _world;
	m_PlayerBody = &_playerBody;

	m_SpriteTexture = new sf::Texture();

	CreateBody(_playerBody.GetPosition().x, _playerBody.GetPosition().y + 50, b2_dynamicBody);

	m_CubeType = CUBEMONTYPE::BLIZZARDBIRD;

	Start();
}

CBlizzardBird::~CBlizzardBird()
{
	DestoryShape();
	DestroyBody();

	DeletePointer(m_SpriteTexture);
	m_PlayerBody = nullptr;
	m_SpriteTexture = nullptr;
	m_RenderWindow = nullptr;
	m_TextureMaster = nullptr;
	m_AudioManager = nullptr;
	m_ParticleSystem = nullptr;
	m_Shape = nullptr;
	m_Body = nullptr;
	m_World = nullptr;
}

void CBlizzardBird::Start()
{
	m_Name = "Blizzard Bird";
	CreateShape();
	LoadSpriteTexture(LoadTexture(m_SpriteTexture, "Cubemon/Blizzard_Bird.png", false), m_Shape);
	m_Shape->setScale(0.5f, 0.5f);
	SetOriginToCenter(*m_Shape);
}

void CBlizzardBird::Update()
{
	EXPLvlUpCheck();
	if (m_Body != nullptr)
	{
		SetShapeToB2Body();

		if (m_PlayerBody->GetLinearVelocity().x < -0.1f)
		{
			m_Body->SetTransform(b2Vec2(m_PlayerBody->GetPosition().x + 50 / m_Scale, m_PlayerBody->GetPosition().y), 0.0f);
		}
		else if (m_PlayerBody->GetLinearVelocity().x > 0.1f)
		{
			m_Body->SetTransform(b2Vec2(m_PlayerBody->GetPosition().x - 50 / m_Scale, m_PlayerBody->GetPosition().y), 0.0f);
		}
		else if (m_PlayerBody->GetLinearVelocity().y < -0.1f)
		{
			m_Body->SetTransform(b2Vec2(m_PlayerBody->GetPosition().x, m_PlayerBody->GetPosition().y + 50 / m_Scale), 0.0f);
		}
		else if (m_PlayerBody->GetLinearVelocity().y > 0.1f)
		{
			m_Body->SetTransform(b2Vec2(m_PlayerBody->GetPosition().x, m_PlayerBody->GetPosition().y - 50 / m_Scale), 0.0f);
		}
		else
		{
			m_Body->SetTransform(b2Vec2(m_PlayerBody->GetPosition().x, m_PlayerBody->GetPosition().y - 50 / m_Scale), 0.0f);
		}
	}


	Render();
}

void CBlizzardBird::Render()
{
	RenderSpritePointer(m_RenderWindow, m_Shape);
}

void CBlizzardBird::Movement()
{
}

void CBlizzardBird::Attack()
{
}
