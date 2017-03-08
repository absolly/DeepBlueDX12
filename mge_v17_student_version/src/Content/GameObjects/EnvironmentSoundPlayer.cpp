#include "EnvironmentSoundPlayer.h"
#include "mge\core\Random.h"

EnvironmentSoundPlayer::EnvironmentSoundPlayer(SoundManager& soundManager) :
	_soundManager(soundManager)
{
	_randomSoundTimer = Random::Range(45.0f, 120.0f);
}

EnvironmentSoundPlayer::~EnvironmentSoundPlayer()
{
}

void EnvironmentSoundPlayer::update(float deltaTime)
{
	GameObject::update(deltaTime);

	_randomSoundTimer -= deltaTime;
	if (_randomSoundTimer <= 0)
	{
		int soundIndex = Random::Range(0, _sounds.size());
		std::cout << soundIndex << std::endl;
		_soundManager.PlaySound(_sounds[soundIndex], "Environment sound", false, true, false, 100, "");
		_randomSoundTimer = Random::Range(45.0f, 120.0f);
	}
}
