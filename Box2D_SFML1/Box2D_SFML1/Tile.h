#pragma once
#ifndef _TILE_H__
#define _TILE_H__

#include "NumptyBehavior.h"

class Tile : public NumptyBehavior
{
public:
	Tile(sf::RenderWindow* _renderWindow, b2World& _world, sf::Texture* _texture, sf::Vector2f _position, sf::Vector2f _size, std::string _tileType = "Wall");
	virtual ~Tile();

	virtual void Update();
	virtual void Render();

	void CreateShape();
	void DestroyShape();
	virtual void CreateBody(float _sizeX, float _sizeY, float _posX, float _posY, b2BodyType _type, bool _sensor = false);
	void DestroyBody();

	sf::Sprite* GetShape();

protected:
	void SetShapeToB2Body();

	sf::RenderWindow* m_RenderWindow = nullptr;
	
	sf::Texture* m_Texture = nullptr;
	sf::Sprite* m_Shape = nullptr;
	b2BodyDef* m_BodyDef = nullptr;
	b2PolygonShape* m_b2pShape = nullptr;
	b2FixtureDef* m_FixtureDef = nullptr;
	b2Body* m_Body = nullptr;
	b2World* m_World = nullptr;
};
#endif