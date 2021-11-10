#pragma once

#define ZOOM 1

#include "IScene.h"
#include "CContactListener.h";

class CGameScene : public IScene
{
public:
	CGameScene(sf::RenderWindow* _renderWindow, TextureMaster* _textureMaster, sf::Event& _event);
	virtual ~CGameScene();

	void Start() override;
	void Update() override;
	void PolledUpdate() override;
	void Render() override;

	virtual void CheckForMARKASDESTROY();

private:
	void CreateB2World();
	void CreateWorldManager();
	void CreatePlayer();
	void CreateAudioManager();
	void CreateGUI();

	void PlayMusic();

	void InitGameOver();
	void GameOverScreen();

	void CenterViewTo(sf::Sprite* _object);
	void InitUIView();
	void InitView();

	virtual void CleanupAllPointers();

	b2World* m_World = nullptr;
	Player* m_Player = nullptr;
	sf::Event* m_Event = nullptr;
	sf::Clock m_DeathTimer;

	// Gameover screen variables
	float m_PlayerRespawnTime = 6;
	sf::Clock m_FadeTimer;
	sf::RectangleShape m_FadeScreen;
	sf::Text m_GameOverText;

	// Main View / Camera
	sf::View m_View;
	sf::View m_UIView;

	CContactListener* m_ContactListener = nullptr;
};

