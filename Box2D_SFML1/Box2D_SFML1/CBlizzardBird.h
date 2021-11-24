#pragma once
#include "ICubemon.h"
class CBlizzardBird : public ICubemon
{
public:
	CBlizzardBird(sf::RenderWindow* _renderWindow, AudioManager* _audioManager);
	CBlizzardBird(sf::RenderWindow* _renderWindow, b2World* _world, b2Body& _playerBody);
	virtual ~CBlizzardBird();

	virtual void Start();
	virtual void Update();
	virtual void Render();

protected:
	virtual void Movement();
	virtual void Attack();

	b2Body* m_PlayerBody = nullptr;
};

