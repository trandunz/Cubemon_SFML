#pragma once
#include "IScene.h"
#include "CMainMenu.h"

class CMainMenuScene : public IScene
{
public:
	CMainMenuScene(sf::RenderWindow* _renderWindow, sf::Event& _event);
	virtual ~CMainMenuScene();

	void Start() override;
	void Update() override;
	void PolledUpdate() override;
	void Render() override;

private:
	virtual void CleanupAllPointers();

	void CreateMainMenu();
	void DestoryMainMenu();

	sf::Event* m_Event = nullptr;
	CMainMenu* m_MainMenu = nullptr;
};

