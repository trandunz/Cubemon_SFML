#pragma once
#include "ICubemon.h"
class CDustDevil :	public ICubemon
{
public:
	CDustDevil(sf::RenderWindow* _renderWindow, AudioManager* _audioManager);
	CDustDevil(sf::RenderWindow* _renderWindow, b2World* _world, b2Body& _playerBody);
	virtual ~CDustDevil();

	virtual void Start();
	virtual void Update();
	virtual void Render();

protected:
	virtual void Movement();
	virtual void Attack();

	b2Body* m_PlayerBody = nullptr;
};

