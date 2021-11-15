#include "ICubemon.h"

int ICubemon::TakeDamage(int _amount)
{
	if (m_DamageTimer.getElapsedTime().asSeconds() >= 0.1f)
	{
		m_AudioManager->PlayDamage();
		for (int i = 0; i < _amount; i++)
		{
			if (m_CurrentHealth >= 1)
			{
				m_CurrentHealth--;
			}
			else
			{
				m_MARKASDESTROY = true;
				break;
			}
		}

		m_DamageTimer.restart();
		return _amount;
	}
	else
	{
		return 0;
	}
}

int ICubemon::Heal(int _amount)
{
	if (m_DamageTimer.getElapsedTime().asSeconds() >= 0.1f)
	{
		for (int i = 0; i < _amount; i++)
		{
			if (m_CurrentHealth < m_MaxHealth && m_CurrentHealth > 0)
			{
				m_CurrentHealth++;
			}
			else
			{
				break;
			}
		}

		m_DamageTimer.restart();
		return _amount;
	}
	else
	{
		return 0;
	}
}

void ICubemon::SetSpritePos(sf::Vector2f _pos)
{
	m_Shape->setPosition(_pos);
}

void ICubemon::SetSpriteScale(sf::Vector2f _scale)
{
	m_Shape->setScale(_scale);
	//SetOriginToCenter(*m_Shape);
}

void ICubemon::LevelUp()
{
	if (m_Lvl < 99)
	{
		m_Lvl++;
	}
}

void ICubemon::SetLevel(int _value)
{
	m_Lvl = _value;
}

AudioManager* ICubemon::GetAudioManager()
{
	return m_AudioManager;
}

float ICubemon::GetWeakAttack()
{
	return m_Attack1;
}

float ICubemon::GetStrongAttack()
{
	return m_Attack2;
}
