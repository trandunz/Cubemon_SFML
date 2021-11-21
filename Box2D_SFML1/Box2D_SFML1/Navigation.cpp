//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) Media Design School
//
// File Name : Navigation.cpp
// Description : Navigation Implementation file.
// Author : William Inman
// Mail : william.inman@mds.ac.nz
//

#include "Navigation.h"

/// <summary>
/// Navigation Constructor
/// Initializes The Param, Loads The Font ANDYB.TTF, Initializes The Closes List (False)
/// </summary>
/// <param name="_renderWindow"></param>
Navigation::Navigation(sf::RenderWindow* _renderWindow)
{
	m_RenderWindow = _renderWindow;
	LoadFont(m_Font, "Fonts/ANDYB.TTF");
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			m_ClosedList[i][j] = false;
		}
	}
}

/// <summary>
/// Navigation Destructor
/// </summary>
Navigation::~Navigation()
{
	CleanupContainers();
	m_SourceNode = nullptr;
	m_DestinationNode = nullptr;
	m_RenderWindow = nullptr;
}

/// <summary>
/// Navigation Start
/// Intializes The Nodes And Sf::Shapes
/// </summary>
void Navigation::Start()
{
	Initnodes();

	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			InitShapes(i, j);
		}
	}
}

/// <summary>
/// CALLED EVERY FRAME
/// Main Update Function For Navigation
/// Also Handles Changing Tiles At Runtime
/// </summary>
/// <param name="_mouserPos"></param>
void Navigation::Update(sf::Vector2f _mouserPos)
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		if (IsMouseOverTile(_mouserPos))
		{
			sf::Vector2i index = GetMousedOverTile(_mouserPos);
			if (!m_Nodes[index.x][index.y].m_bDestination && !m_Nodes[index.x][index.y].m_bSource)
			{
				ChangeTileType(index);
			}
		}
	}

	AStarTraversal(*m_SourceNode, *m_DestinationNode);

	UpdateText();
}

/// <summary>
/// Polled Update For Navigation Controlls
/// </summary>
/// <param name="_event"></param>
/// <param name="_mouserPos"></param>
void Navigation::PolledUpdate(sf::Event* _event, sf::Vector2f _mouserPos)
{
	if (_event->type == sf::Event::KeyPressed)
	{
		if (_event->key.code == sf::Keyboard::Tab)
		{
			ToggleDebug();
		}
		else if (_event->key.code == sf::Keyboard::F1)
		{
			m_bEraser = !m_bEraser;
		}
		else if (_event->key.code == sf::Keyboard::F2)
		{
			m_bOptimizeCorners = !m_bOptimizeCorners;
		}
	}
	else if (_event->type == sf::Event::MouseButtonPressed)
	{
		if (IsMouseOverTile(_mouserPos))
		{
			if (_event->key.code == sf::Mouse::Right)
			{
				sf::Vector2i index = GetMousedOverTile(_mouserPos);
				PlaceDestination(index.x, index.y);
			}
			else if (_event->key.code == sf::Mouse::Middle)
			{
				sf::Vector2i index = GetMousedOverTile(_mouserPos);
				PlaceSource(index.x, index.y);
			}
		}
	}
}

/// <summary>
/// CALLED EVERY FRAME
/// Renders The Navigation Information To The RenderWindow.
/// </summary>
void Navigation::Render()
{

	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			RenderShapes(i, j);
		}
	}

	SetShortestPathGreen();

	if (m_bDebug)
	{
		RenderText();
	}


}

/// <summary>
/// Handles Rendering The Specified Shapes At Locaton m_Shapes[_i][_j]
/// Also Handles The Colours For Each Node Depending On Their Assigned Type
/// </summary>
/// <param name="_i"></param>
/// <param name="_j"></param>
void Navigation::RenderShapes(int _i, int _j)
{
	SetNodeColourToType(_i, _j);

	m_RenderWindow->draw(m_Shapes[_i][_j]);
}

