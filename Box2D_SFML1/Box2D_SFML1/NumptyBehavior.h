#pragma once
#ifndef _NumptyBehavior__
#define _NumptyBehavior__

#define ASCIIOFFSET 48

// Non-Local Includes
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <map>
#include <algorithm>
#include <box2d/box2d.h>
#include <fstream>
#include <stdio.h>
#include <conio.h>
#include <utils-master/stringutils.h>
#include <ctime>
#include <cstdlib>

class NumptyBehavior
{
public:
	typedef std::pair<int, int> Vector2;

	static inline float Mag(Vector2 _vec1, Vector2 _vec2)
	{
		float dx = (float)(_vec2.first - _vec1.first);
		float dy = (float)(_vec2.second - _vec1.second);

		return sqrt((dx * dx) + (dy * dy));
	}

	static inline sf::Font& LoadFont(sf::Font& _font, std::string _path)
	{
		_font.loadFromFile(_path);
		return _font;
	}

	static inline void LoadTextWithFont(sf::Text& _text, sf::Font& _font, sf::Color _colour = sf::Color::White, unsigned _characterSize = 12)
	{
		_text.setFont(_font);
		_text.setCharacterSize(_characterSize);
		_text.setFillColor(_colour);
	}

	template <typename T>
	static inline void DeletePointer(T* _pointer)
	{
		if (!IsNullptr(_pointer))
		{
			delete _pointer;
		}
	}

	template <typename T>
	static inline bool IsNullptr(T* _pointer)
	{
		if (_pointer == nullptr)
		{
			return true;
		}
		return false;
	}

	static inline sf::Texture* CreateTexture()
	{
		auto texture = new sf::Texture;
		return texture;
	}

	static inline sf::Texture* LoadTexture(sf::Texture* _texture, std::string _filePath, bool _repeated = true)
	{
		if (_texture != nullptr)
		{
			_texture->loadFromFile("Resources/Images/" + _filePath);
			_texture->setRepeated(_repeated);
		}
		return _texture;
	}

	static inline sf::Sprite* LoadSpriteTexture(sf::Texture* _texture, sf::Sprite* _sprite, bool _resetTextRect = true, bool _setOriginCentre = true)
	{
		if (_sprite != nullptr)
		{
			_sprite->setTexture(*_texture, _resetTextRect);

			if (_setOriginCentre)
			{
				_sprite->setOrigin(_sprite->getGlobalBounds().width / 2, _sprite->getGlobalBounds().height / 2);
			}
		}
		return _sprite;
	}

	static inline void RenderSpritePointer(sf::RenderWindow* _renderWindow, sf::Sprite* _sprite)
	{
		if (_renderWindow != nullptr)
		{
			if (_sprite != nullptr)
			{
				_renderWindow->draw(*_sprite);
			}
		}
	}

	static inline void InterceptSceneChange(int _change) 
	{
		std::ofstream file;

		file.open("Resources/Output/SceneSettings.ini");
		if (file.is_open())
		{
			file.clear();

			file << _change;
		}
		file.close();
	}

	static inline int ReturnSceneChange()
	{
		std::ifstream filepart2;
		int value = 0;
		filepart2.open("Resources/Output/SceneSettings.ini");
		if (filepart2.is_open())
		{
			filepart2 >> value;
		}
		filepart2.close();
		return value;
	}

	static inline sf::Vector2f SetOriginToCenter(sf::Sprite& _sprite)
	{
		_sprite.setOrigin(_sprite.getGlobalBounds().width / 2, _sprite.getGlobalBounds().height / 2);
		return _sprite.getOrigin();
	}

	static inline float Mag(b2Vec2 _vector)
	{
		return sqrt((_vector.x * _vector.x) + (_vector.y * _vector.y));
	}
	static inline float Mag(float _dx, float _dy)
	{
		return sqrt((_dx * _dx) + (_dy * _dy));
	}

protected:
	virtual void Start() {}
	virtual void Update() {}
	virtual void PolledUpdate() {}
	virtual void Render() {}

	float m_Scale = 45.0f;
};
#endif

