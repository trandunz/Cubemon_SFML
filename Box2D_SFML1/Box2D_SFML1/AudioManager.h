#pragma once
#ifndef _AUDIOMANAGER_H__
#define _AUDIOMANAGER_H__

#include "NumptyBehavior.h"

class AudioManager : public NumptyBehavior
{
public:

	AudioManager();
	virtual ~AudioManager();

	void PlayerDeath();
	void FireBow();
	void SlimeDeath(float _volume);
	void SlimeDamage(float _volume);
	void PlayMusic(int _track = 0);

private:
	void CreatePointers();
	void CleanupPointers();

	sf::Music* m_Music = nullptr;
	sf::SoundBuffer* m_SlimeDeathBuffer = nullptr;
	sf::SoundBuffer* m_SlimeHurt1 = nullptr;
	sf::SoundBuffer* m_SlimeHurt2 = nullptr;
	sf::SoundBuffer* SlimeHurtBuffers = nullptr;
	sf::Sound* m_SlimeDeathSound = nullptr;
	sf::Sound* m_SlimeDamageSound = nullptr;

	sf::SoundBuffer* m_PlayerDeathBuffer = nullptr;
	sf::Sound* m_PlayerDeathSound = nullptr;


	sf::SoundBuffer* m_BowBuffer = nullptr;
	sf::Sound* m_BowShotSound = nullptr;
};
#endif