/// <summary>
/// Handles Rendering The Debug Text To The Screen
/// </summary>
void Navigation::RenderText()
{
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			if (m_Nodes[i][j].m_bTraversed && !m_Nodes[i][j].m_bSource && !m_Nodes[i][j].m_bDestination)
			{
				m_RenderWindow->draw(m_Nodes[i][j].m_FText);
				m_RenderWindow->draw(m_Nodes[i][j].m_GText);
				m_RenderWindow->draw(m_Nodes[i][j].m_HText);
			}
		}
	}
}

/// <summary>
/// Handles Updating The Debug Text To The Correct Values
/// </summary>
void Navigation::UpdateText()
{
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			m_Nodes[i][j].m_GText.setString(std::to_string(m_Nodes[i][j].G));
			m_Nodes[i][j].m_HText.setString(std::to_string(m_Nodes[i][j].H));
			m_Nodes[i][j].m_FText.setString(std::to_string(m_Nodes[i][j].F));
		}
	}
}

/// <summary>
/// Initalizes The Nodes For AStar Navigation
/// </summary>
void Navigation::Initnodes()
{
	for (int i = 0; i < SIZE; i++) 
	{
		for (int j = 0; j < SIZE; j++) 
		{
			m_Nodes[i][j].m_Position = std::make_pair(i,j);

			LoadTextWithFont(m_Nodes[i][j].m_FText, m_Font, sf::Color::Black);
			LoadTextWithFont(m_Nodes[i][j].m_GText, m_Font, sf::Color::Black);
			LoadTextWithFont(m_Nodes[i][j].m_HText, m_Font, sf::Color::Black);
		}
	}
}

/// <summary>
/// Initalizes The Shape At Location m_Shapes[_i][_j].
/// Size : 10,10
/// Fill Colour : Transp
/// Outline Thick : 0.1
/// Outline Colour : Green
/// </summary>
/// <param name="_i"></param>
/// <param name="_j"></param>
void Navigation::InitShapes(int _i, int _j)
{
	m_Shapes[_i][_j] = sf::RectangleShape(sf::Vector2f(100.0f, 100.0f));
	m_Shapes[_i][_j].setSize(sf::Vector2f(100.0f, 100.0f));
	m_Shapes[_i][_j].setFillColor(sf::Color::Transparent);
	m_Shapes[_i][_j].setOutlineThickness(0.1f);
	m_Shapes[_i][_j].setOutlineColor(sf::Color::Green);
	m_Shapes[_i][_j].setOrigin(50.0f, 50.0f);
	m_Shapes[_i][_j].setPosition((float)m_Nodes[_i][_j].m_Position.first * NODE_DIFFERENCE, (float)m_Nodes[_i][_j].m_Position.second * NODE_DIFFERENCE);

	m_Nodes[_i][_j].m_FText.setScale(0.12f, 0.12f);
	m_Nodes[_i][_j].m_GText.setScale(0.12f, 0.12f);
	m_Nodes[_i][_j].m_HText.setScale(0.12f, 0.12f);

	m_Nodes[_i][_j].m_FText.setOrigin(m_Nodes[_i][_j].m_FText.getGlobalBounds().width/2 , m_Nodes[_i][_j].m_FText.getGlobalBounds().height / 2);
	m_Nodes[_i][_j].m_GText.setOrigin(m_Nodes[_i][_j].m_GText.getGlobalBounds().width / 2, m_Nodes[_i][_j].m_GText.getGlobalBounds().height / 2);
	m_Nodes[_i][_j].m_HText.setOrigin(m_Nodes[_i][_j].m_HText.getGlobalBounds().width / 2, m_Nodes[_i][_j].m_HText.getGlobalBounds().height / 2);

	m_Nodes[_i][_j].m_FText.setPosition(m_Shapes[_i][_j].getPosition().x + 1.0f, m_Shapes[_i][_j].getPosition().y - 3.5f);
	m_Nodes[_i][_j].m_GText.setPosition(m_Shapes[_i][_j].getPosition().x - 3.5f, m_Shapes[_i][_j].getPosition().y + 1.5f);
	m_Nodes[_i][_j].m_HText.setPosition(m_Shapes[_i][_j].getPosition().x + 1.0f, m_Shapes[_i][_j].getPosition().y + 1.5f);
}

