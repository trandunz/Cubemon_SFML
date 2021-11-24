#include "GUI.h"

#include "CBattleScene.h"


GUI::GUI(sf::RenderWindow* _renderwindow, TextureMaster* _texturemaster, sf::Font& _font)
{
	m_RenderWindow = _renderwindow;
	m_TextureMaster = _texturemaster;
	m_Font = _font;
	m_Timer = sf::Clock();
	m_DisplayText = sf::Text();
	m_DisplayText.setFont(m_Font);
}

GUI::~GUI()
{
	CleanupBattleSceneButtons();
	m_RenderWindow = nullptr;
	m_TextureMaster = nullptr;
	m_FriendlyCubemon = nullptr;
	m_EnemyCubemon = nullptr;
}

void GUI::Start()
{
	InitHealthAndManaUI();
}

void GUI::Render(Player* _player, sf::Shader* _defaultshader, sf::Vector2f _mousePos)
{
	sf::Vector2f mousePos = m_RenderWindow->mapPixelToCoords(sf::Mouse::getPosition(*m_RenderWindow));
	
	m_RenderWindow->draw(m_HealthBorderSprite, _defaultshader);
	m_RenderWindow->draw(m_HealthSprite, _defaultshader);
	m_RenderWindow->draw(m_ManaBorderSprite, _defaultshader);
	m_RenderWindow->draw(m_ManaSprite, _defaultshader);
}

sf::Vector2f GUI::UIMousePointer(sf::View& _uiview)
{
	sf::Shader* defaultShader = NULL;
	sf::Vector2f mousePos = m_RenderWindow->mapPixelToCoords(sf::Mouse::getPosition(*m_RenderWindow), _uiview);
	return mousePos;
}

void GUI::InitHealthAndManaUI()
{
	m_ManaTexture.loadFromFile("Resources/Images/ManaBar.png");
	m_ManaBorderTexture.loadFromFile("Resources/Images/ManaBarBorder.png");
	m_ManaSprite.setTexture(m_ManaTexture, true);
	m_ManaBorderSprite.setTexture(m_ManaBorderTexture, true);
	m_ManaSprite.setOrigin(-3, -3);
	m_ManaSprite.scale(1.51, 1.5);
	m_ManaBorderSprite.scale(1.5, 1.5);

	m_HealthTexture.loadFromFile("Resources/Images/HealthBar.png");
	m_HealthBorderTexture.loadFromFile("Resources/Images/HealthBarBorder.png");
	m_EnemyHealthSprite.setTexture(m_HealthTexture, true);
	m_HealthSprite.setTexture(m_HealthTexture, true);
	m_EnemyHealthBorderSprite.setTexture(m_HealthBorderTexture, true);
	m_HealthBorderSprite.setTexture(m_HealthBorderTexture, true);
	m_HealthSprite.setOrigin(-3, -3);
	m_HealthSprite.scale(1.51, 1.5);
	m_HealthBorderSprite.scale(1.5, 1.5);

	m_EnemyHealthSprite.setOrigin(-3, -3);
	m_EnemyHealthSprite.scale(1.51, 1.5);
	m_EnemyHealthBorderSprite.scale(1.5, 1.5);
}

void GUI::HealthAndManaUI(sf::RenderWindow* _renderWindow, sf::View& _uiView, Player* _player)
{
	m_RenderWindow->mapCoordsToPixel(m_HealthSprite.getPosition());
	m_RenderWindow->mapCoordsToPixel(m_HealthBorderSprite.getPosition());
	m_HealthSprite.setPosition(m_RenderWindow->getView().getCenter().x + (m_RenderWindow->getView().getSize().x / 2) - 200, m_RenderWindow->getView().getCenter().y - (m_RenderWindow->getView().getSize().y / 2) + 80);
	m_HealthBorderSprite.setPosition(m_RenderWindow->getView().getCenter().x + (m_RenderWindow->getView().getSize().x / 2) - 200, m_RenderWindow->getView().getCenter().y - (m_RenderWindow->getView().getSize().y / 2) + 80);
	
	if (_player->GetCurrentHealth() > 0)
	{
		m_HealthSprite.setTextureRect(sf::IntRect(0, 0, (int) _player->GetCurrentHealth(), m_HealthTexture.getSize().y));
	}
	else
	{
		m_HealthSprite.setTextureRect(sf::IntRect(0, 0, 0, m_HealthTexture.getSize().y));
	}

	m_RenderWindow->mapCoordsToPixel(m_ManaSprite.getPosition());
	m_RenderWindow->mapCoordsToPixel(m_ManaBorderSprite.getPosition());
	m_ManaSprite.setPosition(m_RenderWindow->getView().getCenter().x + (m_RenderWindow->getView().getSize().x / 2) - 200, m_RenderWindow->getView().getCenter().y - (m_RenderWindow->getView().getSize().y / 2) + 100);
	m_ManaBorderSprite.setPosition(m_RenderWindow->getView().getCenter().x + (m_RenderWindow->getView().getSize().x / 2) - 200, m_RenderWindow->getView().getCenter().y - (m_RenderWindow->getView().getSize().y / 2) + 100);
	if (_player->GetCurrentMana() > 0)
	{
		m_ManaSprite.setTextureRect(sf::IntRect(0, 0, _player->GetCurrentMana(), m_ManaTexture.getSize().y));
	}
	else
	{
		m_ManaSprite.setTextureRect(sf::IntRect(0, 0, 0, m_ManaTexture.getSize().y));
	}
}

void GUI::TimerUI()
{
	std::string string = std::to_string((int)std::round(m_Timer.getElapsedTime().asSeconds()));
	m_DisplayText.setString(string);
	m_DisplayText.setFillColor(sf::Color::White);
	m_DisplayText.setOutlineThickness(0.75f);
	m_DisplayText.setOutlineColor(sf::Color::Black);
	m_DisplayText.setOrigin(m_DisplayText.getGlobalBounds().width / 2, m_DisplayText.getGlobalBounds().height / 2);
	m_DisplayText.setCharacterSize(50);
	m_RenderWindow->mapCoordsToPixel(m_DisplayText.getPosition());
	m_DisplayText.setPosition(m_RenderWindow->getView().getCenter().x + (m_RenderWindow->getView().getSize().x / 2) - 110, m_RenderWindow->getView().getCenter().y - (m_RenderWindow->getView().getSize().y / 2) + 15);
	m_RenderWindow->draw(m_DisplayText);
}

void GUI::InitBattleUI()
{
	InitCubeBoyUI();
}

void GUI::BattleUI(sf::View& _uiView, sf::View& _worldView)
{
	m_RenderWindow->setView(_uiView);

	if (m_PlayersTurn == true)
	{
		m_EnemyTurnTimer.restart();
	}
	else if (m_PlayersTurn == false)
	{
		HandleEnemyTurn();
	}

	for (auto& item : m_BattleSceneMenuShapes)
	{
		m_RenderWindow->draw(item);
	}

	if (m_bAttack)
	{
		for (auto& item : m_BattleSceneAttackShapes)
		{
			HandleGUIShapes(item);
		}
	}

	for (auto& item : m_BattleSceneButtons)
	{
		if (m_bFlee && (item->GetHoverText() == &m_FleeButton_Hover))
		{
			item->Update();
			m_RenderWindow->draw(item->Sprite);
		}
		else if (item->GetHoverText() != &m_FleeButton_Hover)
		{
			item->Update();
			m_RenderWindow->draw(item->Sprite);
		}

	}

	if (m_bAttack)
	{
		for (auto& item : m_BattleSceneAttackButtons)
		{
			item->Update();
			HandleGUIButtons(item);
		}
	}

	HandleButtonInteractions();

	m_RenderWindow->setView(_worldView);
}

void GUI::InitButton(CButtons* _button, sf::Texture* _idleTexture, sf::Texture* _hoverTexture)
{
	_button->SetClickTex(_hoverTexture);
	_button->SetHoverTex(_hoverTexture);
	_button->SetIdleTex(_idleTexture);
}

