#pragma once
#ifndef _PLAYER_H__
#define _PLAYER_H__

#define SPRITE_SHEET_SCALE 5

#include "IEntity.h"
#include "AudioManager.h"
#include "TextureMaster.h"
#include "ParticleSystem.h"
#include "CThallic.h"
#include "CKindling.h"
#include "CBrutus.h"

class Player : public IEntity
{
public:
	Player(sf::RenderWindow* _renderWindow, b2World& _world, AudioManager* _audiomanager, TextureMaster* _texturemaster);
	virtual ~Player();

	virtual void Start();
	virtual void Update(sf::Vector2f _mousepos);
	virtual void Render(sf::Shader* _defaultshader = NULL);

	void PollMovement(sf::Event& _event);
	void Movement();

	void SetCurrentMana(float _mana);
	float GetCurrentMana();
	void TakeDamage(float _damage);
	void Heal(float _amount);
	void SetCurrentHealth(float _health);
	float GetCurrentHealth();

	inline std::vector<ICubemon*>& GetCubemonVector() { return m_CubemonVector; }
	void AddCubemon(ICubemon* _cubeMon);

	void UpdateCubemon();
	void RenderCubemon();
	void LateCubemonRender();

	static void ResetPlayerData();

	void WriteCubmonData();
	void GrabCubmonData();
	static std::vector<ICubemon::CUBEMONTYPE> ReturnCubemonData();

	void WriteInventoryData();
	void ReadInventoryData();

private:
	void Animation(b2Vec2 _movementVector);

	sf::Vector2f GrabPlayerData();
	void WritePlayerData();

	void BattleTransition();

	void CleanupCubemon();
	std::vector<ICubemon*> m_CubemonVector;

	void HealAllPokemon();

	virtual void CreateBody(float _posX, float _posY, b2BodyType _type, bool _sensor = false);

	sf::Clock m_DamageTakenTimer;
	sf::Clock m_ManaRegen;
	float m_ManaRegenFrequency = 1.0f;
	float m_CurrentMana = 100.0f;
	float m_MaxMana = 100.0f;
	float m_CurrentHealth = 100.0f;
	float m_MaxHealth = 100.0f;
	float m_iMovementSpeed = 7.0f;
	float m_JumpForce = 500.0f;
	bool m_bCanJump = false;
	sf::Clock m_ParticleClock;
	b2Vec2 m_Velocity;
	sf::Vector2f m_MousePos;

	sf::Texture m_SpriteSheet;

	sf::Clock m_AnimationClock;
	float m_FrameDelay = 0.3f;
	sf::Clock m_EncounterClock;

	bool m_Encounter = false;
	sf::Clock m_BattleTimer;

	bool m_bEnteredRoom = false;
	sf::Clock m_InteractionDelayTimer;
	sf::Clock m_HealTimer;
};

#endif