/// <summary>
/// Calculates A Path To The Specified Destination Node
/// </summary>
/// <param name="_destination"></param>
void Navigation::CalculatePath(Node& _destination)
{
	int i = _destination.m_Position.first;
	int j = _destination.m_Position.second;

	while (!(m_Nodes[i][j].m_Parent.first == i && m_Nodes[i][j].m_Parent.second == j)) 
	{
		m_Path.push(std::make_pair(i, j));
		int temp_i = m_Nodes[i][j].m_Parent.first;
		int temp_j = m_Nodes[i][j].m_Parent.second;
		i = temp_i;
		j = temp_j;
	}

	m_Path.push(std::make_pair(i, j));
	std::queue<Vector2> tempPath = m_Path;
	while (!m_Path.empty())
	{
		Vector2 BestMove = m_Path.front();
		m_Path.pop();
		m_Nodes[BestMove.first][BestMove.second].m_bTraversed = true;
	}

	m_Paths.push_back(tempPath);
}

/// <summary>
/// Handles AStar Traversal Between The Two Specified Nodes.
/// </summary>
/// <param name="_source"></param>
/// <param name="_destination"></param>
void Navigation::AStarTraversal(Node& _source, Node& _destination)
{
	if (m_SourceNode != nullptr && m_DestinationNode != nullptr)
	{
		CleanupContainers();
		std::set<std::pair<int, Vector2>> m_OpenList;

		bool reachedDestination = false;
		int i = 0;
		int j = 0;

		m_Nodes[_source.m_Position.first][_source.m_Position.second].m_Parent = _source.m_Parent;
		i = _source.m_Position.first;
		j = _source.m_Position.second;

		m_Nodes[i][j].F = 0;
		m_Nodes[i][j].G = 0;
		m_Nodes[i][j].H = 0;
		m_Nodes[i][j].m_Parent.first = i;
		m_Nodes[i][j].m_Parent.second = j;
		m_Nodes[i][j].m_bSource = true;
		m_Nodes[i][j].m_bDestination = false;

		m_OpenList.insert(std::make_pair(0, std::make_pair(i, j)));

		
		while (!m_OpenList.empty())
		{
			std::pair<int, Vector2> currentNode = *m_OpenList.begin();
			i = currentNode.second.first;
			j = currentNode.second.second;

			// Move To Close List
			m_OpenList.erase(m_OpenList.begin());
			m_ClosedList[i][j] = true;

			// Check Neighbors
			CalculateNeighbors(i, j, -1, 0, _destination, reachedDestination, m_OpenList, m_ClosedList);
			CalculateNeighbors(i, j, 1, 0, _destination, reachedDestination, m_OpenList, m_ClosedList);
			CalculateNeighbors(i, j, 0, -1, _destination, reachedDestination, m_OpenList, m_ClosedList);
			CalculateNeighbors(i, j, 0, 1, _destination, reachedDestination, m_OpenList, m_ClosedList);
			
			if (m_bOptimizeCorners)
			{
				if (!IsBlocked(std::make_pair(i - 1, j)) || !IsBlocked(std::make_pair(i, j - 1)))
				{
					CalculateNeighbors(i, j, -1, -1, _destination, reachedDestination, m_OpenList, m_ClosedList);
				}
				if (!IsBlocked(std::make_pair(i + 1, j)) || !IsBlocked(std::make_pair(i, j - 1)))
				{
					CalculateNeighbors(i, j, 1, -1, _destination, reachedDestination, m_OpenList, m_ClosedList);
				}
				if (!IsBlocked(std::make_pair(i - 1, j)) || !IsBlocked(std::make_pair(i, j + 1)))
				{
					CalculateNeighbors(i, j, -1, 1, _destination, reachedDestination, m_OpenList, m_ClosedList);
				}
				if (!IsBlocked(std::make_pair(i + 1, j)) || !IsBlocked(std::make_pair(i, j + 1)))
				{
					CalculateNeighbors(i, j, 1, 1, _destination, reachedDestination, m_OpenList, m_ClosedList);
				}
			}
			else
			{
				if (!IsBlocked(std::make_pair(i - 1, j)) && !IsBlocked(std::make_pair(i, j - 1)))
				{
					CalculateNeighbors(i, j, -1, -1, _destination, reachedDestination, m_OpenList, m_ClosedList);
				}
				if (!IsBlocked(std::make_pair(i + 1, j)) && !IsBlocked(std::make_pair(i, j - 1)))
				{
					CalculateNeighbors(i, j, 1, -1, _destination, reachedDestination, m_OpenList, m_ClosedList);
				}
				if (!IsBlocked(std::make_pair(i - 1, j)) && !IsBlocked(std::make_pair(i, j + 1)))
				{
					CalculateNeighbors(i, j, -1, 1, _destination, reachedDestination, m_OpenList, m_ClosedList);
				}
				if (!IsBlocked(std::make_pair(i + 1, j)) && !IsBlocked(std::make_pair(i, j + 1)))
				{
					CalculateNeighbors(i, j, 1, 1, _destination, reachedDestination, m_OpenList, m_ClosedList);
				}
			}
		}
	}
}

