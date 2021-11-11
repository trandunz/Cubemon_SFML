#include "Player.h"

Player::Player(sf::RenderWindow* _renderWindow, b2World& _world, AudioManager* _audiomanager, TextureMaster* _texturemaster)
{
	m_RenderWindow = _renderWindow;
	m_World = &_world;
	m_AudioManager = _audiomanager;
	m_TextureMaster = _texturemaster;
}

Player::~Player()
{
	WriteCubmonData();
	WritePlayerData();
	CleanupCubemon();
	DestoryShape();
	DestroyBody();
	DeletePointer(m_ParticleSystem);
	m_ParticleSystem = nullptr;
	m_AudioManager = nullptr;
	m_RenderWindow = nullptr;
	m_World = nullptr;
	m_TextureMaster = nullptr; 
}

void Player::Start()
{
	CreateShape();
	LoadSpriteTexture(LoadTexture(&m_SpriteSheet, "Player/Player_SpriteSheett.png", false), m_Shape, true, false);
	m_Shape->setScale(10, 10);

	CreateBody(GrabPlayerData().x, GrabPlayerData().y, b2_dynamicBody);

	GrabCubmonData();
}

void Player::Update(sf::Vector2f _mousepos)
{
	m_MousePos = _mousepos;

	Movement();

	SetShapeToB2Body();

	for (b2Contact* contact = m_World->GetContactList(); contact; contact = contact->GetNext())
	{
		b2Fixture* a = contact->GetFixtureA();
		b2Fixture* b = contact->GetFixtureB();
		b2WorldManifold worldManifold;
		contact->GetWorldManifold(&worldManifold);

		// Bush / Player To Sensor Action
		if ((a->GetBody() == m_Body || b->GetBody() == m_Body) && (a->GetBody()->GetFixtureList()->IsSensor() || b->GetBody()->GetFixtureList()->IsSensor()))
		{
			if (m_EncounterClock.getElapsedTime().asSeconds() >= 5.0f)
			{
				srand((unsigned)time(0));
				int bushEncounter = rand() % 12;

				if (bushEncounter == 0)
				{
					m_Encounter = true;
					m_EncounterClock.restart();
					break;
				}
				else if (bushEncounter == 6)
				{
					m_Encounter = true;
					m_EncounterClock.restart();
					break;
				}
			}
			else
			{
				break;
			}
		}

		a = nullptr;
		b = nullptr;
	}

	if (m_ManaRegen.getElapsedTime().asSeconds() >= m_ManaRegenFrequency)
	{
		if (GetCurrentMana() < m_MaxMana)
		{
			SetCurrentMana(GetCurrentMana() + 1);
			m_ManaRegen.restart();
		}
	}

	UpdateCubemon();

	BattleTransition();
}

void Player::Render(sf::Shader* _defaultshader)
{
	RenderSpritePointer(m_RenderWindow, m_Shape);
}

void Player::PollMovement(sf::Event& _event)
{
}

