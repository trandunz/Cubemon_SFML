#pragma once
#include "ICubemon.h"

class CKindling : public ICubemon
{
public:
	CKindling(sf::RenderWindow* _renderWindow);
	CKindling(sf::RenderWindow* _renderWindow, b2World* _world, b2Body& _playerBody);
	virtual ~CKindling();

	virtual void Start();
	virtual void Update();
	virtual void Render();

protected:
	virtual void Movement();
	virtual void Attack();

	b2Body* m_PlayerBody = nullptr;
};

