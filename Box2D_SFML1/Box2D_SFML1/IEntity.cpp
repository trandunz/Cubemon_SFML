#include "IEntity.h"

void IEntity::SetShapeToB2Body()
{
	// Set SFML Shape Transform To Box 2D Body Transform
	if (m_Shape != nullptr)
	{
		m_Shape->setPosition(m_Body->GetPosition().x * m_Scale, m_Body->GetPosition().y * m_Scale);
		m_Shape->setRotation(m_Body->GetAngle() * 180 / b2_pi);
	}
}

void IEntity::CreateShape()
{
	m_Shape = new sf::Sprite;
}

void IEntity::DestoryShape()
{
	DeletePointer(m_Shape);
	m_Shape = nullptr;
}

void IEntity::CreateBody(float _posX, float _posY, b2BodyType _type, bool _sensor)
{
	// Body
	m_BodyDef = new b2BodyDef;
	m_BodyDef->position = b2Vec2(_posX / m_Scale, _posY / m_Scale);
	m_BodyDef->type = _type;
	m_BodyDef->fixedRotation = 1;
	m_BodyDef->linearDamping = 1.0f;
	m_BodyDef->gravityScale = 0;
	m_Body = m_World->CreateBody(m_BodyDef);

	// Shape
	m_b2pShape = new b2PolygonShape;
	m_b2pShape->SetAsBox((50.0f / 2) / m_Scale, (50.0f / 2) / m_Scale);

	// Fixture
	m_FixtureDef = new b2FixtureDef;
	if (_sensor)
	{
		m_FixtureDef->isSensor = true;
	}
	m_FixtureDef->density = 2.0f;
	m_FixtureDef->friction = 0.1f;
	m_FixtureDef->restitution = 0.2f;
	m_FixtureDef->shape = m_b2pShape;
	m_FixtureDef->filter.groupIndex = -1;
	m_Body->CreateFixture(m_FixtureDef);
}

void IEntity::DestroyBody()
{
	if (m_World != nullptr && m_Body != nullptr)
	{
		m_World->DestroyBody(m_Body);
		m_Body = nullptr;
	}

	DeletePointer(m_FixtureDef);
	DeletePointer(m_b2pShape);
	DeletePointer(m_BodyDef);
	m_FixtureDef = nullptr;
	m_b2pShape = nullptr;
	m_BodyDef = nullptr;
}