ICubemon::CUBEMONTYPE GUI::GetCurrentCubemon()
{
	return m_CurrentCubemon;
}

void GUI::CleanupBattleSceneButtons()
{
	for (auto& item : m_BattleSceneButtons)
	{
		DeletePointer(item);
		item = nullptr;
	}
	m_BattleSceneButtons.erase(std::remove(m_BattleSceneButtons.begin(), m_BattleSceneButtons.end(), nullptr), m_BattleSceneButtons.end());

	CleanupBattleSceneAttackButtons();
}

void GUI::CleanupBattleSceneAttackButtons()
{
	for (auto& item : m_BattleSceneAttackButtons)
	{
		DeletePointer(item);
		item = nullptr;
	}
	m_BattleSceneAttackButtons.erase(std::remove(m_BattleSceneAttackButtons.begin(), m_BattleSceneAttackButtons.end(), nullptr), m_BattleSceneAttackButtons.end());
}

void GUI::InitCubeBoyUI()
{
	m_CurrentlyHeldCubemons = Player::ReturnCubemonData();
	m_CurrentCubemon = m_CurrentlyHeldCubemons.front();
	InitTextures();
	InitCubeBoyMenuButtons();

	sf::RectangleShape CubeBoyBackgrounds;
	CubeBoyBackgrounds.setSize(sf::Vector2f(800, 300));
	CubeBoyBackgrounds.setFillColor(sf::Color(128,128,128));
	CubeBoyBackgrounds.setOrigin(400, 150);
	CubeBoyBackgrounds.setPosition(m_RenderWindow->getView().getCenter().x, m_RenderWindow->getView().getCenter().y + 370);
	m_BattleSceneMenuShapes.push_back(CubeBoyBackgrounds);

	CubeBoyBackgrounds.setSize(sf::Vector2f(470, 275));
	CubeBoyBackgrounds.setFillColor(sf::Color(100, 100, 100));
	CubeBoyBackgrounds.setOrigin(100, 275/2);
	CubeBoyBackgrounds.setPosition(m_RenderWindow->getView().getCenter().x, m_RenderWindow->getView().getCenter().y + 370);
	m_BattleSceneMenuShapes.push_back(CubeBoyBackgrounds);

	InitCubeBoyAttackUI();
}

void GUI::InitCubeBoyMenuButtons()
{
	m_BattleSceneButtons.push_back(new CButtons(m_RenderWindow));
	sf::Vector2f pos = sf::Vector2f(m_RenderWindow->getView().getCenter().x - m_BattleSceneButtons.back()->Sprite.getGlobalBounds().width * 5 - 120, m_RenderWindow->getView().getCenter().y + m_BattleSceneButtons.back()->Sprite.getGlobalBounds().width * 8.5);
	sf::Vector2f scale = sf::Vector2f(0.9f, 0.9f);
	InitButtonPosScaleTexture(pos, scale, &m_AttackButton, &m_AttackButton_Hover, m_BattleSceneButtons);

	m_BattleSceneButtons.push_back(new CButtons(m_RenderWindow));
	pos = sf::Vector2f(m_RenderWindow->getView().getCenter().x - m_BattleSceneButtons.back()->Sprite.getGlobalBounds().width * 5 - 120, m_RenderWindow->getView().getCenter().y + m_BattleSceneButtons.back()->Sprite.getGlobalBounds().width * 10.91666666666667);
	scale = sf::Vector2f(0.9f, 0.9f);
	InitButtonPosScaleTexture(pos, scale, &m_BackpackButton, &m_BackpackButton_Hover, m_BattleSceneButtons);

	m_BattleSceneButtons.push_back(new CButtons(m_RenderWindow));
	pos = sf::Vector2f(m_RenderWindow->getView().getCenter().x - m_BattleSceneButtons.back()->Sprite.getGlobalBounds().width * 5 - 120, m_RenderWindow->getView().getCenter().y + m_BattleSceneButtons.back()->Sprite.getGlobalBounds().width * 13.36066666666667);
	scale = sf::Vector2f(0.9f, 0.9f);
	InitButtonPosScaleTexture(pos, scale, &m_CubeboyButton, &m_CubeboyButton_Hover, m_BattleSceneButtons);

	m_BattleSceneButtons.push_back(new CButtons(m_RenderWindow));
	pos = sf::Vector2f(m_RenderWindow->getView().getCenter().x - m_BattleSceneButtons.back()->Sprite.getGlobalBounds().width * 5 - 120, m_RenderWindow->getView().getCenter().y + m_BattleSceneButtons.back()->Sprite.getGlobalBounds().width * 15.75);
	scale = sf::Vector2f(0.9f, 0.9f);
	InitButtonPosScaleTexture(pos, scale, &m_FleeButton, &m_FleeButton_Hover, m_BattleSceneButtons);
}

void GUI::InitCubeBoyAttackUI()
{
	sf::RectangleShape tempShape;
	sf::RectangleShape tempPortrate;

	// Buttons
	InitCubeBoyAttackButtons();

	// BG
	InitAttackUIBackgroundImages(tempShape);

	// Cubemons
	InitCubemonImages(tempPortrate);

	// Attacks
	InitKindlingAttackImages(tempShape);
	InitBrutusAttackImages(tempShape);
	InitWirlsonAttackImages(tempShape);
	InitBlizzardBird(tempShape);
}

void GUI::InitCubeBoyAttackButtons()
{
	CreateAttackButtons(m_AttackFire, m_AttackFire_Hover);
	CreateAttackButtons(m_AttackEarth, m_AttackEarth_Hover);
	CreateAttackButtons(m_AttackWater, m_AttackWater_Hover);
	CreateAttackButtons(m_AttackAir, m_AttackAir_Hover);
}

void GUI::InitCubemonHUD(ICubemon* _friendly, ICubemon* _enemy)
{
	m_FriendlyCubemon = _friendly;
	m_EnemyCubemon = _enemy;

	InitHealthAndManaUI();

	m_EnemyHealthSprite.setPosition(sf::Vector2f(m_RenderWindow->getView().getCenter().x + m_RenderWindow->getView().getSize().x / 6, m_RenderWindow->getView().getCenter().y - m_RenderWindow->getView().getSize().y / 6));
	m_EnemyHealthBorderSprite.setPosition(sf::Vector2f(m_RenderWindow->getView().getCenter().x + m_RenderWindow->getView().getSize().x / 6, m_RenderWindow->getView().getCenter().y - m_RenderWindow->getView().getSize().y / 6));
	m_HealthSprite.setPosition(sf::Vector2f(m_RenderWindow->getView().getCenter().x - m_RenderWindow->getView().getSize().x / 3.5, m_RenderWindow->getView().getCenter().y + m_RenderWindow->getView().getSize().y / 7));
	m_HealthBorderSprite.setPosition(sf::Vector2f(m_RenderWindow->getView().getCenter().x - m_RenderWindow->getView().getSize().x / 3.5, m_RenderWindow->getView().getCenter().y + m_RenderWindow->getView().getSize().y / 7));

	m_FriendlyCubemonName.setFont(m_Font);
	m_FriendlyCubemonName.setCharacterSize(18);
	std::string nameLvl = m_FriendlyCubemon->GetName();
	nameLvl += "      Lvl: ";
	nameLvl += std::to_string(m_FriendlyCubemon->GetLvl());
	m_FriendlyCubemonName.setString(nameLvl);
	m_FriendlyCubemonName.setOrigin(m_FriendlyCubemonName.getGlobalBounds().width / 2, m_FriendlyCubemonName.getGlobalBounds().height / 2);
	m_FriendlyCubemonName.setPosition(m_RenderWindow->getView().getCenter().x - m_RenderWindow->getView().getSize().x / 3.75, m_RenderWindow->getView().getCenter().y + m_RenderWindow->getView().getSize().y / 14.25);
	m_FriendlyCubemonName.setOutlineColor(sf::Color::Black);
	m_FriendlyCubemonName.setOutlineThickness(2);

	m_EnemyCubemonName.setFont(m_Font);
	m_EnemyCubemonName.setCharacterSize(18);
	nameLvl = m_EnemyCubemon->GetName();
	nameLvl += "      Lvl: ";
	nameLvl += std::to_string(m_EnemyCubemon->GetLvl());
	m_EnemyCubemonName.setString(nameLvl);
	m_EnemyCubemonName.setOrigin(m_EnemyCubemonName.getGlobalBounds().width / 2, m_EnemyCubemonName.getGlobalBounds().height / 2);
	m_EnemyCubemonName.setPosition(m_RenderWindow->getView().getCenter().x + m_RenderWindow->getView().getSize().x / 5.55, m_RenderWindow->getView().getCenter().y - m_RenderWindow->getView().getSize().y / 4.225);
	m_EnemyCubemonName.setOutlineColor(sf::Color::Black);
	m_EnemyCubemonName.setOutlineThickness(2);
}

