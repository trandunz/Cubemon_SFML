//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) Media Design School
//
// File Name : Navigation.h
// Description : Navigation Header file.
// Author : William Inman
// Mail : william.inman@mds.ac.nz
//

#pragma once

#include "NumptyBehavior.h"

#define NODE_DIFFERENCE 100
#define SIZE 20
#define DIAGONAL true

#include "node.h"

class Navigation : public NumptyBehavior
{
public:
	Navigation(sf::RenderWindow* _renderWindow);
	~Navigation();

	virtual void Start() override;
	virtual void Update(sf::Vector2f _mouserPos);
	virtual void PolledUpdate(sf::Event* _event, sf::Vector2f _mouserPos);
	virtual void Render() override;

	void Initnodes();
	void InitShapes(int _i, int _j);

	void UpdateText();

	void RenderShapes(int _i, int _j);
	void RenderText();

	void CalculatePath(Node& _destination);
	void AStarTraversal(Node& _source, Node& _destination);

	void CalculateNeighbors(int _i, int _j, int _offsetI, int _offsetJ, Node& _destination, bool& _reachedDestination, std::set<std::pair<int, std::pair<int, int>>>& _openList, bool m_ClosedList[SIZE][SIZE]);

	void ToggleDebug();

	bool IsMouseOverTile(sf::Vector2f _mousePos);

	sf::Vector2i GetMousedOverTile(sf::Vector2f _mousePos);

	Node& GetSourceNode();
	Node& GetDestinationNode();

	void SetNodeColourToType(int _i, int _j);

	void ChangeTileType(sf::Vector2i _index);

	void CleanupContainers();

	void SetShortestPathGreen();

	bool m_bEraser = true;
	bool m_bOptimizeCorners = false;
protected:
	inline int CalculateHValue(Vector2 _node, Vector2 _destination)
	{
		int dx = _node.first - _destination.first;
		int dy = _node.second - _destination.second;
		int h = abs(dx) + abs(dy);
		return  h;
	}

	inline bool IsValid(int _i , int _j)
	{
		if (_i >= 0 && _j >= 0 && _i < SIZE && _j < SIZE)
		{
			return true;
		}
		return false;
	}

	inline bool IsBlocked(Vector2 _position)
	{
		if (m_Nodes[_position.first][_position.second].m_bObstical == true)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	inline bool IsDestination(Vector2 _position, Vector2 _destination)
	{
		if (_position.first == _destination.first && _position.second == _destination.second)
		{
			return (true);
		}
		return (false);
	}

	inline void PlaceDestination(int _i, int _j)
	{
		if (!m_Nodes[_i][_j].m_bSource && !m_Nodes[_i][_j].m_bObstical)
		{
			m_Nodes[_i][_j].m_bDestination = true;
			m_DestinationNode = &m_Nodes[_i][_j];
		}
	}

	inline void PlaceSource(int _i, int _j)
	{
		if (!m_Nodes[_i][_j].m_bDestination && !m_Nodes[_i][_j].m_bObstical)
		{
			m_Nodes[_i][_j].m_bSource = true;
			m_SourceNode = &m_Nodes[_i][_j];
		}
	}

	Node m_Nodes[SIZE][SIZE]{};
	std::queue<Vector2> m_Path;
	std::vector< std::queue<Vector2>> m_Paths;
	sf::RectangleShape m_Shapes[SIZE][SIZE];
	bool m_ClosedList[SIZE][SIZE];

	bool m_bDebug = false;

	sf::Font m_Font;
	sf::RenderWindow* m_RenderWindow = nullptr;

	Node* m_SourceNode = nullptr;
	Node* m_DestinationNode = nullptr;
};

