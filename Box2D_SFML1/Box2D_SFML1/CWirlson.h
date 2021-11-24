#pragma once
#include "ICubemon.h"
class CWirlson : public ICubemon
{
public:
	CWirlson(sf::RenderWindow* _renderWindow, AudioManager* _audioManager);
	CWirlson(sf::RenderWindow* _renderWindow, b2World* _world, b2Body& _playerBody);
	virtual ~CWirlson();

	virtual void Start();
	virtual void Update();
	virtual void Render();

protected:
	virtual void Movement();
	virtual void Attack();

	b2Body* m_PlayerBody = nullptr;
};