void GUI::InitTextures()
{
	LoadTexture(&m_AttackButton, "GUI/Attack.png", false);
	LoadTexture(&m_BackpackButton, "GUI/Backpack.png", false);
	LoadTexture(&m_CubeboyButton, "GUI/CubeBoy.png", false);
	LoadTexture(&m_FleeButton, "GUI/Flee.png", false);

	LoadTexture(&m_AttackButton_Hover, "GUI/Attack_Hover.png", false);
	LoadTexture(&m_BackpackButton_Hover, "GUI/Backpack_Hover.png", false);
	LoadTexture(&m_CubeboyButton_Hover, "GUI/CubeBoy_Hover.png", false);
	LoadTexture(&m_FleeButton_Hover, "GUI/Flee_Hover.png", false);

	LoadTexture(&m_FireElement, "GUI/Fire.png", false);
	LoadTexture(&m_WaterElement, "GUI/Water.png", false);
	LoadTexture(&m_EarthElement, "GUI/Earth.png", false);
	LoadTexture(&m_AirElement, "GUI/Air.png", false);

	LoadTexture(&m_AttackFire, "GUI/FireAttack.png", false);
	LoadTexture(&m_AttackWater, "GUI/WaterAttack.png", false);
	LoadTexture(&m_AttackEarth, "GUI/EarthAttack.png", false);
	LoadTexture(&m_AttackAir, "GUI/AirAttack.png", false);

	LoadTexture(&m_AttackFire_Hover, "GUI/FireAttack_Hover.png", false);
	LoadTexture(&m_AttackWater_Hover, "GUI/WaterAttack_Hover.png", false);
	LoadTexture(&m_AttackEarth_Hover, "GUI/EarthAttack_Hover.png", false);
	LoadTexture(&m_AttackAir_Hover, "GUI/AirAttack_Hover.png", false);

	LoadTexture(&m_FireDeminishAttack, "GUI/Attacks/FireDeminish2.png", false);
	LoadTexture(&m_FireBurnAttack, "GUI/Attacks/FireBurn.png", false);
	LoadTexture(&m_FireEmberAttack, "GUI/Attacks/FireEmber.png", false);
	LoadTexture(&m_FireIncinerateAttack, "GUI/Attacks/FireIncinerate.png", false);

	LoadTexture(&m_KindlingCubemon, "Cubemon/Fire_Elemental.png", false);
	LoadTexture(&m_BrutusCubemon, "Cubemon/Plant_Elemental.png", false);
	LoadTexture(&m_ThallicCubemon, "Cubemon/Thallic.png", false);
	LoadTexture(&m_WirlsonCubemon, "Cubemon/Water_Elemental.png", false);
	LoadTexture(&m_DustDevilCubemon, "Cubemon/Dust_Devil.png", false);
	LoadTexture(&m_BlizzardBirdCubemon, "Cubemon/Blizzard_Bird.png", false);

	LoadTexture(&m_AirFFKAttack, "GUI/Attacks/AirFFK.png", false);
	LoadTexture(&m_AirHWAttack, "GUI/Attacks/AirHW.png", false);
	LoadTexture(&m_AirLevitateAttack, "GUI/Attacks/AirLevitate.png", false);
	LoadTexture(&m_AirSBAttack, "GUI/Attacks/AirSB.png", false);

	LoadTexture(&m_EarthFOTFAttack, "GUI/Attacks/EarthFOTF.png", false);
	LoadTexture(&m_EarthHardenAttack, "GUI/Attacks/EarthHardenr.png", false);
	LoadTexture(&m_EarthOvergrowthAttack, "GUI/Attacks/EarthOvergrowth.png", false);
	LoadTexture(&m_EarthShootsAttack, "GUI/Attacks/EarthShoots.png", false);

	LoadTexture(&m_WaterJetAttack, "GUI/Attacks/WaterJet.png", false);
	LoadTexture(&m_WaterSplashAttack, "GUI/Attacks/WaterSplash.png", false);
	LoadTexture(&m_WaterTsunamiAttack, "GUI/Attacks/WaterTsunami.png", false);
	LoadTexture(&m_WaterWTAttack, "GUI/Attacks/WaterWT.png", false);
}

void GUI::SetAllButtonScaling(float _newScale)
{
	for (auto& button : m_BattleSceneButtons)
	{
		button->Sprite.setScale(_newScale, _newScale);
		button->Sprite.setOrigin(button->Sprite.getGlobalBounds().height / 2, button->Sprite.getGlobalBounds().width / 2);
	}
}

void GUI::InitButtonPosScaleTexture(sf::Vector2f _position, sf::Vector2f _scale, sf::Texture* _idleTexture, sf::Texture* _hoverTexture, std::vector<CButtons*> _vector)
{
	_vector.back()->SetPosition(_position.x, _position.y);
	_vector.back()->SetIdleTex(_idleTexture);
	_vector.back()->SetHoverTex(_hoverTexture);
	_vector.back()->SetClickTex(_hoverTexture);
	_vector.back()->Sprite.setScale(_scale.x, _scale.y);
}

