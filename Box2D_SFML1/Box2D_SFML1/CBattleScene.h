#pragma once

#include "IScene.h"

class CBattleScene : public IScene
{
public:
	CBattleScene(sf::RenderWindow* _renderWindow, TextureMaster* _textureMaster, sf::Event& _event, ICubemon::CUBEMONTYPE  _enemyType1 = ICubemon::CUBEMONTYPE::UNASSIGNED, ICubemon::CUBEMONTYPE  _enemyType2 = ICubemon::CUBEMONTYPE::UNASSIGNED, ICubemon::CUBEMONTYPE  _enemyType3 = ICubemon::CUBEMONTYPE::UNASSIGNED, bool _custom = false);
	virtual ~CBattleScene();

	virtual void Start()override;
	virtual void Update()override;
	virtual void PolledUpdate() override;
	virtual void Render() override;
	virtual void CheckForMARKASDESTROY()override;

	void InitCubemon(ICubemon::CUBEMONTYPE _friendly = ICubemon::CUBEMONTYPE::UNASSIGNED, ICubemon::CUBEMONTYPE _enemy = ICubemon::CUBEMONTYPE::UNASSIGNED);
	int GrabCubemonLevelBasedOnType();

	int GrabCubemonHealthBasedOnType();

	int GrabCubemonEXPBasedOnType();

	static int GrabCubemonHealthBasedOnType(int _type);
	static std::vector<int> GrabCubemonHealth();
	static std::vector<int> GrabCubemonTypes();
private:
	void CreateAudioManager();
	void CreateGUI();

	virtual void CleanupAllPointers()override;

	void InitUIView();
	void InitWorldView();
	void InitBackground();

	void SaveCubemonValues();

	bool IsPlayerDeath();
	void ResetPlayerPosition();

	void CreateNewlyChangedPokemon(int _newType);
	void ChangePokemon(int _newType);

	void InitPlayerCubemonFromINI();

	void InitCustomBattleTypeOne();
	void InitCustomBattleTypeTwo();
	void InitCustomBattleTypeThree();

	sf::Event* m_Event = nullptr;
	sf::View m_UIView;
	sf::View m_WorldView;

	ICubemon* m_EnemyCubemon = nullptr;
	ICubemon* m_FriendlyCubemon = nullptr;

	sf::RectangleShape m_Background;
	sf::Texture m_BackgroundTex;

	ICubemon::CUBEMONTYPE m_PreDefEnemyType1 = ICubemon::CUBEMONTYPE::UNASSIGNED;
	ICubemon::CUBEMONTYPE m_PreDefEnemyType2 = ICubemon::CUBEMONTYPE::UNASSIGNED;
	ICubemon::CUBEMONTYPE m_PreDefEnemyType3 = ICubemon::CUBEMONTYPE::UNASSIGNED;
	bool m_bCustom = false;

	std::vector<ICubemon*> m_EnemyCubemonVector{};
};

