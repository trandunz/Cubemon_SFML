#pragma once
#include "IEntity.h"
class ICubemon : public IEntity
{
public:
	enum class CUBEMONSTATE
	{
		UNASSIGNED,

		ASLEEP = 1,
		WORLD = 2,
		COMBAT = 3,
		DEAD = 4
	};

	enum class CUBEMONTYPE
	{
		UNASSIGNED,

		THALLIC = 1,
		KINDLING = 2,
		BRUTUS = 3,
		WIRLSON = 4,
		DUSTDEVIL = 5,
		BLIZZARDBIRD = 6,
	};

	virtual ~ICubemon() { };

	virtual void Start() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void Movement() = 0;
	virtual void Attack() = 0;

	virtual int TakeDamage(int _amount);
	inline void SetCurrentHealth(int _amount) { m_CurrentHealth = _amount; }
	inline int GetCurrentHealth() { return m_CurrentHealth; }
	virtual int Heal(int _amount);

	void SetSpritePos(sf::Vector2f _pos);
	void SetSpriteScale(sf::Vector2f _scale);

	std::string GetName() { return m_Name; }
	int GetLvl() { return m_Lvl; }

	void LevelUp();
	void SetLevel(int _value);

	AudioManager* GetAudioManager();

	float GetWeakAttack();
	float GetStrongAttack();

	inline float GetXP()
	{
		return m_EXP;
	}
	inline void SetXP(int _amount)
	{
		m_EXP = _amount;
	}
	inline void AddXP(float _amount)
	{
		m_EXP += _amount;
	}

	inline void EXPLvlUpCheck()
	{
		if (m_EXP >= (100 * m_Lvl))
		{
			m_EXP = 0;
			LevelUp();
		}
	}
	
	CUBEMONTYPE m_CubeType = CUBEMONTYPE::UNASSIGNED;

protected:

	CUBEMONSTATE m_CubeState = CUBEMONSTATE::UNASSIGNED;

	int m_CurrentHealth = 100;
	int m_MaxHealth = 100;

	sf::Texture* m_SpriteTexture = nullptr;

	std::string m_Name = "UNASSIGNED";
	int m_Lvl = 1;
	float m_EXP = 0;

	float m_Attack1 = 5;
	float m_Attack2 = 10;
private:
	sf::Clock m_DamageTimer;
};

