//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) Media Design School
//
// File Name : CInputField.h
// Description : CInputField Header file.
// Author : William Inman
// Mail : william.inman@mds.ac.nz
//

#pragma once
#ifndef _INPUTFIELD_H__
#define _INPUTFIELD_H__

#include "NumptyBehavior.h"

class CInputField : public NumptyBehavior
{
public:
	std::string m_Label; // Button Label

	CInputField(sf::RenderWindow* _renderWindow);
	virtual ~CInputField();

	virtual void Start() = 0;
	virtual void Update();
	virtual void Render();

	void SetMousePosition(sf::Vector2f _position);
	sf::Vector2f GetMousePosition();

	bool bIsinBounds(sf::Vector2f _vMousePosition);

	void SetLabel(std::string _label);

	void SetState(std::string _type);

	void SetPosition(float _newX, float _newY);

	inline float GetHeight()
	{
		return Sprite.getGlobalBounds().height;
	}

	inline float GetWidth()
	{
		return Sprite.getGlobalBounds().width;
	}

	std::string removeLastChar(std::string _string);


private:
	bool m_bIsHovering;
	bool m_bIsPressed;

	sf::RenderWindow* m_RenderWindow = nullptr;

	sf::Text m_tLabel;
	
	std::string m_OnClickLabel;
	std::string m_HoverLabel;

	sf::Sprite Sprite;

	sf::Texture m_IdleTexture;
	sf::Texture m_HoverTexture;

	sf::Font m_Font;

	sf::Vector2f m_vMousePosition;
};
#endif