void GUI::HandleGUIShapes(sf::RectangleShape& _item)
{
	if (m_CurrentCubemon == ICubemon::CUBEMONTYPE::KINDLING)
	{
		if (_item.getTexture() == &m_FireDeminishAttack)
		{
			if (m_BattleSceneAttackButtons[0]->m_bIsHovering && m_PlayersTurn)
			{
				m_RenderWindow->draw(_item);
			}
		}
		else if (_item.getTexture() == &m_FireBurnAttack)
		{
			if (m_BattleSceneAttackButtons[1]->m_bIsHovering && m_PlayersTurn)
			{
				m_RenderWindow->draw(_item);
			}
		}
		else if (_item.getTexture() == &m_FireEmberAttack)
		{
			if (m_BattleSceneAttackButtons[2]->m_bIsHovering && m_PlayersTurn)
			{
				m_RenderWindow->draw(_item);
			}
		}
		else if (_item.getTexture() == &m_FireIncinerateAttack)
		{
			if (m_BattleSceneAttackButtons[3]->m_bIsHovering && m_PlayersTurn)
			{
				m_RenderWindow->draw(_item);
			}
		}
		else if (_item.getTexture() == &m_KindlingCubemon)
		{
			m_RenderWindow->draw(_item);
		}
	}
	else if (m_CurrentCubemon == ICubemon::CUBEMONTYPE::THALLIC)
	{
		if (_item.getTexture() == &m_EarthFOTFAttack)
		{
			if (m_BattleSceneAttackButtons[4]->m_bIsHovering && m_PlayersTurn)
			{
				m_RenderWindow->draw(_item);
			}
		}
		else if (_item.getTexture() == &m_EarthHardenAttack)
		{
			if (m_BattleSceneAttackButtons[5]->m_bIsHovering && m_PlayersTurn)
			{
				m_RenderWindow->draw(_item);
			}
		}
		else if (_item.getTexture() == &m_EarthOvergrowthAttack)
		{
			if (m_BattleSceneAttackButtons[6]->m_bIsHovering && m_PlayersTurn)
			{
				m_RenderWindow->draw(_item);
			}
		}
		else if (_item.getTexture() == &m_EarthShootsAttack)
		{
			if (m_BattleSceneAttackButtons[7]->m_bIsHovering && m_PlayersTurn)
			{
				m_RenderWindow->draw(_item);
			}
		}
		else if (_item.getTexture() == &m_ThallicCubemon)
		{
			m_RenderWindow->draw(_item);
		}
	}
	else if (m_CurrentCubemon == ICubemon::CUBEMONTYPE::BRUTUS)
	{
		if (_item.getTexture() == &m_EarthFOTFAttack)
		{
			if (m_BattleSceneAttackButtons[4]->m_bIsHovering && m_PlayersTurn)
			{
				m_RenderWindow->draw(_item);
			}
		}
		else if (_item.getTexture() == &m_EarthHardenAttack)
		{
			if (m_BattleSceneAttackButtons[5]->m_bIsHovering && m_PlayersTurn)
			{
				m_RenderWindow->draw(_item);
			}
		}
		else if (_item.getTexture() == &m_EarthOvergrowthAttack)
		{
			if (m_BattleSceneAttackButtons[6]->m_bIsHovering && m_PlayersTurn)
			{
				m_RenderWindow->draw(_item);
			}
		}
		else if (_item.getTexture() == &m_EarthShootsAttack)
		{
			if (m_BattleSceneAttackButtons[7]->m_bIsHovering && m_PlayersTurn)
			{
				m_RenderWindow->draw(_item);
			}
		}
		else if (_item.getTexture() == &m_BrutusCubemon)
		{
			m_RenderWindow->draw(_item);
		}
	}
	else if (m_CurrentCubemon == ICubemon::CUBEMONTYPE::WIRLSON)
	{
	if (_item.getTexture() == &m_WaterJetAttack)
	{
		if (m_BattleSceneAttackButtons[8]->m_bIsHovering && m_PlayersTurn)
		{
			m_RenderWindow->draw(_item);
		}
	}
	else if (_item.getTexture() == &m_WaterSplashAttack)
	{
		if (m_BattleSceneAttackButtons[9]->m_bIsHovering && m_PlayersTurn)
		{
			m_RenderWindow->draw(_item);
		}
	}
	else if (_item.getTexture() == &m_WaterTsunamiAttack)
	{
		if (m_BattleSceneAttackButtons[10]->m_bIsHovering && m_PlayersTurn)
		{
			m_RenderWindow->draw(_item);
		}
	}
	else if (_item.getTexture() == &m_WaterWTAttack)
	{
		if (m_BattleSceneAttackButtons[11]->m_bIsHovering && m_PlayersTurn)
		{
			m_RenderWindow->draw(_item);
		}
	}
	else if (_item.getTexture() == &m_WirlsonCubemon)
	{
		m_RenderWindow->draw(_item);
	}
	}
	else if (m_CurrentCubemon == ICubemon::CUBEMONTYPE::DUSTDEVIL)
	{
	if (_item.getTexture() == &m_AirFFKAttack)
	{
		if (m_BattleSceneAttackButtons[12]->m_bIsHovering && m_PlayersTurn)
		{
			m_RenderWindow->draw(_item);
		}
	}
	else if (_item.getTexture() == &m_AirHWAttack)
	{
		if (m_BattleSceneAttackButtons[13]->m_bIsHovering && m_PlayersTurn)
		{
			m_RenderWindow->draw(_item);
		}
	}
	else if (_item.getTexture() == &m_AirLevitateAttack)
	{
		if (m_BattleSceneAttackButtons[14]->m_bIsHovering && m_PlayersTurn)
		{
			m_RenderWindow->draw(_item);
		}
	}
	else if (_item.getTexture() == &m_AirSBAttack)
	{
		if (m_BattleSceneAttackButtons[15]->m_bIsHovering && m_PlayersTurn)
		{
			m_RenderWindow->draw(_item);
		}
	}
	else if (_item.getTexture() == &m_DustDevilCubemon)
	{
		m_RenderWindow->draw(_item);
	}
	}
	else if (m_CurrentCubemon == ICubemon::CUBEMONTYPE::BLIZZARDBIRD)
	{
	if (_item.getTexture() == &m_AirFFKAttack)
	{
		if (m_BattleSceneAttackButtons[12]->m_bIsHovering && m_PlayersTurn)
		{
			m_RenderWindow->draw(_item);
		}
	}
	else if (_item.getTexture() == &m_AirHWAttack)
	{
		if (m_BattleSceneAttackButtons[13]->m_bIsHovering && m_PlayersTurn)
		{
			m_RenderWindow->draw(_item);
		}
	}
	else if (_item.getTexture() == &m_AirLevitateAttack)
	{
		if (m_BattleSceneAttackButtons[14]->m_bIsHovering && m_PlayersTurn)
		{
			m_RenderWindow->draw(_item);
		}
	}
	else if (_item.getTexture() == &m_AirSBAttack)
	{
		if (m_BattleSceneAttackButtons[15]->m_bIsHovering && m_PlayersTurn)
		{
			m_RenderWindow->draw(_item);
		}
	}
	else if (_item.getTexture() == &m_DustDevilCubemon)
	{
		m_RenderWindow->draw(_item);
	}
	}
}

void GUI::HandleGUIButtons(CButtons* _button)
{
	if (m_CurrentCubemon == ICubemon::CUBEMONTYPE::KINDLING)
	{
		if (_button == m_BattleSceneAttackButtons[0] ||
			_button == m_BattleSceneAttackButtons[1] ||
			_button == m_BattleSceneAttackButtons[2] ||
			_button == m_BattleSceneAttackButtons[3]
			)
		{
			m_RenderWindow->draw(_button->Sprite);
		}
	}
	else if (m_CurrentCubemon == ICubemon::CUBEMONTYPE::BRUTUS || m_CurrentCubemon == ICubemon::CUBEMONTYPE::THALLIC)
	{
		if (_button == m_BattleSceneAttackButtons[4] ||
			_button == m_BattleSceneAttackButtons[5] ||
			_button == m_BattleSceneAttackButtons[6] ||
			_button == m_BattleSceneAttackButtons[7]
			)
		{
			m_RenderWindow->draw(_button->Sprite);
		}
	}
	else if (m_CurrentCubemon == ICubemon::CUBEMONTYPE::WIRLSON)
	{
		if (_button == m_BattleSceneAttackButtons[8] ||
			_button == m_BattleSceneAttackButtons[9] ||
			_button == m_BattleSceneAttackButtons[10] ||
			_button == m_BattleSceneAttackButtons[11]
			)
		{
			m_RenderWindow->draw(_button->Sprite);
		}
	}
	else if (m_CurrentCubemon == ICubemon::CUBEMONTYPE::DUSTDEVIL || m_CurrentCubemon == ICubemon::CUBEMONTYPE::BLIZZARDBIRD)
	{
		if (_button == m_BattleSceneAttackButtons[12] ||
			_button == m_BattleSceneAttackButtons[13] ||
			_button == m_BattleSceneAttackButtons[14] ||
			_button == m_BattleSceneAttackButtons[15]
			)
		{
			m_RenderWindow->draw(_button->Sprite);
		}
	}
}

void GUI::HandleButtonInteractions()
{
	if (m_PlayersTurn && m_ButtonRegulator.getElapsedTime().asSeconds() >= 0.25f)
	{
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			HandleMenuButtons();
			if (!m_bChangePokemon)
			{
				HandleAttackButtons();
			}
		}
	}
}

