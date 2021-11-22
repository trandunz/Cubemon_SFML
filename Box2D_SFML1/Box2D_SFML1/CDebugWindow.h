#pragma once
#include "NumptyBehavior.h"

#include "CButtons.h"
#include "CInputField.h"
#include "Player.h"

class CDebugWindow : public NumptyBehavior
{
public:
	CDebugWindow(sf::Event* _event, Player* _player = nullptr);
	~CDebugWindow();

	virtual void Start();
	virtual void Update();
	virtual void PolledUpdate();
	virtual void Render();

	void InitBackGround();
	void InitWindow(sf::Uint32 _style);

private:
	bool IsSceneGame();
	bool IsSceneBattle();

	void HandleGameButtons();
	void HandleMiscButtons();
	void HandleBattleButtons();

	void HealAllPokemon();
	void AddRandomPokemonToParty();
	void AddThallicToParty();
	void AddKindlingToParty();
	void AddBrutusToParty();
	void LevelUpFirstCubemon();

	void ForceBattleScene();
	void ForceMedicalScene();
	void ForceMainMenu();
	void ForceGameScene();

	sf::RenderWindow* m_RenderWindow = nullptr;
	sf::Event* m_Event = nullptr;
	Player* m_Player = nullptr;

	std::vector<CButtons*> m_InGameButtons;
	std::vector<CButtons*> m_MiscButtons;
	std::vector<CButtons*> m_BattleButtons;
	std::vector<sf::RectangleShape> m_Shapes;
};

