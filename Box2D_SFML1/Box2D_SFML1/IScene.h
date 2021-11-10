#pragma once

#include "WorldManager.h"
#include "GUI.h"

class IScene : public NumptyBehavior
{
public:
	virtual ~IScene() {}

	virtual void Start() {}
	virtual void Update() {}
	virtual void PolledUpdate() {}
	virtual void Render() {}

	virtual void CheckForMARKASDESTROY() {}

protected:
	virtual void CleanupAllPointers();

	sf::RenderWindow* m_RenderWindow = nullptr;
	WorldManager* m_WorldManager = nullptr;
	GUI* m_GUI = nullptr;
	AudioManager* m_AudioManager = nullptr;
	TextureMaster* m_TextureMaster = nullptr;

	sf::Vector2f m_MousePos;
	sf::Font m_Font;
	const b2Vec2 m_Gravity = b2Vec2(0.0f, 10.0f);
};

