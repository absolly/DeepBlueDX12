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
		_soundManager.PlaySound(_sounds[soundIndex], "Environment sound", false, true, false, Random::Range(_soundVolumes[soundIndex]-10, _soundVolumes[soundIndex]), "");
		_randomSoundTimer = Random::Range(45.0f, 120.0f);
	}
}
