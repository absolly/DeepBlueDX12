#include "PlayerBreathingBehaviour.h"

#include "Content\GameObjects\Player.h"
#include "mge\core\SoundManager.hpp"
#include "mge\core\Random.h"
#include "mge\config.hpp"
#include "Content\Hud\Hud.hpp"

PlayerBreathingBehaviour::PlayerBreathingBehaviour(Player& player) :
	_player(player)
{
	updateVariablesFromConfig();
	Config::onConfigUpdated.bind(this, &PlayerBreathingBehaviour::updateVariablesFromConfig);
}

PlayerBreathingBehaviour::~PlayerBreathingBehaviour()
{
}

void PlayerBreathingBehaviour::update(float deltaTime)
{
	if (_breatheDelay <= 0)
	{
		if (!Hud::getInstance()->getNoOxygenLeft())
			playBreatheSound();
		_breatheDelay = Random::Range(getMinimumBreatheDelay(), getMaximumBreatheDelay());
		_breathingIn = !_breathingIn;
	}
	else
	{
		_breatheDelay -= deltaTime;
	}
}

void PlayerBreathingBehaviour::playBreatheSound()
{
	std::string soundToPlay = getSoundToPlay();
	std::string soundChannel = getSoundToPlay();
	bool interupt = true;
	int volume = _breathingIn ? _breatheInVolume : _breatheOutVolume;
	SoundManager::getInstance()->PlaySound(getSoundToPlay(), soundChannel, false, interupt, false, volume, "", Random::Range(0.8f, 1.2f));

}

float PlayerBreathingBehaviour::getMinimumBreatheDelay()
{
	float minimumBreatheDelayDifference = _minimumBreatheDelayWhenNotAffraid - _minimumBreatheDelayWhenAffraid;
	return _minimumBreatheDelayWhenNotAffraid - (_player.getAffraidness() * 0.01) * minimumBreatheDelayDifference;
}

float PlayerBreathingBehaviour::getMaximumBreatheDelay()
{
	float minimumBreatheDelayDifference = _maximumBreatheDelayWhenNotAffraid - _maximumBreatheDelayWhenAffraid;
	return _maximumBreatheDelayWhenNotAffraid - (_player.getAffraidness() * 0.01) * minimumBreatheDelayDifference;
}

void PlayerBreathingBehaviour::updateVariablesFromConfig()
{
	Config::updateValue("_minimumBreatheDelayWhenNotAffraid", _minimumBreatheDelayWhenNotAffraid);
	Config::updateValue("_maximumBreatheDelayWhenNotAffraid", _maximumBreatheDelayWhenNotAffraid);
	Config::updateValue("_minimumBreatheDelayWhenAffraid", _minimumBreatheDelayWhenAffraid);
	Config::updateValue("_maximumBreatheDelayWhenAffraid", _maximumBreatheDelayWhenAffraid);

	Config::updateValue("_breatheInVolume", _breatheInVolume);
	Config::updateValue("_breatheOutVolume", _breatheOutVolume);
}

std::string PlayerBreathingBehaviour::getSoundToPlay()
{
	std::string soundToPlay;
	if (_breathingIn)
		soundToPlay = _player.getAffraidness() > 50 ? "air_in_panic_" : "air_in_relaxed_";
	else
		soundToPlay = _player.getAffraidness() > 50 ? "air_out_panic_" : "air_out_relaxed_";

	int fileIndex = Random::Range(1, 6);
	soundToPlay += std::to_string(fileIndex);
	return soundToPlay;
}