void Player::Movement()
{
	if (m_Encounter == false)
	{
		float x = 0.f;
		float y = 0.f;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
		{
			if (m_Shape != nullptr)
			{
				m_Shape->setScale(-1, 1);
			}
			y = -1.f;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
		{
			if (m_Shape != nullptr)
			{
				m_Shape->setScale(-1, 1);
			}
			y = -1.f;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
		{
			if (m_Shape != nullptr)
			{
				m_Shape->setScale(-1, 1);
			}
			x = -1.f;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
		{
			if (m_Shape != nullptr)
			{
				m_Shape->setScale(-1, 1);
			}
			x = -1.f;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
		{
			if (m_Shape != nullptr)
			{
				m_Shape->setScale(-1, 1);
			}
			y = 1.f;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
		{
			if (m_Shape != nullptr)
			{
				m_Shape->setScale(-1, 1);
			}
			y = 1.f;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
		{
			if (m_Shape != nullptr)
			{
				m_Shape->setScale(1, 1);
			}
			x = 1.f;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
		{
			if (m_Shape != nullptr)
			{
				m_Shape->setScale(1, 1);
			}
			x = 1.f;
		}
		m_Velocity = b2Vec2(x, y);
		m_Velocity.Normalize();
		m_Body->SetLinearVelocity((float)m_iMovementSpeed * m_Velocity);

		Animation(m_Velocity);
	}
}

void Player::SetCurrentMana(float _mana)
{
	m_CurrentMana = _mana;
}

float Player::GetCurrentMana()
{
	return m_CurrentMana;
}

void Player::TakeDamage(float _damage)
{
	if (m_DamageTakenTimer.getElapsedTime().asSeconds() >= 0.3f)
	{
		for (int i = 0; i < _damage; i++)
		{
			if (m_CurrentHealth > 1)
			{
				m_CurrentHealth--;
			}
			else
			{
				m_MARKASDESTROY = true;
				break;
			}
		}
		std::cout << "Damage Taken : " << _damage << std::endl;
		std::cout << "Current Health : " << GetCurrentHealth() << std::endl;
		m_DamageTakenTimer.restart();
	}
}

void Player::Heal(float _amount)
{
	if (m_DamageTakenTimer.getElapsedTime().asSeconds() >= 0.3f)
	{
		for (int i = 0; i < _amount; i++)
		{
			if (m_CurrentHealth < m_MaxHealth && m_CurrentHealth > 0)
			{
				m_CurrentHealth++;
			}
			else
			{
				break;
			}
		}
		std::cout << "Player Healed!" << std::endl;
		std::cout << "Current Health : " << GetCurrentHealth() << std::endl;
		m_DamageTakenTimer.restart();
	}
}

void Player::SetCurrentHealth(float _health)
{
	m_CurrentHealth = _health;
}

float Player::GetCurrentHealth()
{
	return m_CurrentHealth;
}

void Player::AddCubemon(ICubemon* _cubeMon)
{
	m_CubemonVector.push_back(_cubeMon);
	_cubeMon = nullptr;
}

void Player::UpdateCubemon()
{
	m_CubemonVector.front()->Update();
}

void Player::RenderCubemon()
{
	float Mag1 = 0;
	float dx = 0;
	float dy = 0;

	// Everything Else
	dx = m_CubemonVector.front()->GetShape()->getPosition().x - m_RenderWindow->getView().getCenter().x;
	dy = m_CubemonVector.front()->GetShape()->getPosition().y - m_RenderWindow->getView().getCenter().y;
	Mag1 = Mag(dx, dy);
	if (Mag1 < 1200)
	{
		m_CubemonVector.front()->Render();
	}
}

void Player::LateCubemonRender()
{
	float Mag1 = 0;
	float dx = 0;
	float dy = 0;

	// Everything Else
	for (int i = 0; i < m_CubemonVector.size(); i++)
	{
		dx = m_CubemonVector[i]->GetShape()->getPosition().x - m_RenderWindow->getView().getCenter().x;
		dy = m_CubemonVector[i]->GetShape()->getPosition().y - m_RenderWindow->getView().getCenter().y;
		Mag1 = Mag(dx, dy);
		if (Mag1 < 1200 && dy >= 0)
		{
			m_CubemonVector[i]->Render();
		}
	}
}

void Player::Animation(b2Vec2 _movementVector)
{
	bool _ignoreScale = false;
	if (_movementVector.x > 0.1f)
	{
		if (m_Shape->getTextureRect().left != 0 && m_Shape->getTextureRect().left != 14)
		{
			m_Shape->setTextureRect(sf::IntRect(0, 0, 14, 16));
		}
		else if (m_AnimationClock.getElapsedTime().asSeconds() > m_FrameDelay && m_Shape->getTextureRect().left != 14)
		{
			m_Shape->setTextureRect(sf::IntRect(m_Shape->getTextureRect().left + 14, 0, 14, 16));
			m_AnimationClock.restart();
		}
		else if (m_AnimationClock.getElapsedTime().asSeconds() > m_FrameDelay && m_Shape->getTextureRect().left == 14)
		{
			m_Shape->setTextureRect(sf::IntRect(0, 0, 14, 16));
			m_AnimationClock.restart();
		}
	}
	else if (_movementVector.x < -0.1f)
	{
		_ignoreScale = true;
		m_Shape->setScale(-SPRITE_SHEET_SCALE, SPRITE_SHEET_SCALE);
		if (m_Shape->getTextureRect().left != 0 && m_Shape->getTextureRect().left != 14)
		{
			m_Shape->setTextureRect(sf::IntRect(0, 0, 14, 16));
		}
		else if (m_AnimationClock.getElapsedTime().asSeconds() > m_FrameDelay && m_Shape->getTextureRect().left != 14)
		{
			m_Shape->setTextureRect(sf::IntRect(m_Shape->getTextureRect().left + 14, 0, 14, 16));
			m_AnimationClock.restart();
		}
		else if (m_AnimationClock.getElapsedTime().asSeconds() > m_FrameDelay && m_Shape->getTextureRect().left == 14)
		{
			m_Shape->setTextureRect(sf::IntRect(0, 0, 14, 16));
			m_AnimationClock.restart();
		}
	}
	else if (_movementVector.y < -0.1f)
	{
		if (m_Shape->getTextureRect().left != 28 && m_Shape->getTextureRect().left != 42)
		{
			m_Shape->setTextureRect(sf::IntRect(28, 0, 14, 16));
		}
		else if (m_AnimationClock.getElapsedTime().asSeconds() > m_FrameDelay && m_Shape->getTextureRect().left != 42)
		{
			m_Shape->setTextureRect(sf::IntRect(m_Shape->getTextureRect().left + 14, 0, 14, 16));
			m_AnimationClock.restart();
		}
		else if (m_AnimationClock.getElapsedTime().asSeconds() > m_FrameDelay && m_Shape->getTextureRect().left == 42)
		{
			m_Shape->setTextureRect(sf::IntRect(28, 0, 14, 16));
			m_AnimationClock.restart();
		}
	}
	else if (_movementVector.y > 0.1f)
	{
		if (m_Shape->getTextureRect().left != 56 && m_Shape->getTextureRect().left != 70)
		{
			m_Shape->setTextureRect(sf::IntRect(56, 0, 14, 16));
		}
		else if (m_AnimationClock.getElapsedTime().asSeconds() > m_FrameDelay && m_Shape->getTextureRect().left != 70)
		{
			m_Shape->setTextureRect(sf::IntRect(m_Shape->getTextureRect().left + 14, 0, 14, 16));
			m_AnimationClock.restart();
		}
		else if (m_AnimationClock.getElapsedTime().asSeconds() > m_FrameDelay && m_Shape->getTextureRect().left == 70)
		{
			m_Shape->setTextureRect(sf::IntRect(56, 0, 14, 16));
			m_AnimationClock.restart();
		}
	}
	else
	{
		m_Shape->setTextureRect(sf::IntRect(84, 0, 14, 16));
	}

	if (!_ignoreScale)
	{
		m_Shape->setOrigin(7, 8);
		m_Shape->setScale(SPRITE_SHEET_SCALE, SPRITE_SHEET_SCALE);
	}
}

sf::Vector2f Player::GrabPlayerData()
{
	std::ifstream file;
	std::string currentLine;
	float posX = 0;
	float posY = 0;
	int iterator = 0;
	file.open("Resources/Output/PlayerData.ini");
	if (file.is_open())
	{
		while (std::getline(file, currentLine))
		{
			if (iterator == 0)
			{
				std::size_t pos = currentLine.find('=');
				std::string value = currentLine.substr(pos + 1);

				posX = std::stod(value);
				iterator++;
			}
			else
			{
				std::size_t pos2 = currentLine.find('=');
				std::string value2 = currentLine.substr(pos2 + 1);

				posY = std::stod(value2);
			}
			
		}
	}
	file.close();

	return sf::Vector2f(posX, posY);
}

void Player::WritePlayerData()
{
	std::ofstream file;

	file.open("Resources/Output/PlayerData.ini");
	if (file.is_open())
	{
		file.clear();
		file << "x =" << m_Shape->getPosition().x << std::endl << "y =" << m_Shape->getPosition().y;
	}
	file.close();
}

void Player::WriteCubmonData()
{
	std::ofstream file;

	file.open("Resources/Output/CubemonData.ini");
	if (file.is_open())
	{
		file.clear();
		for (auto& cubemon : m_CubemonVector)
		{
			file << (int)cubemon->m_CubeType << ",";
		}
	}
	file.close();
}

void Player::GrabCubmonData()
{
	CleanupCubemon();
	std::ifstream file;
	std::string currentLine;

	char m_Type = 0;

	file.open("Resources/Output/CubemonData.ini");
	if (file.is_open())
	{
		while (file.get(m_Type))
		{
			if (m_Type == (int)ICubemon::CUBEMONTYPE::THALLIC + ASCIIOFFSET)
			{
				AddCubemon(new CThallic(m_RenderWindow, m_World, *m_Body));
			}
			else if (m_Type == (int)ICubemon::CUBEMONTYPE::KINDLING + ASCIIOFFSET)
			{
				AddCubemon(new CKindling(m_RenderWindow, m_World, *m_Body));
			}
			else if (m_Type == (int)ICubemon::CUBEMONTYPE::BRUTUS + ASCIIOFFSET)
			{
				AddCubemon(new CBrutus(m_RenderWindow, m_World, *m_Body));
			}
		}
		file.close();
	}
}

std::vector<ICubemon::CUBEMONTYPE> Player::ReturnCubemonData()
{
	std::ifstream file;
	std::string currentLine;
	char m_Type = 0;

	std::vector<ICubemon::CUBEMONTYPE> cubemon{};

	file.open("Resources/Output/CubemonData.ini");
	if (file.is_open())
	{
		while (file.get(m_Type))
		{
			if (m_Type == (int)ICubemon::CUBEMONTYPE::THALLIC + ASCIIOFFSET)
			{
				cubemon.push_back(ICubemon::CUBEMONTYPE::THALLIC);
			}
			else if (m_Type == (int)ICubemon::CUBEMONTYPE::KINDLING + ASCIIOFFSET)
			{
				cubemon.push_back(ICubemon::CUBEMONTYPE::KINDLING);
			}
		}
		file.close();
	}

	return cubemon;
}

void Player::WriteInventoryData()
{
}

void Player::ReadInventoryData()
{
}

void Player::BattleTransition()
{
	if (m_BattleTimer.getElapsedTime().asSeconds() >= 2.0f && m_Encounter == true)
	{
		m_Encounter = false;
		m_BattleTimer.restart();
		InterceptSceneChange(-1);
	}
}

void Player::ResetPlayerData()
{
	std::ofstream file;

	file.open("Resources/Output/PlayerData.ini");
	if (file.is_open())
	{
		file.clear();
		file << "x =" << 0 << std::endl << "y =" << -100;
	}
	file.close();
}

void Player::CleanupCubemon()
{
	for (auto& pointer : m_CubemonVector)
	{
		DeletePointer(pointer);
		pointer = nullptr;
	}
	m_CubemonVector.erase(std::remove(m_CubemonVector.begin(), m_CubemonVector.end(), nullptr), m_CubemonVector.end());

}
