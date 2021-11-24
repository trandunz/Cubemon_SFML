#pragma once
#include "NumptyBehavior.h"

#include "CButtons.h"
#include "CInputField.h"
#include "Player.h"
#include "GUI.h"

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

	void CreateIngameButtons();
	void CreateButtons();
	void CreateMiscButttons();
	void CreateBattleButtons();

	sf::RenderWindow* m_RenderWindow = nullptr;
	sf::Event* m_Event = nullptr;
	Player* m_Player = nullptr;

	std::vector<CButtons*> m_InGameButtons;
	std::vector<CButtons*> m_MiscButtons;
	std::vector<CButtons*> m_BattleButtons;
	std::vector<sf::RectangleShape> m_Shapes;

	void InitButtonTextures();
	sf::Texture m_BattleTexture;
	sf::Texture m_BattleHoverTexture;
	sf::Texture m_GameTexture;
	sf::Texture m_GameHoverTexture;
	sf::Texture m_HealTexture;
	sf::Texture m_HealHoverTexture;
	sf::Texture m_HealthTexture;
	sf::Texture m_HealthHoverTexture;
	sf::Texture m_MainMenuTexture;
	sf::Texture m_MainMenuHoverTexture;
	sf::Texture m_UpgradeTexture;
	sf::Texture m_UpgradeHoverTexture;
};