void GUI::HandleEnemyTurn()
{
	if (m_EnemyTurnTimer.getElapsedTime().asSeconds() >= 1)
	{
		if (m_EnemyAttackTimer.getElapsedTime().asSeconds() >= 1.1)
		{
			int ran = INT_MAX;
			srand((unsigned)time(NULL));
			ran = rand() % 2;
			if (ran == 0)
			{
				m_FriendlyCubemon->TakeDamage(m_EnemyCubemon->GetStrongAttack() * m_EnemyCubemon->GetLvl());
			}
			else if (ran == 1)
			{
				m_FriendlyCubemon->TakeDamage(m_EnemyCubemon->GetWeakAttack() * m_EnemyCubemon->GetLvl());
			}
			
			m_EnemyAttackTimer.restart();
		}

		if (m_EnemyTurnTimer.getElapsedTime().asSeconds() >= 2)
		{
			m_PlayersTurn = true;
		}
	}
}

bool GUI::HandleMenuButtons()
{
	if (m_BattleSceneButtons[0]->m_bIsHovering && !m_bChangePokemon)
	{
		m_bAttack = !m_bAttack;
		m_ButtonRegulator.restart();
		return true;
	}
	else if (m_BattleSceneButtons[3]->m_bIsHovering && m_bFlee)
	{
		int ran = INT_MAX;
		srand((unsigned)time(NULL));
		ran = rand() % 2;
		if (ran == 0)
		{
		}
		else if (ran == 1)
		{
			InterceptSceneChange(1);
		}
		m_bFlee = false;
		m_BattleSceneButtons[3]->m_bIsHovering = false;

		EndTurn();
		return true;
	}
	else
	{
		return false;
	}
}

void GUI::CreateAttackButtons(sf::Texture& _idleTexture, sf::Texture& _hoverTexture)
{
	m_BattleSceneAttackButtons.push_back(new CButtons(m_RenderWindow));
	sf::Vector2f pos = sf::Vector2f(m_RenderWindow->getView().getCenter().x - m_BattleSceneAttackButtons.back()->Sprite.getGlobalBounds().width * 5 + 85, m_RenderWindow->getView().getCenter().y + m_BattleSceneAttackButtons.back()->Sprite.getGlobalBounds().width * 12.25);
	sf::Vector2f scale = sf::Vector2f(0.4f, 0.4f);
	InitButtonPosScaleTexture(pos, scale, &_idleTexture, &_hoverTexture, m_BattleSceneAttackButtons);

	m_BattleSceneAttackButtons.push_back(new CButtons(m_RenderWindow));
	pos = sf::Vector2f(m_RenderWindow->getView().getCenter().x - m_BattleSceneAttackButtons.back()->Sprite.getGlobalBounds().width * 5 + 85, m_RenderWindow->getView().getCenter().y + m_BattleSceneAttackButtons.back()->Sprite.getGlobalBounds().width * 13.41);
	scale = sf::Vector2f(0.4f, 0.4f);
	InitButtonPosScaleTexture(pos, scale, &_idleTexture, &_hoverTexture, m_BattleSceneAttackButtons);

	m_BattleSceneAttackButtons.push_back(new CButtons(m_RenderWindow));
	pos = sf::Vector2f(m_RenderWindow->getView().getCenter().x - m_BattleSceneAttackButtons.back()->Sprite.getGlobalBounds().width * 5 + 85, m_RenderWindow->getView().getCenter().y + m_BattleSceneAttackButtons.back()->Sprite.getGlobalBounds().width * 14.58);
	scale = sf::Vector2f(0.4f, 0.4f);
	InitButtonPosScaleTexture(pos, scale, &_idleTexture, &_hoverTexture, m_BattleSceneAttackButtons);

	m_BattleSceneAttackButtons.push_back(new CButtons(m_RenderWindow));
	pos = sf::Vector2f(m_RenderWindow->getView().getCenter().x - m_BattleSceneAttackButtons.back()->Sprite.getGlobalBounds().width * 5 + 85, m_RenderWindow->getView().getCenter().y + m_BattleSceneAttackButtons.back()->Sprite.getGlobalBounds().width * 15.75);
	scale = sf::Vector2f(0.4f, 0.4f);
	InitButtonPosScaleTexture(pos, scale, &_idleTexture, &_hoverTexture, m_BattleSceneAttackButtons);
}