/// <summary>
/// Checks The Neighbor To The Specified Node At Location m_Nodes[_i + _offsetI][_j + _offsetJ]
/// </summary>
/// <param name="_i"></param>
/// <param name="_j"></param>
/// <param name="_offsetI"></param>
/// <param name="_offsetJ"></param>
/// <param name="_destination"></param>
/// <param name="_reachedDestination"></param>
/// <param name="_openList"></param>
/// <param name="m_ClosedList"></param>
void Navigation::CalculateNeighbors(int _i, int _j, int _offsetI, int _offsetJ, Node& _destination, bool& _reachedDestination, std::set<std::pair<int, std::pair<int, int>>>& _openList, bool m_ClosedList[SIZE][SIZE])
{
	int newG = 0;
	int newF = 0;
	int newH = 0;

	if (IsValid(_i + _offsetI, _j + _offsetJ) == true)
	{
		if (IsDestination(std::make_pair(_i + _offsetI, _j + _offsetJ), _destination.m_Position) == true)
		{
			m_Nodes[_i + _offsetI][_j + _offsetJ].m_Parent.first = _i;
			m_Nodes[_i + _offsetI][_j + _offsetJ].m_Parent.second = _j;
			CalculatePath(_destination);
			_reachedDestination = true;
			m_Nodes[_i + _offsetI][_j + _offsetJ].m_bDestination = true;
			return;
		}
		else if (m_ClosedList[_i + _offsetI][_j + _offsetJ] == false && !IsBlocked(std::make_pair(_i + _offsetI, _j + _offsetJ)) == true)
		{
			newG = m_Nodes[_i][_j].G + 1;
			newH = CalculateHValue(std::make_pair(_offsetI + _i, _offsetJ+ _j), _destination.m_Position);
			newF = newG + newH;

			if (m_Nodes[_offsetI + _i][_j + _offsetJ].F == INT_MAX || m_Nodes[_i + _offsetI][_j + _offsetJ].F > newF)
			{
				_openList.insert(std::make_pair(newF, std::make_pair(_i + _offsetI, _j + _offsetJ)));

				m_Nodes[_i + _offsetI][_j + _offsetJ].F = newF;
				m_Nodes[_i + _offsetI][_j + _offsetJ].G = newG;
				m_Nodes[_i + _offsetI][_j + _offsetJ].H = newH;
				m_Nodes[_i + _offsetI][_j + _offsetJ].m_Parent.first = _i;
				m_Nodes[_i + _offsetI][_j + _offsetJ].m_Parent.second = _j;
			}
		}
		return;
	}
}

/// <summary>
/// Toggles m_bDebug
/// Allows You To See The Values For Each Node
/// </summary>
void Navigation::ToggleDebug()
{
	m_bDebug = !m_bDebug;
}

/// <summary>
/// Return A bool Based On If The Given sf::Vector2f  Position Is Over Any m_Shapes
/// </summary>
/// <param name="_mousePos"></param>
/// <returns></returns>
bool Navigation::IsMouseOverTile(sf::Vector2f _mousePos)
{
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			if (m_Shapes[i][j].getGlobalBounds().contains(_mousePos))
			{
				return true;
			}
		}
	}
	return false;
}

/// <summary>
/// Returns The Location Of The Node The Passed In Position Is Over
/// </summary>
/// <param name="_mousePos"></param>
/// <returns></returns>
sf::Vector2i Navigation::GetMousedOverTile(sf::Vector2f _mousePos)
{
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			if (m_Shapes[i][j].getGlobalBounds().contains(_mousePos))
			{
				return sf::Vector2i(i,j);
			}
		}
	}
	return sf::Vector2i(0,0);
}

