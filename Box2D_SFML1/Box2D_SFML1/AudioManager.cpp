#include "AudioManager.h"

AudioManager::AudioManager()
{
    srand((unsigned) time(NULL));

    CreatePointers();

    m_SlimeHurt1->loadFromFile("Resources/Audio/SlimeHit.wav");
    m_SlimeHurt2->loadFromFile("Resources/Audio/SlimeHit2.wav");

    m_PlayerDeathBuffer->loadFromFile("Resources/Audio/Death.wav");
    m_SlimeDeathBuffer->loadFromFile("Resources/Audio/SlimeDeath.wav");

    m_BowBuffer->loadFromFile("Resources/Audio/FireBow.wav");

    SlimeHurtBuffers[0] = *m_SlimeHurt1;
    SlimeHurtBuffers[1] = *m_SlimeHurt2;
}

AudioManager::~AudioManager()
{
    CleanupPointers();
}

void AudioManager::PlayerDeath()
{
    m_PlayerDeathSound->setBuffer(*m_PlayerDeathBuffer);
    m_PlayerDeathSound->setVolume(25.0f);
    m_PlayerDeathSound->play();
}

void AudioManager::FireBow()
{
    m_BowShotSound->setBuffer(*m_BowBuffer);
    m_BowShotSound->setVolume(25.0f);
    m_BowShotSound->play();
}

void AudioManager::SlimeDeath(float _volume)
{
    m_SlimeDeathSound->setBuffer(*m_SlimeDeathBuffer);
    m_SlimeDeathSound->setVolume(25.0f);
    m_SlimeDeathSound->play();
}

void AudioManager::SlimeDamage(float _volume)
{
    m_SlimeDamageSound->setBuffer(SlimeHurtBuffers[0]);
    m_SlimeDamageSound->setVolume(_volume);
    m_SlimeDamageSound->play();
}

void AudioManager::PlayMusic(int _track)
{
    if (_track == 0)
    {
        m_Music->stop();
        m_Music->openFromFile("Resources/Audio/PokemonVillage.wav");
        m_Music->setLoop(true);
        m_Music->setVolume(10.0f);
        m_Music->play();
    }
    else if (_track == -1)
    {
        m_Music->stop();
        m_Music->openFromFile("Resources/Audio/Encounter.wav");
        m_Music->setLoop(true);
        m_Music->setVolume(10.0f);
        m_Music->play();
    }
}

void AudioManager::CreatePointers()
{
    {
        m_Music = new sf::Music();
        m_SlimeDeathBuffer = new sf::SoundBuffer();
        m_SlimeHurt1 = new sf::SoundBuffer();
        m_SlimeHurt2 = new sf::SoundBuffer();

        SlimeHurtBuffers = new sf::SoundBuffer[2];

        m_SlimeDeathSound = new sf::Sound;
        m_SlimeDamageSound = new sf::Sound;

        m_PlayerDeathBuffer = new sf::SoundBuffer();
        m_PlayerDeathSound = new sf::Sound;

        m_BowBuffer = new sf::SoundBuffer();
        m_BowShotSound = new sf::Sound;
    }
}

void AudioManager::CleanupPointers()
{
    DeletePointer(m_Music);
    m_Music = nullptr;
    DeletePointer(m_SlimeDeathBuffer);
    m_SlimeDeathBuffer = nullptr;
    DeletePointer(m_SlimeHurt1);
    m_SlimeHurt1 = nullptr;
    DeletePointer(m_SlimeHurt2);
    m_SlimeHurt2 = nullptr;
    DeletePointer(m_SlimeDeathSound);
    m_SlimeDeathSound = nullptr;
    DeletePointer(m_SlimeDamageSound);
    m_SlimeDamageSound = nullptr;
    DeletePointer(m_PlayerDeathBuffer);
    m_PlayerDeathBuffer = nullptr;
    DeletePointer(m_PlayerDeathSound);
    m_PlayerDeathSound = nullptr;
    DeletePointer(m_BowBuffer);
    m_BowBuffer = nullptr;
    DeletePointer(m_BowShotSound);
    m_BowShotSound = nullptr;
    delete[] SlimeHurtBuffers;
    SlimeHurtBuffers = nullptr;
}
