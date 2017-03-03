#pragma once

#include "mge\behaviours\AbstractBehaviour.hpp"
#include <string>

class Player;

class PlayerBreathingBehaviour : public AbstractBehaviour
{
public:
	PlayerBreathingBehaviour(Player& player);
	~PlayerBreathingBehaviour();

	virtual void update(float deltaTime);

private:
	Player& _player;

	void playBreatheSound();

	float getMinimumBreatheDelay();
	float getMaximumBreatheDelay();
	float _breatheDelay = 0;
	int _breatheInVolume = 100;
	int _breatheOutVolume = 50;
	float _minimumBreatheDelayWhenNotAffraid = 2.0f;
	float _maximumBreatheDelayWhenNotAffraid = 2.5f;
	float _minimumBreatheDelayWhenAffraid = 0.5f;
	float _maximumBreatheDelayWhenAffraid = 0.7f;

	void updateVariablesFromConfig();

	std::string getSoundToPlay();

	bool _breathingIn = true;
};