bool GUI::HandleAttackButtons()
{
	if (m_BattleSceneAttackButtons[0]->m_bIsHovering && !m_bChangePokemon)
	{
		switch (m_FriendlyCubemon->m_CubeType)
		{
		case ICubemon::CUBEMONTYPE::THALLIC:
		{
			m_PlayerAttackBuff = 15;
			m_PlayerArmourBuff = 0;
			break;
		}
		case ICubemon::CUBEMONTYPE::BRUTUS:
		{
			m_PlayerAttackBuff = 15;
			m_PlayerArmourBuff = 0;
			break;
		}
		case ICubemon::CUBEMONTYPE::KINDLING:
		{
			m_PlayerAttackBuff = 5;
			m_PlayerArmourBuff = 0;
		}
		case ICubemon::CUBEMONTYPE::WIRLSON:
		{
			m_PlayerAttackBuff = 5;
			m_PlayerArmourBuff = 0;
		}
		case ICubemon::CUBEMONTYPE::BLIZZARDBIRD:
		{
			m_PlayerAttackBuff = 5;
			m_PlayerArmourBuff = 0;
		}
		case ICubemon::CUBEMONTYPE::DUSTDEVIL:
		{
			m_PlayerAttackBuff = 5;
			m_PlayerArmourBuff = 0;
		}
		default:
			break;
		}

		EndTurn();
		return true;
	}
	else if (m_BattleSceneAttackButtons[1]->m_bIsHovering && !m_bChangePokemon)
	{
		switch (m_FriendlyCubemon->m_CubeType)
		{
		case ICubemon::CUBEMONTYPE::THALLIC:
		{
			m_PlayerArmourBuff = 10;
			break;
		}
		case ICubemon::CUBEMONTYPE::BRUTUS:
		{
			m_PlayerArmourBuff = 10;
			break;
		}
		case ICubemon::CUBEMONTYPE::KINDLING:
		{
			float damage = m_PlayerAttackBuff + (m_FriendlyCubemon->GetWeakAttack() * m_FriendlyCubemon->GetLvl());
			m_EnemyCubemon->TakeDamage(damage);
			m_FriendlyCubemon->GetAudioManager()->PlayFireAttack();

			if (m_EnemyCubemon->GetCurrentHealth() <= 0)
			{
				m_FriendlyCubemon->AddXP(50 * m_EnemyCubemon->GetLvl());
				InterceptSceneChange(1);
			}

			m_PlayerAttackBuff = 0;
			m_PlayerArmourBuff = 0;
			std::cout << damage << std::endl;
		}
		case ICubemon::CUBEMONTYPE::WIRLSON:
		{
			m_PlayerAttackBuff = 5;
			m_PlayerArmourBuff = 0;
		}
		case ICubemon::CUBEMONTYPE::BLIZZARDBIRD:
		{
			m_PlayerAttackBuff = 5;
			m_PlayerArmourBuff = 0;
		}
		case ICubemon::CUBEMONTYPE::DUSTDEVIL:
		{
			m_PlayerAttackBuff = 5;
			m_PlayerArmourBuff = 0;
		}
		default:
			break;
		}

		EndTurn();
		return true;
	}
	else if (m_BattleSceneAttackButtons[2]->m_bIsHovering && !m_bChangePokemon)
	{
		switch (m_FriendlyCubemon->m_CubeType)
		{
		case ICubemon::CUBEMONTYPE::THALLIC:
		{
			float damage = m_PlayerAttackBuff + (10 * m_FriendlyCubemon->GetLvl());
			m_EnemyCubemon->TakeDamage(damage);
			m_FriendlyCubemon->GetAudioManager()->PlayFireAttack();

			if (m_EnemyCubemon->GetCurrentHealth() <= 0)
			{
				m_FriendlyCubemon->AddXP(50 * m_EnemyCubemon->GetLvl());
				InterceptSceneChange(1);
			}

			m_PlayerAttackBuff = 0;
			m_PlayerArmourBuff = 5;
			std::cout << damage << std::endl;
			break;
		}
		case ICubemon::CUBEMONTYPE::BRUTUS:
		{
			float damage = m_PlayerAttackBuff + (10 * m_FriendlyCubemon->GetLvl());
			m_EnemyCubemon->TakeDamage(damage);
			m_FriendlyCubemon->GetAudioManager()->PlayFireAttack();

			if (m_EnemyCubemon->GetCurrentHealth() <= 0)
			{
				m_FriendlyCubemon->AddXP(50 * m_EnemyCubemon->GetLvl());
				InterceptSceneChange(1);
			}

			m_PlayerAttackBuff = 0;
			m_PlayerArmourBuff = 5;
			std::cout << damage << std::endl;
			break;
		}
		case ICubemon::CUBEMONTYPE::KINDLING:
		{
			m_PlayerAttackBuff = 15;
			m_PlayerArmourBuff = 0;
		}
		case ICubemon::CUBEMONTYPE::WIRLSON:
		{
			m_PlayerAttackBuff = 5;
			m_PlayerArmourBuff = 0;
		}
		case ICubemon::CUBEMONTYPE::BLIZZARDBIRD:
		{
			m_PlayerAttackBuff = 5;
			m_PlayerArmourBuff = 0;
		}
		case ICubemon::CUBEMONTYPE::DUSTDEVIL:
		{
			m_PlayerAttackBuff = 5;
			m_PlayerArmourBuff = 0;
		}
		default:
			break;
		}

		EndTurn();
		return true;
	}
	else if (m_BattleSceneAttackButtons[3]->m_bIsHovering && !m_bChangePokemon)
	{
		switch (m_FriendlyCubemon->m_CubeType)
		{
		case ICubemon::CUBEMONTYPE::THALLIC:
		{
			float damage = m_PlayerAttackBuff + (5 * m_FriendlyCubemon->GetLvl());
			m_EnemyCubemon->TakeDamage(damage);
			m_FriendlyCubemon->GetAudioManager()->PlayFireAttack();

			if (m_EnemyCubemon->GetCurrentHealth() <= 0)
			{
				m_FriendlyCubemon->AddXP(50 * m_EnemyCubemon->GetLvl());
				InterceptSceneChange(1);
			}

			m_PlayerAttackBuff = 0;
			m_PlayerArmourBuff = 0;
			std::cout << damage << std::endl;
			break;
		}
		case ICubemon::CUBEMONTYPE::BRUTUS:
		{
			float damage = m_PlayerAttackBuff + (5 * m_FriendlyCubemon->GetLvl());
			m_EnemyCubemon->TakeDamage(damage);
			m_FriendlyCubemon->GetAudioManager()->PlayFireAttack();

			if (m_EnemyCubemon->GetCurrentHealth() <= 0)
			{
				m_FriendlyCubemon->AddXP(50 * m_EnemyCubemon->GetLvl());
				InterceptSceneChange(1);
			}

			m_PlayerAttackBuff = 0;
			m_PlayerArmourBuff = 0;
			std::cout << damage << std::endl;
			break;
		}
		case ICubemon::CUBEMONTYPE::KINDLING:
		{
			float damage = m_PlayerAttackBuff + (m_FriendlyCubemon->GetStrongAttack() * m_FriendlyCubemon->GetLvl());
			m_EnemyCubemon->TakeDamage(damage);
			m_FriendlyCubemon->GetAudioManager()->PlayFireAttack();

			if (m_EnemyCubemon->GetCurrentHealth() <= 0)
			{
				m_FriendlyCubemon->AddXP(50 * m_EnemyCubemon->GetLvl());
				InterceptSceneChange(1);
			}

			m_PlayerAttackBuff = 0;
			m_PlayerArmourBuff = 0;
			std::cout << damage << std::endl;
			break;
		}
		case ICubemon::CUBEMONTYPE::WIRLSON:
		{
			m_PlayerAttackBuff = 5;
			m_PlayerArmourBuff = 0;
		}
		case ICubemon::CUBEMONTYPE::BLIZZARDBIRD:
		{
			m_PlayerAttackBuff = 5;
			m_PlayerArmourBuff = 0;
		}
		case ICubemon::CUBEMONTYPE::DUSTDEVIL:
		{
			m_PlayerAttackBuff = 5;
			m_PlayerArmourBuff = 0;
		}
		default:
			break;
		}

		EndTurn();
		return true;
	}
	else
	{
		return false;
	}
}

void GUI::InitCubemonImages(sf::RectangleShape& _tempShape)
{
	_tempShape.setSize(sf::Vector2f(100, 100));
	_tempShape.setFillColor(sf::Color::White);
	_tempShape.setOrigin(50, 50);
	_tempShape.setPosition(m_RenderWindow->getView().getCenter().x - 30, m_RenderWindow->getView().getCenter().y + 300);
	_tempShape.setTexture(&m_KindlingCubemon);
	m_BattleSceneAttackShapes.push_back(_tempShape);

	_tempShape.setSize(sf::Vector2f(100, 100));
	_tempShape.setFillColor(sf::Color::White);
	_tempShape.setOrigin(50, 50);
	_tempShape.setPosition(m_RenderWindow->getView().getCenter().x - 30, m_RenderWindow->getView().getCenter().y + 300);
	_tempShape.setTexture(&m_BrutusCubemon);
	m_BattleSceneAttackShapes.push_back(_tempShape);

	_tempShape.setSize(sf::Vector2f(100, 100));
	_tempShape.setFillColor(sf::Color::White);
	_tempShape.setOrigin(50, 50);
	_tempShape.setPosition(m_RenderWindow->getView().getCenter().x - 30, m_RenderWindow->getView().getCenter().y + 300);
	_tempShape.setTexture(&m_ThallicCubemon, true);
	_tempShape.setScale(1.f, 1.f);
	m_BattleSceneAttackShapes.push_back(_tempShape);

	_tempShape.setSize(sf::Vector2f(100, 100));
	_tempShape.setFillColor(sf::Color::White);
	_tempShape.setOrigin(50, 50);
	_tempShape.setPosition(m_RenderWindow->getView().getCenter().x - 30, m_RenderWindow->getView().getCenter().y + 300);
	_tempShape.setTexture(&m_BlizzardBirdCubemon, true);
	_tempShape.setScale(1.f, 1.f);
	m_BattleSceneAttackShapes.push_back(_tempShape);

	_tempShape.setSize(sf::Vector2f(100, 100));
	_tempShape.setFillColor(sf::Color::White);
	_tempShape.setOrigin(50, 50);
	_tempShape.setPosition(m_RenderWindow->getView().getCenter().x - 30, m_RenderWindow->getView().getCenter().y + 300);
	_tempShape.setTexture(&m_DustDevilCubemon, true);
	_tempShape.setScale(1.f, 1.f);
	m_BattleSceneAttackShapes.push_back(_tempShape);

	_tempShape.setSize(sf::Vector2f(100, 100));
	_tempShape.setFillColor(sf::Color::White);
	_tempShape.setOrigin(50, 50);
	_tempShape.setPosition(m_RenderWindow->getView().getCenter().x - 30, m_RenderWindow->getView().getCenter().y + 300);
	_tempShape.setTexture(&m_WirlsonCubemon, true);
	_tempShape.setScale(1.f, 1.f);
	m_BattleSceneAttackShapes.push_back(_tempShape);
}

