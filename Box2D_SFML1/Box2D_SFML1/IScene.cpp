#include "IScene.h"

void IScene::CleanupAllPointers()
{
	m_RenderWindow = nullptr;
	DeletePointer(m_WorldManager);
	m_WorldManager = nullptr;
	DeletePointer(m_GUI);
	m_GUI = nullptr;
	DeletePointer(m_AudioManager);
	m_AudioManager = nullptr;
	m_TextureMaster = nullptr;
}
