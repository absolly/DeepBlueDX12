#include "SoundManager.hpp"
#include <iostream>
#include <SFML/Audio.hpp>
#include "mge/config.hpp"
#include <map>
#include <string>
#include <vector>
//#include "mge\core\SoundManager.hpp"

SoundManager* SoundManager::_instance;

SoundManager::SoundManager()
{
	_instance = this;
	std::cout << "---- sound incoming ---" << std::endl;
	/*
	if (!music.openFromFile(Config::MGE_SOUNDS_PATH + "full_soundtrack.wav"))
		return;
	music.setVolume(30);
	music.setLoop(true);
	music.play();	
	*/
	PlayMusic(Config::MGE_SOUNDS_PATH + "full_soundtrack.wav", 10, true);
	PlayMusic(Config::MGE_SOUNDS_PATH + "ambient_underwater.wav", 10, true);
	SetupFiles();
	SetupSounds();
}

void SoundManager::SetupFiles()
{
	fileNames = new std::vector<std::string>();

	std::string path = "mge/sounds/";

	for (int i = 0; i < 6; i++)
	{
		fileNames->push_back(path + "air_in_" + std::to_string((i + 1)));
	}

	for (int i = 0; i < 6; i++)
	{
		fileNames->push_back(path + "air_out_panic_" + std::to_string((i + 1)));
	}

	for (int i = 0; i < 6; i++)
	{
		fileNames->push_back(path + "air_out_relaxed_" + std::to_string((i + 1)));
	}

	fileNames->push_back(path + "ambient_underwater");
	fileNames->push_back(path + "environment_sonar");
	fileNames->push_back(path + "environment_whale");
	fileNames->push_back(path + "suffocating");
	fileNames->push_back(path + "ability");
	fileNames->push_back(path + "Alright its a beautiful day");
	fileNames->push_back(path + "But what is this");
	fileNames->push_back(path + "Ive deciphered");

	
}

void SoundManager::PlayMusic(std::string pSoundName, float volume, bool loop)
{
	sf::Music& music = *new sf::Music();
	if (!music.openFromFile(pSoundName))
		return;
	music.setVolume(volume);
	music.setLoop(loop);
	music.play();
}

void SoundManager::PlaySound(std::string pSoundBufferName, std::string pSoundChannel, bool pLoop, bool pInterrupt, bool repeatedSong, int pVolume)
{
	if (sounds[pSoundChannel].getStatus() == sf::Sound::Status::Stopped || !repeatedSong)
	{
		std::string path = "mge/sounds/";

		if (!sounds.count(pSoundChannel))
		{
			sf::Sound sound;
			sounds[pSoundChannel] = sound;
		}

		if (sounds[pSoundChannel].getStatus() == sf::Sound::Status::Playing && pInterrupt || sounds[pSoundChannel].getStatus() == sf::Sound::Status::Stopped)
		{
			sounds[pSoundChannel].setBuffer(soundBuffers[path + pSoundBufferName]);
			sounds[pSoundChannel].setLoop(pLoop);
			sounds[pSoundChannel].setVolume(pVolume);
			sounds[pSoundChannel].play();
		}
	}
}

bool SoundManager::GetChannelState(std::string pChannelName)
{
	if (sounds[pChannelName].getStatus() == sf::Sound::Status::Playing)
		return 1;

	return 0;
}

void SoundManager::SetupSounds()
{
	////soundBuffers = new std::map<std::string, sf::SoundBuffer>();
	//sf::SoundBuffer soundBuffer;
	//sf::Sound sound;

	//if (!soundBuffer.loadFromFile("mge/sounds/ambient_underwater.wav"))
	//	std::cout << "cant find shit" << std::endl;

	//soundBuffers["ambient_underwater"] = soundBuffer;
	////sounds["ambient"]
	//sounds["ambient"] = sound;

	//sounds["ambient"].setBuffer(soundBuffers["ambient_underwater"]);

	//sounds["ambient"].setVolume(100);
	//sounds["ambient"].setLoop(true);
	//sounds["ambient"].play();

	//sf::SoundBuffer soundBuffer2;

	//if (!soundBuffer2.loadFromFile("mge/sounds/environment_whale.wav"))
	//	std::cout << "cant find shit" << std::endl;

	//soundBuffers["environment_whale"] = soundBuffer2;

	//sounds["ambient"].setBuffer(soundBuffers["environment_whale"]);
	//sounds["ambient"].setLoop(true);

	//sounds["ambient"].play();

	for (int i = 0; i < fileNames->size(); i++)
	{
		sf::SoundBuffer soundBuffer;
		sf::Sound sound;

		std::string filePath = fileNames->at(i);

		if (!soundBuffer.loadFromFile(filePath + ".wav"))
		{ 
			std::cout << std::endl << "[ERROR] Unable to find " << filePath << ".wav" << std::endl << std::endl;
			continue;
		}

		soundBuffers[filePath] = soundBuffer;
		//if (filePath == "mge/sounds/ambient_underwater")
		//{
		//	std::cout << "found the sound!!" << std::endl;
		//	sounds["ambient"] = sound;
		//	sounds["ambient"].setBuffer(soundBuffers[filePath]);
		//	sounds["ambient"].setVolume(100.0f);
		//	sounds["ambient"].setLoop(true);
		//	sounds["ambient"].play();
		//}
	}
}


SoundManager::~SoundManager()
{
}

SoundManager * SoundManager::getInstance()
{
	return _instance;
}
