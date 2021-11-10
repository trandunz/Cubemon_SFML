//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) Media Design School
//
// File Name : CButtons.cpp
// Description : CButtons Implementation file.
// Author : William Inman
// Mail : william.inman@mds.ac.nz
//

// Local Includes
#include "CButtons.h"

/// <summary>
/// Buttons Constructor.
/// Takes in:	sf::RenderWindow* (Intended To Be The PopOutMenu Render Window).
/// </summary>
CButtons::CButtons(sf::RenderWindow* _renderWindow)
{
	m_IdleTexture = new sf::Texture();
	m_ClickTexture = new sf::Texture();
	m_HoverTexture = new sf::Texture();
	if (m_IdleTexture != nullptr)
	{
		// Load Textures And Fonts
		m_IdleTexture->loadFromFile("Resources/Images/ItemSpacer.png");
		m_HoverTexture->loadFromFile("Resources/Images/ItemSpacer-Hover.png");
		m_ClickTexture->loadFromFile("Resources/Images/CurrentItem.png");

		// Make Them Smooth
		m_IdleTexture->setSmooth(true);
		m_HoverTexture->setSmooth(true);
		m_ClickTexture->setSmooth(true);
	}
	m_Font.loadFromFile("Resources/Fonts/ANDYB.TTF");

	// Set Up Font
	m_tLabel.setFont(m_Font);
	m_tLabel.setCharacterSize(24);
	m_tLabel.setFillColor(sf::Color::Black);
	
	// Set Button Texture To Idle
	if (m_IdleTexture != nullptr)
	{
		Sprite.setTexture(*m_IdleTexture, true);
	}
	Sprite.setScale(0.3f, 0.3f);
	m_tLabel.setScale(0.3f, 0.3f);

	// Set Origin's
	m_tLabel.setOrigin(m_tLabel.getGlobalBounds().width / 2, m_tLabel.getGlobalBounds().height / 2);
	Sprite.setOrigin(Sprite.getGlobalBounds().width / 2, Sprite.getGlobalBounds().height / 2);
	
	// Initialization
	m_RenderWindow = _renderWindow;

	m_bIsHovering = false;
	m_bIsPressed = false;
	m_bIsClicked = false;

	m_Label = "Default";
	m_OnClickLabel = m_Label;
	m_HoverLabel = m_Label;
}

/// <summary>
/// Buttons Destructor
/// </summary>
CButtons::~CButtons()
{
	// Cleanup Pointer
	m_RenderWindow = nullptr;
	if (!m_HasChangedIdle)
	{
		DeletePointer(m_IdleTexture);
	}
	m_IdleTexture = nullptr;
	if (!m_HasChangedHover)
	{
		DeletePointer(m_HoverTexture);
	}
	m_HoverTexture = nullptr;
	if (!m_HasChangedClick)
	{
		DeletePointer(m_ClickTexture);
	}
	m_ClickTexture = nullptr;
}

/// <summary>
/// Update Definition For Buttons.
/// </summary>
void CButtons::Update()
{
	// Set m_vMousePosition To Mouse Position
	sf::Vector2i pixelPos = sf::Mouse::getPosition((*m_RenderWindow));

	SetMousePosition(m_RenderWindow->mapPixelToCoords(pixelPos));

	// Checks If Mouse Is On Button
	bIsinBounds(m_vMousePosition);
}

/// <summary>
/// Render Definition For Buttons.
/// </summary>
void CButtons::Render()
{
	m_RenderWindow->draw(Sprite);
	m_RenderWindow->draw(m_tLabel);
}

/// <summary>
/// Draws Only The Label To m_RenderWindow
/// </summary>
void CButtons::RenderOnlyLabel()
{
	m_RenderWindow->draw(m_tLabel);
}

/// <summary>
/// Assigns sf::Vector2f(Position) to m_vMousePosition 
/// </summary>
/// <param name="Position"></param>
void CButtons::SetMousePosition(sf::Vector2f _position)
{
	m_vMousePosition = _position;
}

/// <summary>
/// Returns m_vMousePosition (sf::Vector2f)
/// </summary>
/// <returns></returns>
sf::Vector2f CButtons::GetMousePosition()
{
	/*std::cout << m_vMousePosition.x << std::endl;
	std::cout << m_vMousePosition.y << std::endl;*/

	return m_vMousePosition;
}

/// <summary>
/// Set The Position Of The Button To float (newX), float (newY).
/// </summary>
/// <param name="newX"></param>
/// <param name="newY"></param>
void CButtons::SetPosition(float _newX, float _newY)
{
	Sprite.setPosition(sf::Vector2f(_newX, _newY));
	m_tLabel.setPosition(sf::Vector2f(_newX, _newY));
}

sf::Vector2f CButtons::GetPosition()
{
	return Sprite.getPosition();
}

/// <summary>
/// Sets The Label Of The Button To Specified Parameter.
/// Takes In: std::string (_newLabel)
/// </summary>
/// <param name="_newLabel"></param>
void CButtons::SetLabel(std::string _newLabel)
{
	// Temp Creation
	std::string oldLabel = m_Label;
	std::string oldClickLabel = m_OnClickLabel;

	// Checks
	if (_newLabel != m_OnClickLabel && _newLabel != m_HoverLabel)
	{
		m_Label = _newLabel;
	}
	if (m_OnClickLabel == oldLabel && _newLabel != m_HoverLabel)
	{
		m_OnClickLabel = _newLabel;
	}
	if (m_HoverLabel == oldLabel && _newLabel != oldClickLabel)
	{
		m_HoverLabel = _newLabel;
	}

	// Update Label
	m_tLabel.setString(_newLabel);

	// Set Origin
	m_tLabel.setOrigin(sf::Vector2f(m_tLabel.getGlobalBounds().width / 2, m_tLabel.getGlobalBounds().height / 2));
	Sprite.setOrigin(sf::Vector2f(Sprite.getGlobalBounds().width / 2, Sprite.getGlobalBounds().height / 2));
}

