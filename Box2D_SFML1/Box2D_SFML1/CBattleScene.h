#pragma once

#include "IScene.h"

class CBattleScene : public IScene
{
public:
	CBattleScene(sf::RenderWindow* _renderWindow, TextureMaster* _textureMaster, sf::Event& _event);
	virtual ~CBattleScene();

	virtual void Start()override;
	virtual void Update()override;
	virtual void PolledUpdate() override;
	virtual void Render() override;
	virtual void CheckForMARKASDESTROY()override;

	void InitCubemon(ICubemon::CUBEMONTYPE _friendly = ICubemon::CUBEMONTYPE::UNASSIGNED, ICubemon::CUBEMONTYPE _enemy = ICubemon::CUBEMONTYPE::UNASSIGNED);

private:
	void CreateAudioManager();
	void CreateGUI();

	virtual void CleanupAllPointers()override;

	void InitUIView();
	void InitWorldView();
	void InitBackground();

	void SaveCubemonValues();
	int GrabCubemonLevelBasedOnType();

	int GrabCubemonHealthBasedOnType();
	std::vector<int> GrabCubemonHealth();
	int GrabCubemonEXPBasedOnType();
	bool IsPlayerDeath();
	void ResetPlayerPosition();

	sf::Event* m_Event = nullptr;
	sf::View m_UIView;
	sf::View m_WorldView;

	ICubemon* m_EnemyCubemon = nullptr;
	ICubemon* m_FriendlyCubemon = nullptr;

	sf::RectangleShape m_Background;
	sf::Texture m_BackgroundTex;
};