void GUI::InitKindlingAttackImages(sf::RectangleShape& _tempShape)
{
	_tempShape.setSize(sf::Vector2f(315, 255));
	_tempShape.setFillColor(sf::Color::White);
	_tempShape.setOrigin(200, 255 / 2);
	_tempShape.setPosition(m_RenderWindow->getView().getCenter().x + 240, m_RenderWindow->getView().getCenter().y + 365);
	_tempShape.setTexture(&m_FireDeminishAttack, true);
	_tempShape.setScale(1, 1);
	m_BattleSceneAttackShapes.push_back(_tempShape);

	_tempShape.setSize(sf::Vector2f(315, 255));
	_tempShape.setFillColor(sf::Color::White);
	_tempShape.setOrigin(200, 255 / 2);
	_tempShape.setPosition(m_RenderWindow->getView().getCenter().x + 240, m_RenderWindow->getView().getCenter().y + 365);
	_tempShape.setTexture(&m_FireBurnAttack, true);
	_tempShape.setScale(1, 1);
	m_BattleSceneAttackShapes.push_back(_tempShape);

	_tempShape.setSize(sf::Vector2f(315, 255));
	_tempShape.setFillColor(sf::Color::White);
	_tempShape.setOrigin(200, 255 / 2);
	_tempShape.setPosition(m_RenderWindow->getView().getCenter().x + 240, m_RenderWindow->getView().getCenter().y + 365);
	_tempShape.setTexture(&m_FireEmberAttack, true);
	_tempShape.setScale(1, 1);
	m_BattleSceneAttackShapes.push_back(_tempShape);

	_tempShape.setSize(sf::Vector2f(315, 255));
	_tempShape.setFillColor(sf::Color::White);
	_tempShape.setOrigin(200, 255 / 2);
	_tempShape.setPosition(m_RenderWindow->getView().getCenter().x + 240, m_RenderWindow->getView().getCenter().y + 365);
	_tempShape.setTexture(&m_FireIncinerateAttack, true);
	_tempShape.setScale(1, 1);
	m_BattleSceneAttackShapes.push_back(_tempShape);
}

void GUI::InitBrutusAttackImages(sf::RectangleShape& _tempShape)
{
	_tempShape.setSize(sf::Vector2f(315, 255));
	_tempShape.setFillColor(sf::Color::White);
	_tempShape.setOrigin(200, 255 / 2);
	_tempShape.setPosition(m_RenderWindow->getView().getCenter().x + 240, m_RenderWindow->getView().getCenter().y + 365);
	_tempShape.setTexture(&m_EarthFOTFAttack, true);
	_tempShape.setScale(1, 1);
	m_BattleSceneAttackShapes.push_back(_tempShape);

	_tempShape.setSize(sf::Vector2f(315, 255));
	_tempShape.setFillColor(sf::Color::White);
	_tempShape.setOrigin(200, 255 / 2);
	_tempShape.setPosition(m_RenderWindow->getView().getCenter().x + 240, m_RenderWindow->getView().getCenter().y + 365);
	_tempShape.setTexture(&m_EarthHardenAttack, true);
	_tempShape.setScale(1, 1);
	m_BattleSceneAttackShapes.push_back(_tempShape);

	_tempShape.setSize(sf::Vector2f(315, 255));
	_tempShape.setFillColor(sf::Color::White);
	_tempShape.setOrigin(200, 255 / 2);
	_tempShape.setPosition(m_RenderWindow->getView().getCenter().x + 240, m_RenderWindow->getView().getCenter().y + 365);
	_tempShape.setTexture(&m_EarthOvergrowthAttack, true);
	_tempShape.setScale(1, 1);
	m_BattleSceneAttackShapes.push_back(_tempShape);

	_tempShape.setSize(sf::Vector2f(315, 255));
	_tempShape.setFillColor(sf::Color::White);
	_tempShape.setOrigin(200, 255 / 2);
	_tempShape.setPosition(m_RenderWindow->getView().getCenter().x + 240, m_RenderWindow->getView().getCenter().y + 365);
	_tempShape.setTexture(&m_EarthShootsAttack, true);
	_tempShape.setScale(1, 1);
	m_BattleSceneAttackShapes.push_back(_tempShape);
}

void GUI::InitAttackUIBackgroundImages(sf::RectangleShape& _tempShape)
{
	_tempShape.setSize(sf::Vector2f(315, 255));
	_tempShape.setFillColor(sf::Color(90, 90, 90));
	_tempShape.setOrigin(200, 255 / 2);
	_tempShape.setPosition(m_RenderWindow->getView().getCenter().x + 240, m_RenderWindow->getView().getCenter().y + 370);
	m_BattleSceneAttackShapes.push_back(_tempShape);

	_tempShape.setSize(sf::Vector2f(110, 110));
	_tempShape.setFillColor(sf::Color(139, 69, 19));
	_tempShape.setOrigin(55, 55);
	_tempShape.setPosition(m_RenderWindow->getView().getCenter().x - 30, m_RenderWindow->getView().getCenter().y + 300);
	m_BattleSceneAttackShapes.push_back(_tempShape);
}

void GUI::InitWirlsonAttackImages(sf::RectangleShape& _tempShape)
{
	_tempShape.setSize(sf::Vector2f(315, 255));
	_tempShape.setFillColor(sf::Color::White);
	_tempShape.setOrigin(200, 255 / 2);
	_tempShape.setPosition(m_RenderWindow->getView().getCenter().x + 240, m_RenderWindow->getView().getCenter().y + 365);
	_tempShape.setTexture(&m_WaterJetAttack, true);
	_tempShape.setScale(1, 1);
	m_BattleSceneAttackShapes.push_back(_tempShape);

	_tempShape.setSize(sf::Vector2f(315, 255));
	_tempShape.setFillColor(sf::Color::White);
	_tempShape.setOrigin(200, 255 / 2);
	_tempShape.setPosition(m_RenderWindow->getView().getCenter().x + 240, m_RenderWindow->getView().getCenter().y + 365);
	_tempShape.setTexture(&m_WaterSplashAttack, true);
	_tempShape.setScale(1, 1);
	m_BattleSceneAttackShapes.push_back(_tempShape);

	_tempShape.setSize(sf::Vector2f(315, 255));
	_tempShape.setFillColor(sf::Color::White);
	_tempShape.setOrigin(200, 255 / 2);
	_tempShape.setPosition(m_RenderWindow->getView().getCenter().x + 240, m_RenderWindow->getView().getCenter().y + 365);
	_tempShape.setTexture(&m_WaterTsunamiAttack, true);
	_tempShape.setScale(1, 1);
	m_BattleSceneAttackShapes.push_back(_tempShape);

	_tempShape.setSize(sf::Vector2f(315, 255));
	_tempShape.setFillColor(sf::Color::White);
	_tempShape.setOrigin(200, 255 / 2);
	_tempShape.setPosition(m_RenderWindow->getView().getCenter().x + 240, m_RenderWindow->getView().getCenter().y + 365);
	_tempShape.setTexture(&m_WaterWTAttack, true);
	_tempShape.setScale(1, 1);
	m_BattleSceneAttackShapes.push_back(_tempShape);
}

