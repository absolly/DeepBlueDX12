#pragma once

#include "mge\core\GameObject.hpp"
#include "mge\core\SoundManager.hpp"

class EnvironmentSoundPlayer : public GameObject
{
public:
	EnvironmentSoundPlayer(SoundManager& soundManager);
	~EnvironmentSoundPlayer();

	virtual void update(float deltaTime);

private:
	SoundManager& _soundManager;
	float _randomSoundTimer = 0;

	std::vector<std::string> _sounds
	{
		"environment_sonar",
		"environment_whale"
	};
	std::vector<int> _soundVolumes
	{
		100,
		40
	};
};