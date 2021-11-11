#pragma once
#include "ICubemon.h"

class CThallic : public ICubemon
{
public:
	CThallic(sf::RenderWindow* _renderWindow, AudioManager* _audioManager);
	CThallic(sf::RenderWindow* _renderWindow, b2World* _world, b2Body& _playerBody);
	virtual ~CThallic();

	virtual void Start();
	virtual void Update();
	virtual void Render();

protected:
	virtual void Movement();
	virtual void Attack();

	b2Body* m_PlayerBody = nullptr;
};