void GUI::InitBlizzardBird(sf::RectangleShape& _tempShape)
{
	_tempShape.setSize(sf::Vector2f(315, 255));
	_tempShape.setFillColor(sf::Color::White);
	_tempShape.setOrigin(200, 255 / 2);
	_tempShape.setPosition(m_RenderWindow->getView().getCenter().x + 240, m_RenderWindow->getView().getCenter().y + 365);
	_tempShape.setTexture(&m_AirFFKAttack, true);
	_tempShape.setScale(1, 1);
	m_BattleSceneAttackShapes.push_back(_tempShape);

	_tempShape.setSize(sf::Vector2f(315, 255));
	_tempShape.setFillColor(sf::Color::White);
	_tempShape.setOrigin(200, 255 / 2);
	_tempShape.setPosition(m_RenderWindow->getView().getCenter().x + 240, m_RenderWindow->getView().getCenter().y + 365);
	_tempShape.setTexture(&m_AirHWAttack, true);
	_tempShape.setScale(1, 1);
	m_BattleSceneAttackShapes.push_back(_tempShape);

	_tempShape.setSize(sf::Vector2f(315, 255));
	_tempShape.setFillColor(sf::Color::White);
	_tempShape.setOrigin(200, 255 / 2);
	_tempShape.setPosition(m_RenderWindow->getView().getCenter().x + 240, m_RenderWindow->getView().getCenter().y + 365);
	_tempShape.setTexture(&m_AirLevitateAttack, true);
	_tempShape.setScale(1, 1);
	m_BattleSceneAttackShapes.push_back(_tempShape);

	_tempShape.setSize(sf::Vector2f(315, 255));
	_tempShape.setFillColor(sf::Color::White);
	_tempShape.setOrigin(200, 255 / 2);
	_tempShape.setPosition(m_RenderWindow->getView().getCenter().x + 240, m_RenderWindow->getView().getCenter().y + 365);
	_tempShape.setTexture(&m_AirSBAttack, true);
	_tempShape.setScale(1, 1);
	m_BattleSceneAttackShapes.push_back(_tempShape);
}

void GUI::HandleINISwaps(int _newType)
{
	std::ifstream file;
	std::vector<int> m_Types{};
	std::vector<int> m_Lvls{};
	std::vector<int> m_HPs{};
	std::vector<int> m_XPs{};
	char type = 0;
	char lvl = 0;
	file.open("Resources/Output/CubemonData.ini");
	if (file.is_open())
	{
		while (file.get(type))
		{
			if (type == ',') {}
			else
			{
				m_Types.push_back(((int)type) - ASCIIOFFSET);
			}
		}
	}
	file.close();

	std::string currentLine;
	file.open("Resources/Output/CubemonLvlData.ini");
	if (file.is_open())
	{
		while (std::getline(file, currentLine))
		{
			std::size_t pos = currentLine.find('=');
			std::string value = currentLine.substr(pos + 1);

			m_Lvls.push_back(std::stoi(value));
		}

	}
	file.close();
	for (int i = 0; i < m_Types.size(); i++)
	{
		if (m_Types[i] == (int)m_FriendlyCubemon->m_CubeType)
		{
			m_Lvls[i] = (int)m_FriendlyCubemon->GetLvl();
		}
	}

	file.open("Resources/Output/CubemonXPData.ini");
	if (file.is_open())
	{
		while (std::getline(file, currentLine))
		{
			std::size_t pos = currentLine.find('=');
			std::string value = currentLine.substr(pos + 1);

			m_XPs.push_back(std::stoi(value));
		}

	}
	file.close();
	for (int i = 0; i < m_Types.size(); i++)
	{
		if (m_Types[i] == (int)m_FriendlyCubemon->m_CubeType)
		{
			m_XPs[i] = (int)m_FriendlyCubemon->GetXP();
		}
	}

	file.open("Resources/Output/CubemonHPData.ini");
	if (file.is_open())
	{
		while (std::getline(file, currentLine))
		{
			std::size_t pos = currentLine.find('=');
			std::string value = currentLine.substr(pos + 1);

			m_HPs.push_back(std::stoi(value));
		}

	}
	file.close();
	for (int i = 0; i < m_HPs.size(); i++)
	{
		if (m_Types[i] == (int)m_FriendlyCubemon->m_CubeType)
		{
			m_HPs[i] = (int)m_FriendlyCubemon->GetCurrentHealth();
		}
	}

	// Change
	for (int i = 0; i < m_Types.size(); i++)
	{
		if (m_Types[i] == _newType)
		{
			std::swap(m_Types.front(), m_Types[i]);
			std::swap(m_XPs.front(), m_XPs[i]);
			std::swap(m_HPs.front(), m_HPs[i]);
			std::swap(m_Lvls.front(), m_Lvls[i]);
		}
	}

	// Push Values To File
	std::ofstream oFile;

	oFile.open("Resources/Output/CubemonData.ini");
	if (oFile.is_open())
	{
		oFile.clear();
		for (auto& cubemon : m_Types)
		{
			oFile << cubemon << ",";
		}
	}
	oFile.close();

	oFile.open("Resources/Output/CubemonLvlData.ini");
	if (oFile.is_open())
	{
		oFile.clear();
		for (int i = 0; i < m_Types.size(); i++)
		{
			oFile << m_Lvls[i] << std::endl;
		}
	}
	oFile.close();

	oFile.open("Resources/Output/CubemonHPData.ini");
	if (oFile.is_open())
	{
		oFile.clear();
		for (int i = 0; i < m_Types.size(); i++)
		{
			oFile << m_HPs[i] << std::endl;
		}
	}
	oFile.close();

	oFile.open("Resources/Output/CubemonXPData.ini");
	if (oFile.is_open())
	{
		oFile.clear();
		for (int i = 0; i < m_Types.size(); i++)
		{
			oFile << m_XPs[i] << std::endl;
		}
	}
	oFile.close();
}

int GUI::ReturnPokemonChangeType()
{
	return NULL;
}

void GUI::HandleCubemonHUD(sf::View& _uiView, sf::View& _worldView)
{
	m_RenderWindow->setView(_uiView);

	m_RenderWindow->mapCoordsToPixel(m_HealthSprite.getPosition());
	m_RenderWindow->mapCoordsToPixel(m_HealthBorderSprite.getPosition());
	m_HealthSprite.setPosition(m_RenderWindow->getView().getCenter().x - m_RenderWindow->getView().getSize().x / 3.25, m_RenderWindow->getView().getCenter().y + m_RenderWindow->getView().getSize().y / 12);
	m_HealthBorderSprite.setPosition(m_RenderWindow->getView().getCenter().x - m_RenderWindow->getView().getSize().x / 3.25, m_RenderWindow->getView().getCenter().y + m_RenderWindow->getView().getSize().y / 12);
	if (m_FriendlyCubemon->GetCurrentHealth() > 0)
	{
		m_HealthSprite.setTextureRect(sf::IntRect(0, 0, (int)m_FriendlyCubemon->GetCurrentHealth(), m_HealthTexture.getSize().y));
	}
	else
	{
		m_HealthSprite.setTextureRect(sf::IntRect(0, 0, 0, m_HealthTexture.getSize().y));
	}

	m_RenderWindow->mapCoordsToPixel(m_EnemyHealthSprite.getPosition());
	m_RenderWindow->mapCoordsToPixel(m_EnemyHealthBorderSprite.getPosition());
	m_EnemyHealthSprite.setPosition(m_RenderWindow->getView().getCenter().x + m_RenderWindow->getView().getSize().x / 7.25, m_RenderWindow->getView().getCenter().y - m_RenderWindow->getView().getSize().y / 4.5);
	m_EnemyHealthBorderSprite.setPosition(m_RenderWindow->getView().getCenter().x + m_RenderWindow->getView().getSize().x / 7.25, m_RenderWindow->getView().getCenter().y - m_RenderWindow->getView().getSize().y / 4.5);
	if (m_EnemyCubemon->GetCurrentHealth() > 0)
	{
		m_EnemyHealthSprite.setTextureRect(sf::IntRect(0, 0, (int)m_EnemyCubemon->GetCurrentHealth(), m_HealthTexture.getSize().y));
	}
	else
	{
		m_EnemyHealthSprite.setTextureRect(sf::IntRect(0, 0, 0, m_HealthTexture.getSize().y));
	}

	m_RenderWindow->draw(m_EnemyHealthBorderSprite);
	m_RenderWindow->draw(m_EnemyHealthSprite);
	m_RenderWindow->draw(m_HealthBorderSprite);
	m_RenderWindow->draw(m_HealthSprite);

	m_RenderWindow->draw(m_EnemyCubemonName);
	m_RenderWindow->draw(m_FriendlyCubemonName);

	m_RenderWindow->setView(_worldView);
}
