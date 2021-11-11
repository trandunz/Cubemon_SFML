#pragma once
#include "ICubemon.h"
class CBrutus :	public ICubemon
{
public:
	CBrutus(sf::RenderWindow* _renderWindow, AudioManager* _audioManager);
	CBrutus(sf::RenderWindow* _renderWindow, b2World* _world, b2Body& _playerBody);
	virtual ~CBrutus();

	virtual void Start();
	virtual void Update();
	virtual void Render();

protected:
	virtual void Movement();
	virtual void Attack();

	b2Body* m_PlayerBody = nullptr;
};

