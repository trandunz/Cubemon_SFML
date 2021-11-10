#include "Tile.h"

Tile::Tile(sf::RenderWindow* _renderWindow, b2World& _world, sf::Texture* _texture, sf::Vector2f _position, sf::Vector2f _size, std::string _tileType)
{
	m_World = &_world;
	m_RenderWindow = _renderWindow;
	m_Texture = _texture;

	CreateShape();

	if (_tileType == "Wall")
	{
		CreateBody(_size.x, _size.y, _position.x, _position.y, b2_staticBody);
		LoadSpriteTexture(_texture, m_Shape, false);
	}
	else if (_tileType == "Floor")
	{
		LoadSpriteTexture(_texture, m_Shape);
	}
	else if (_tileType == "Event")
	{
		CreateBody(_size.x, _size.y, _position.x, _position.y, b2_staticBody, true);
		LoadSpriteTexture(_texture, m_Shape);
	}
	else if (_tileType == "Door")
	{
		CreateBody(_size.x, _size.y, _position.x, _position.y, b2_staticBody, true);
		LoadSpriteTexture(_texture, m_Shape);
	}

	if (m_Shape != nullptr)
	{
		m_Shape->setPosition(_position);
	}
}

Tile::~Tile()
{
	DestroyShape();
	DestroyBody();
	m_RenderWindow = nullptr;
	m_Body = nullptr;
	m_World = nullptr;
	m_Texture = nullptr;
}

void Tile::Update()
{	
	SetShapeToB2Body();
}

void Tile::Render()
{
	RenderSpritePointer(m_RenderWindow, m_Shape);
}

void Tile::CreateShape()
{
	DeletePointer(m_Shape);
	m_Shape = new sf::Sprite;
}

void Tile::DestroyShape()
{
	DeletePointer(m_Shape);
	m_Shape = nullptr;
}

void Tile::CreateBody(float _sizeX, float _sizeY, float _posX, float _posY, b2BodyType _type, bool _sensor)
{
	// Body
	m_BodyDef = new b2BodyDef;
	m_BodyDef->position = b2Vec2(_posX / m_Scale, _posY / m_Scale);
	m_BodyDef->type = _type;
	m_BodyDef->fixedRotation = 1;
	m_BodyDef->linearDamping = 0.4f;
	m_BodyDef->gravityScale = 10;
	m_Body = m_World->CreateBody(m_BodyDef);

	// Shape
	m_b2pShape = new b2PolygonShape;
	m_b2pShape->SetAsBox((100.f / 2) / m_Scale, (100.f / 2) / m_Scale);

	// Fixture
	m_FixtureDef = new b2FixtureDef;
	if (_sensor)
	{
		m_FixtureDef->filter.categoryBits = 0x0002;
		m_FixtureDef->isSensor = true;
	}
	m_FixtureDef->density = 2.0f;
	m_FixtureDef->friction = 1.0f;
	m_FixtureDef->restitution = 0.2f;
	m_FixtureDef->shape = m_b2pShape;
	m_Body->CreateFixture(m_FixtureDef);
}

sf::Sprite* Tile::GetShape()
{
	return m_Shape;
}

void Tile::SetShapeToB2Body()
{
	// Set SFML Shape Transform To Box 2D Body Transform
	if (m_Shape != nullptr && m_Body != nullptr)
	{
		m_Shape->setOrigin(m_Shape->getGlobalBounds().width / 2, m_Shape->getGlobalBounds().height / 2);
		m_Shape->setPosition(m_Body->GetPosition().x * m_Scale, m_Body->GetPosition().y * m_Scale);
		m_Shape->setRotation(m_Body->GetAngle() * 180 / b2_pi);
	}
}

void Tile::DestroyBody()
{
	if (m_Body != nullptr)
	{
		m_World->DestroyBody(m_Body);
		DeletePointer(m_FixtureDef);
		DeletePointer(m_b2pShape);
		DeletePointer(m_BodyDef);
		m_FixtureDef = nullptr;
		m_b2pShape = nullptr;
		m_BodyDef = nullptr;
		m_Body = nullptr;
	}
}


