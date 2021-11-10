#pragma once

#include "NumptyBehavior.h"
#include "TextureMaster.h"
#include "AudioManager.h"
#include "ParticleSystem.h"
#include "CContactListener.h"

class IEntity :	public NumptyBehavior
{
public:
	enum class ENTITYTYPE
	{
		UNASSIGNED,

		PLAYER = 1,
		CUBEMON = 2,
		ENEMY = 3,
	};

	virtual ~IEntity() 
	{ 
		DeletePointer(m_ParticleSystem); 
		DeletePointer(m_ContactListener);
		DestoryShape();
		DestroyBody();
		m_RenderWindow = nullptr;
		m_TextureMaster = nullptr;
		m_AudioManager = nullptr;
		m_ParticleSystem = nullptr;
		m_ContactListener = nullptr;
		m_World = nullptr;
	}

	inline b2Body* GetBody() { return m_Body; }
	inline sf::Sprite* GetShape() { return m_Shape; }
	inline ParticleSystem* GetParticleSystem() { return m_ParticleSystem; }

	bool m_MARKASDESTROY = false;

protected:
	void SetShapeToB2Body();

	void CreateShape();
	void DestoryShape();
	void CreateBody(float _posX, float _posY, b2BodyType _type, bool _sensor = false);
	void DestroyBody();

	sf::RenderWindow* m_RenderWindow = nullptr;
	TextureMaster* m_TextureMaster = nullptr;
	AudioManager* m_AudioManager = nullptr;
	ParticleSystem* m_ParticleSystem = nullptr;
	CContactListener* m_ContactListener = nullptr;

	sf::Sprite* m_Shape = nullptr;
	b2BodyDef* m_BodyDef = nullptr;
	b2PolygonShape* m_b2pShape = nullptr;
	b2FixtureDef* m_FixtureDef = nullptr;
	b2Body* m_Body = nullptr;
	b2World* m_World = nullptr;
};

