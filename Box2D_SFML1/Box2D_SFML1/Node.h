//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) Media Design School
//
// File Name : Node.h
// Description : Node Header file.
// Author : William Inman
// Mail : william.inman@mds.ac.nz
//

#pragma once
#include "NumptyBehavior.h"
class Node : public NumptyBehavior
{
public:
	Vector2 m_Position{};

	int F = INT_MAX;
	int G = INT_MAX;
	int H = INT_MAX;

	Vector2 m_Parent{-1,-1};

	sf::Text m_FText;
	sf::Text m_GText;
	sf::Text m_HText;

	bool m_bObstical = false;
	bool m_bTraversed = false;
	bool m_bSource = false;
	bool m_bDestination = false;
};

