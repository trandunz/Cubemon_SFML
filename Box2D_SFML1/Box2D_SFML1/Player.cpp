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
	if (!m_bEnteredRoom)
	{
		WritePlayerData();
	}
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
	Animation(m_Velocity);

	if (ReturnSceneChange() == 2)
	{
		CreateBody(0.0f, 0.0f, b2_dynamicBody);
	}
	else
	{
		CreateBody(GrabPlayerData().x, GrabPlayerData().y, b2_dynamicBody);
	}

	GrabWaterBootsData();

	GrabCubmonData();
}

void Player::Update(sf::Vector2f _mousepos)
{
	m_MousePos = _mousepos;

	Movement();

	SetShapeToB2Body();

	HandleInteractions();

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
	if (ReturnSceneChange() == 1)
	{

	}
	else
	{
		std::ofstream file;

		file.open("Resources/Output/PlayerData.ini");
		if (file.is_open())
		{
			file.clear();
			file << "x =" << m_Shape->getPosition().x << std::endl << "y =" << m_Shape->getPosition().y;
		}
		file.close();

		file.open("Resources/Output/WaterBootsData.ini");
		if (file.is_open())
		{
			file.clear();
			file << m_WaterBoots;
		}
		file.close();
	}
	
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
			else if (m_Type == (int)ICubemon::CUBEMONTYPE::WIRLSON + ASCIIOFFSET)
			{
				AddCubemon(new CWirlson(m_RenderWindow, m_World, *m_Body));
			}
			else if (m_Type == (int)ICubemon::CUBEMONTYPE::DUSTDEVIL + ASCIIOFFSET)
			{
				AddCubemon(new CDustDevil(m_RenderWindow, m_World, *m_Body));
			}
			else if (m_Type == (int)ICubemon::CUBEMONTYPE::BLIZZARDBIRD + ASCIIOFFSET)
			{
				AddCubemon(new CBlizzardBird(m_RenderWindow, m_World, *m_Body));
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
			else if (m_Type == (int)ICubemon::CUBEMONTYPE::BRUTUS + ASCIIOFFSET)
			{
				cubemon.push_back(ICubemon::CUBEMONTYPE::BRUTUS);
			}
			else if (m_Type == (int)ICubemon::CUBEMONTYPE::WIRLSON + ASCIIOFFSET)
			{
				cubemon.push_back(ICubemon::CUBEMONTYPE::WIRLSON);
			}
			else if (m_Type == (int)ICubemon::CUBEMONTYPE::DUSTDEVIL + ASCIIOFFSET)
			{
				cubemon.push_back(ICubemon::CUBEMONTYPE::DUSTDEVIL);
			}
			else if (m_Type == (int)ICubemon::CUBEMONTYPE::BLIZZARDBIRD + ASCIIOFFSET)
			{
				cubemon.push_back(ICubemon::CUBEMONTYPE::BLIZZARDBIRD);
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

void Player::HealAllPokemon()
{
	std::ifstream file;
	std::vector<int> m_HPs{};
	std::string currentLine;
	file.open("Resources/Output/CubemonHPData.ini");
	if (file.is_open())
	{
		while (std::getline(file, currentLine))
		{
			std::size_t pos = currentLine.find('=');
			std::string value = currentLine.substr(pos + 1);

			m_HPs.push_back(100);
		}

	}
	file.close();

	std::ofstream oFile;
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
}

void Player::ToggleDebugSprint()
{
	if (m_iMovementSpeed == (7 * 5))
	{
		m_iMovementSpeed = 7;
	}
	else if (m_iMovementSpeed == 7)
	{
		m_iMovementSpeed *= 5;
	}
	else
	{
		m_iMovementSpeed = 7;
	}
}

void Player::CreateBody(float _posX, float _posY, b2BodyType _type, bool _sensor)
{
	// Body
	m_BodyDef = new b2BodyDef;
	m_BodyDef->position = b2Vec2(_posX / m_Scale, _posY / m_Scale);
	m_BodyDef->type = _type;
	m_BodyDef->fixedRotation = 1;
	m_BodyDef->linearDamping = 1.0f;
	m_BodyDef->gravityScale = 0;
	m_Body = m_World->CreateBody(m_BodyDef);

	// Shape
	m_b2pShape = new b2PolygonShape;
	m_b2pShape->SetAsBox((50.0f / 2) / m_Scale, (50.0f / 2) / m_Scale);

	// Fixture
	m_FixtureDef = new b2FixtureDef;
	if (_sensor)
	{
		m_FixtureDef->isSensor = true;
	}
	m_FixtureDef->density = 2.0f;
	m_FixtureDef->friction = 0.1f;
	m_FixtureDef->restitution = 0.2f;
	m_FixtureDef->shape = m_b2pShape;
	m_FixtureDef->filter.categoryBits = 3;
	m_FixtureDef->filter.groupIndex = -2;
	m_Body->CreateFixture(m_FixtureDef);
}

void Player::GrabWaterBootsData()
{
	std::ifstream file;
	std::string currentLine;
	file.open("Resources/Output/WaterBootsData.ini");
	if (file.is_open())
	{
		while (std::getline(file, currentLine))
		{
			m_WaterBoots = std::stoi(currentLine);
		}
	}
	file.close();
}

void Player::HandleInteractions()
{
	if (m_InteractionDelayTimer.getElapsedTime().asSeconds() >= 1.0f)
	{
		for (b2Contact* contact = m_World->GetContactList(); contact; contact = contact->GetNext())
		{
			b2Fixture* a = contact->GetFixtureA();
			b2Fixture* b = contact->GetFixtureB();
			b2WorldManifold worldManifold;
			contact->GetWorldManifold(&worldManifold);

			// Bush / Player To Sensor Action
			if ((a->GetBody() == m_Body || b->GetBody() == m_Body) && (a->GetBody()->GetFixtureList()->IsSensor() || b->GetBody()->GetFixtureList()->IsSensor()))
			{
				if (a->GetBody()->GetFixtureList()->GetFilterData().categoryBits == 6 || b->GetBody()->GetFixtureList()->GetFilterData().categoryBits == 6)
				{
					InterceptSceneChange(2);
					std::ofstream file;
					file.open("Resources/Output/PlayerData.ini");
					if (file.is_open())
					{
						file.clear();
						file << "x =" << m_Shape->getPosition().x << std::endl << "y =" << m_Shape->getPosition().y + 100;
					}
					file.close();
					m_bEnteredRoom = true;
					break;
				}
				if (a->GetBody()->GetFixtureList()->GetFilterData().categoryBits == 9 || b->GetBody()->GetFixtureList()->GetFilterData().categoryBits == 9
					&& m_WaterBoots)
				{
					std::cout << "Boots Trigger" << std::endl;
					m_WaterBoots = false;
					break;
				}
				else if (a->GetBody()->GetFixtureList()->GetFilterData().categoryBits == 8 || b->GetBody()->GetFixtureList()->GetFilterData().categoryBits == 8)
				{
					InterceptSceneChange(1);
					break;
				}
				else if ((a->GetBody()->GetFixtureList()->GetFilterData().categoryBits == 4 || b->GetBody()->GetFixtureList()->GetFilterData().categoryBits == 4) && m_HealTimer.getElapsedTime().asSeconds() >= 1.0f)
				{
					HealAllPokemon();
					m_HealTimer.restart();
					// Some Text Popups
					break;
				}
				else if ((a->GetBody()->GetFixtureList()->GetFilterData().categoryBits == 2 || b->GetBody()->GetFixtureList()->GetFilterData().categoryBits == 2) && m_EncounterClock.getElapsedTime().asSeconds() >= 2.5f)
				{
					std::cout << "AIDS" << std::endl;
					srand((unsigned)time(0));
					int bushEncounter = rand() % 4;

					if (bushEncounter == 0)
					{
						m_Encounter = true;
						break;
					}
					else if (bushEncounter == 3)
					{
						m_Encounter = true;
						break;
					}

				}
				else if ((a->GetBody()->GetFixtureList()->GetFilterData().categoryBits == 1 || b->GetBody()->GetFixtureList()->GetFilterData().categoryBits == 1) && m_EncounterClock.getElapsedTime().asSeconds() >= 2.5f
					&& !m_WaterBoots)
				{
					std::cout << "DUSTY AIDS" << std::endl;
					srand((unsigned)time(0));
					int bushEncounter = rand() % 4;

					if (bushEncounter == 0)
					{
						m_Encounter = true;
						break;
					}
					else if (bushEncounter == 3)
					{
						m_Encounter = true;
						break;
					}

				}
				else if ((a->GetBody()->GetFixtureList()->GetFilterData().categoryBits == 5 || b->GetBody()->GetFixtureList()->GetFilterData().categoryBits == 5)
					&& m_WaterBoots == false)
				{
					std::cout << "Water Boots!" << std::endl;
					// Old man
					m_WaterBoots = true;
					break;
				}
				else if ((a->GetBody()->GetFixtureList()->GetFilterData().categoryBits == 7 || b->GetBody()->GetFixtureList()->GetFilterData().categoryBits == 7))
				{
					// Boss
					InterceptSceneChange(-2);
					break;
				}

				if ((a->GetBody()->GetFixtureList()->GetFilterData().categoryBits == 3 || b->GetBody()->GetFixtureList()->GetFilterData().categoryBits == 3)
					&& m_WaterBoots)
				{
					// Water
					if (a->GetBody()->GetFixtureList()->GetFilterData().categoryBits == 3)
					{
						a->SetSensor(true);
					}
					else if (b->GetBody()->GetFixtureList()->GetFilterData().categoryBits == 3)
					{
						b->SetSensor(true);
					}
					break;
				}
				else if ((a->GetBody()->GetFixtureList()->GetFilterData().categoryBits == 3 || b->GetBody()->GetFixtureList()->GetFilterData().categoryBits == 3)
					&& !m_WaterBoots)
				{
					// Water
					if (a->GetBody()->GetFixtureList()->GetFilterData().categoryBits == 3)
					{
						a->SetSensor(false);
					}
					else if (b->GetBody()->GetFixtureList()->GetFilterData().categoryBits == 3)
					{
						b->SetSensor(false);
					}
					break;
				}
			}

			a = nullptr;
			b = nullptr;
		}

	}
	if (m_EncounterClock.getElapsedTime().asSeconds() >= 2.5f)
	{
		m_EncounterClock.restart();
	}
}
