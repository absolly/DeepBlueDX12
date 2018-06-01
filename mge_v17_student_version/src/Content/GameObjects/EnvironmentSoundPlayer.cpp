#include "EnvironmentSoundPlayer.h"
#include "mge\core\Random.h"

EnvironmentSoundPlayer::EnvironmentSoundPlayer(SoundManager& soundManager) :
	_soundManager(soundManager)
{
	_randomSoundTimer = 5;
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
		int soundIndex;
		while ((soundIndex = Random::Range(0, _sounds.size() - 1)) == _previousSound && _sounds.size() > 1);
		_soundManager.PlayAudio(_sounds[soundIndex], 
								_sounds[soundIndex],
								false, 
								true, 
								false, 
								Random::Range(_soundVolumeRanges[soundIndex].x, _soundVolumeRanges[soundIndex].y), 
								"",
								Random::Range(_soundPitchRanges[soundIndex].x, _soundPitchRanges[soundIndex].y)
								);
		std::cout << "Playing sound: " << _sounds[soundIndex] << std::endl;
		_randomSoundTimer = Random::Range(10.0f, 15.0f);
		_previousSound = soundIndex;
	}
}
