#pragma once
#ifndef _GUI_H__
#define _GUI_H__

// Local Includes
#include "Player.h"
#include "CButtons.h"

class GUI : public NumptyBehavior
{
public:
	GUI(sf::RenderWindow* _renderwindow, TextureMaster* _texturemaster, sf::Font &_font);
	virtual ~GUI();

	virtual void Start();
	virtual void Render(Player* _player, sf::Shader* _defaultshader = NULL ,sf::Vector2f _mousePos = sf::Vector2f(0, 0));

	sf::Vector2f UIMousePointer(sf::View& _uiview);
	void InitHealthAndManaUI();
	void HealthAndManaUI(sf::RenderWindow* _renderWindow, sf::View& _uiView, Player* _player);
	void TimerUI();

	void InitCubemonHUD(ICubemon* _friendly, ICubemon* _enemy);
	void InitBattleUI();
	void BattleUI(sf::View& _uiView, sf::View& _worldView);
	void HandleCubemonHUD(sf::View& _uiView, sf::View& _worldView);

	void InitButton(CButtons* _button, sf::Texture* _idleTexture, sf::Texture* _hoverTexture);

	ICubemon::CUBEMONTYPE GetCurrentCubemon();

	sf::Font m_Font;

private:
	sf::RenderWindow* m_RenderWindow = nullptr;
	TextureMaster* m_TextureMaster = nullptr;
	sf::Clock m_Timer;
	sf::Text m_DisplayText;

	sf::Sprite m_MousePointer;

	sf::Texture m_ManaTexture;
	sf::Texture m_ManaBorderTexture;
	sf::Sprite m_ManaSprite;
	sf::Sprite m_ManaBorderSprite;

	sf::Texture m_HealthTexture;
	sf::Texture m_HealthBorderTexture;

	sf::Sprite m_HealthSprite;
	sf::Sprite m_HealthBorderSprite;

	sf::Sprite m_EnemyHealthSprite;
	sf::Sprite m_EnemyHealthBorderSprite;

	sf::Text m_FriendlyCubemonName;
	sf::Text m_EnemyCubemonName;

	std::vector<CButtons*> m_BattleSceneButtons{};
	std::vector<sf::RectangleShape> m_BattleSceneMenuShapes{};

	std::vector<ICubemon::CUBEMONTYPE> m_CurrentlyHeldCubemons{};
	ICubemon::CUBEMONTYPE m_CurrentCubemon = ICubemon::CUBEMONTYPE::KINDLING;

	std::vector<CButtons*> m_BattleSceneAttackButtons{};
	std::vector<sf::RectangleShape> m_BattleSceneAttackShapes{};

	bool m_bAttack = false;
	bool m_bBackpack = false;
	bool m_bCubemon = false;
	bool m_bFlee = true;

	int m_PlayerAttackBuff = 0;
	int m_PlayerArmourBuff = 0;

	bool m_PlayersTurn = true;

	ICubemon* m_FriendlyCubemon = nullptr;
	ICubemon* m_EnemyCubemon = nullptr;

	sf::Clock m_ButtonRegulator;
	sf::Clock m_EnemyTurnTimer;
	sf::Clock m_EnemyAttackTimer;

	sf::Texture m_AttackButton;
	sf::Texture m_BackpackButton;
	sf::Texture m_CubeboyButton;
	sf::Texture m_FleeButton;

	sf::Texture m_AttackButton_Hover;
	sf::Texture m_BackpackButton_Hover;
	sf::Texture m_CubeboyButton_Hover;
	sf::Texture m_FleeButton_Hover;
	
	sf::Texture m_FireElement;
	sf::Texture m_WaterElement;
	sf::Texture m_EarthElement;
	sf::Texture m_AirElement;

	sf::Texture m_AttackFire;
	sf::Texture m_AttackWater;
	sf::Texture m_AttackEarth;
	sf::Texture m_AttackAir;

	sf::Texture m_AttackFire_Hover;
	sf::Texture m_AttackWater_Hover;
	sf::Texture m_AttackEarth_Hover;
	sf::Texture m_AttackAir_Hover;

	sf::Texture m_FireDeminishAttack;
	sf::Texture m_FireBurnAttack;
	sf::Texture m_FireEmberAttack;
	sf::Texture m_FireIncinerateAttack;

	sf::Texture m_KindlingCubemon;

	void CleanupBattleSceneButtons();
	void InitCubeBoyUI();
	void InitCubeBoyMenuButtons();
	void InitCubeBoyAttackUI();
	void InitCubeBoyAttackButtons();

	void InitTextures();
	void SetAllButtonScaling(float _newScale);

	void InitButtonPosScaleTexture(sf::Vector2f _position, sf::Vector2f _scale, sf::Texture* _idleTexture, sf::Texture* _hoverTexture, std::vector<CButtons*> _vector);

	void HandleKindlingGUIShapes(sf::RectangleShape& _item);
	void HandleKindlingGUIButtons(int _iter, CButtons* _button);
	void HandleButtonInteractions();
	void HandleKindlingButtonInteractions();

	void HandleEnemyTurn();

	inline void EndTurn()
	{
		m_PlayersTurn = false;
		m_EnemyAttackTimer.restart();
		m_EnemyTurnTimer.restart();
		m_ButtonRegulator.restart();
	}
};
#endif

class GUIButton
{
		

};