void CButtons::SetLabel(std::string _newLabel, unsigned int _fontSize)
{
	// Temp Creation
	std::string oldLabel = m_Label;
	std::string oldClickLabel = m_OnClickLabel;

	// Checks
	if (_newLabel != m_OnClickLabel && _newLabel != m_HoverLabel)
	{
		m_Label = _newLabel;
	}
	if (m_OnClickLabel == oldLabel && _newLabel != m_HoverLabel)
	{
		m_OnClickLabel = _newLabel;
	}
	if (m_HoverLabel == oldLabel && _newLabel != oldClickLabel)
	{
		m_HoverLabel = _newLabel;
	}

	// Update Label
	m_tLabel.setString(_newLabel);
	m_tLabel.setCharacterSize(_fontSize);

	// Set Origin
	m_tLabel.setOrigin(sf::Vector2f(m_tLabel.getGlobalBounds().width / 2, m_tLabel.getGlobalBounds().height / 2));

}

/// <summary>
/// Sets The State Of The Button To Sepecified Parameter.
/// Takes In: std::string (type).
/// </summary>
/// <param name="type"></param>
void CButtons::SetState(std::string _type)
{
	if (_type == "Idle")
	{
		if (m_IdleTexture != nullptr)
		{
			Sprite.setTexture(*m_IdleTexture, 1);
		}
	}
	else if (_type == "Hover")
	{
		if (m_HoverTexture != nullptr)
		{
			Sprite.setTexture(*m_HoverTexture, 1);
		}
	}
	else if (_type == "Click")
	{
		if (m_ClickTexture != nullptr)
		{
			Sprite.setTexture(*m_ClickTexture, 1);
		}
	}
}

/// <summary>
/// Checks If The sf::Vector2f (_vMousePosition) Is Inside Of The Buttons GlobalBounds.
/// Takes In: sf::Vector2f (_vMousePosition)
/// </summary>
/// <param name="_vMousePosition"></param>
/// <returns></returns>
bool CButtons::bIsinBounds(sf::Vector2f _vMousePosition)
{
	// If Mouse Is In Sprite
	if (Sprite.getGlobalBounds().contains(sf::Vector2f(_vMousePosition.x, _vMousePosition.y)))
	{
		m_bIsHovering = true;

		// Checks For Pressed
		if (m_bIsPressed == false) 
		{
			if (m_HoverTexture != nullptr)
			{
				Sprite.setTexture(*m_HoverTexture, 1);
			}

			SetLabel(m_HoverLabel);

			SetState("Hover");
		}
		if (m_bIsPressed == true)
		{
			if (m_ClickTexture != nullptr)
			{
				Sprite.setTexture(*m_ClickTexture, 1);
			}

			SetLabel(m_Label);

			SetState("Click");
		}

		/*std::cout << "MousedOverButton" << std::endl;*/

		return true;
	}
	// Mouse Is Not In Sprite
	else
	{
		m_bIsHovering = false;

		// Check For Pressed
		if (m_bIsPressed) 
		{
			SetLabel(m_Label);

			SetState("Click");
		}
		else if (!m_bIsPressed)
		{
			if (m_IdleTexture != nullptr)
			{
				Sprite.setTexture(*m_IdleTexture, 1);
			}

			SetLabel(m_Label);

			SetState("Idle");
		}
		
		return false;
	}
	
	return false;
}

/// <summary>
/// Assigns m_HoverTexture to Specified Parameter.
/// Takes In: sf::Texture (_newTexture)
/// </summary>
/// <param name="_newTexture"></param>
void CButtons::SetHoverTex(sf::Texture* _newTexture)
{
	NumptyBehavior::DeletePointer(m_HoverTexture);
	m_HoverTexture = nullptr;
	m_HoverTexture = _newTexture;
	m_HasChangedHover = true;
}

/// <summary>
/// Assigns m_IdleTexture to Specified Parameter.
/// Takes In: sf::Texture (_newTexture)
/// </summary>
/// <param name="_newTexture"></param>
void CButtons::SetIdleTex(sf::Texture* _newTexture)
{
	NumptyBehavior::DeletePointer(m_IdleTexture);
	m_IdleTexture = nullptr;
	m_IdleTexture = _newTexture;
	m_HasChangedIdle = true;
}

/// <summary>
/// Assigns m_ClickTexture to Specified Parameter.
/// Takes In: sf::Texture (_newTexture)
/// </summary>
/// <param name="_newTexture"></param>
void CButtons::SetClickTex(sf::Texture* _newTexture)
{
	NumptyBehavior::DeletePointer(m_ClickTexture);
	m_ClickTexture = nullptr;
	m_ClickTexture = _newTexture;
	m_HasChangedClick = true;
}

void CButtons::SetFontSize(unsigned int _size)
{

	m_tLabel.setCharacterSize(_size);
}

/// <summary>
/// Returns The Height Of The Buttons Sprite.
/// </summary>
/// <returns></returns>
int CButtons::GetHeight()
{
	return Sprite.getGlobalBounds().height;
}

/// <summary>
/// Returns The Width Of The Buttons Sprite.
/// </summary>
/// <returns></returns>
int CButtons::GetWidth()
{
	return Sprite.getGlobalBounds().width;
}