/// <summary>
/// Changes The Tile Type At Location m_Nodes[_index.x][_index.y] To Eather Empty Or An Obsticle
/// (Toggles)
/// </summary>
/// <param name="_index"></param>
void Navigation::ChangeTileType(sf::Vector2i _index)
{
	m_Nodes[_index.x][_index.y].m_bObstical = !m_bEraser;
	
	if (m_Nodes[_index.x][_index.y].m_bSource)
	{
		m_Nodes[_index.x][_index.y].m_bSource = false;
		m_SourceNode = nullptr;
	}

	SetNodeColourToType(_index.x, _index.y);
}

/// <summary>
/// Cleans Up All The Pointers And Memory For The Navigation
/// </summary>
void Navigation::CleanupContainers()
{
	// Path Stack Cleanup
	m_Path = std::queue<Vector2>();

	// Node Value Reset
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			m_Nodes[i][j].H = INT_MAX;
			m_Nodes[i][j].G = INT_MAX;
			m_Nodes[i][j].F = INT_MAX;

			m_Nodes[i][j].m_bTraversed = false;
			m_Nodes[i][j].m_bSource = false;
			m_Nodes[i][j].m_bDestination = false;
			m_Nodes[i][j].m_Parent = std::make_pair(-1, -1);
		}
	}

	// Closed List Value Reset
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			m_ClosedList[i][j] = false;
		}
	}
}

/// <summary>
/// Searches All The Calculated Paths And Finds The Shortest One Before Colouring It Green
/// </summary>
void Navigation::SetShortestPathGreen()
{
	size_t size = INT_MAX;
	for (auto& item : m_Paths)
	{
		if (item.size() < size)
		{
			size = item.size();
		}
	}
	for (auto& item : m_Paths)
	{
		if (item.size() == size)
		{
			while (item.size() > 0)
			{
				if (!m_Nodes[item.front().first][item.front().second].m_bDestination && !m_Nodes[item.front().first][item.front().second].m_bSource)
				{
					m_Shapes[item.front().first][item.front().second].setFillColor(sf::Color::Green);
				}
				m_RenderWindow->draw(m_Shapes[item.front().first][item.front().second]);
				item.pop();
			}
			break;
		}
	}
	for (auto& item : m_Paths)
	{
		while (item.size() > 0)
		{
			item.pop();
		}
	}
	m_Paths.clear();
}

/// <summary>
/// Sets The Colour Of The Specified Node At Location m_Nodes[_i][_j] To Its Appropriate Type
/// </summary>
/// <param name="_i"></param>
/// <param name="_j"></param>
void Navigation::SetNodeColourToType(int _i, int _j)
{
	if (m_Nodes[_i][_j].m_bSource)
	{
		m_Shapes[_i][_j].setFillColor(sf::Color::Cyan);
	}
	if (m_Nodes[_i][_j].m_bObstical)
	{
		m_Shapes[_i][_j].setFillColor(sf::Color::Red);
	}
	if (m_Nodes[_i][_j].m_bDestination)
	{
		m_Shapes[_i][_j].setFillColor(sf::Color::Magenta);
	}
	if (!m_Nodes[_i][_j].m_bObstical && !m_Nodes[_i][_j].m_bSource && !m_Nodes[_i][_j].m_bDestination)
	{
		m_Shapes[_i][_j].setFillColor(sf::Color::Transparent);
	}
}

/// <summary>
/// Returns The Source Node
/// </summary>
/// <returns></returns>
Node& Navigation::GetSourceNode()
{
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			if (m_Nodes[i][j].m_bSource)
			{
				return m_Nodes[i][j];
			}
		}
	}
	return m_Nodes[0][0];
}

/// <summary>
/// Returns The Destination Node
/// </summary>
/// <returns></returns>
Node& Navigation::GetDestinationNode()
{
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			if (m_Nodes[i][j].m_bDestination)
			{
				return m_Nodes[i][j];
			}
		}
	}
	return m_Nodes[0][0];
}
