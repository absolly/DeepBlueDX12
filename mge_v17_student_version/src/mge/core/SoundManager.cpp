#include "SoundManager.hpp"
#include <iostream>
#include <SFML/Audio.hpp>
#include "mge/config.hpp"

SoundManager::SoundManager()
{
	sf::Music music;
	//std::string path = Config::MGE_SOUNDS_PATH + "full_soundtrack.wav";
	//if (!music.openFromFile(path))
	//	return;

	music.play();
	
}


SoundManager::~SoundManager()
{
}
