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
	int _previousSound = -1;

	std::vector<std::string> _sounds
	{
		"environment_sonar",
		"environment_whale",
		"Whale Call High (echo) 14",
		"Whale Call Low (echo) 9",
		"Whale Call Mid (echo) 8",
		"Whale Call-Answer Close (echo) 7"
	};
	std::vector<glm::vec2> _soundVolumeRanges
	{
		glm::vec2(90,100),
		glm::vec2(5,50),
		glm::vec2(5,75),
		glm::vec2(5,75),
		glm::vec2(5,75)
	};

	std::vector<glm::vec2> _soundPitchRanges
	{
		glm::vec2(0.95f, 1.05f),
		glm::vec2(0.9f, 1.1f),
		glm::vec2(0.9f, 1.1f),
		glm::vec2(0.9f, 1.1f),
		glm::vec2(0.9f, 1.1f)
	};
};