#include "SoundManager.hpp"
#include <iostream>
#include <SFML/Audio.hpp>
#include "mge/config.hpp"
#include <map>
#include <string>
#include <vector>

SoundManager::SoundManager()
{
	std::cout << "---- sound incoming ---" << std::endl;

	////std::string path = Config::MGE_SOUNDS_PATH + "full_soundtrack.wav";
	if (!music.openFromFile("mge/sounds/full_soundtrack.wav"))
		return;

	//std::cout << "hey link hey listen" << std::endl;
	music.setVolume(30);
	
	music.setLoop(true);
	//music.play();	

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

	
}

void SoundManager::PlaySound(std::string pSoundBufferName, std::string pSoundChannel, bool pLoop, bool pInterrupt, int pVolume)
{
	std::cout << "-----------play sound---------" << std::endl;
	std::cout << pSoundBufferName << std::endl;
	std::cout << pSoundChannel << std::endl;
	//if (!sounds.count(pSoundChannel))
	//{
	//	sf::Sound sound;
	//	sounds[pSoundChannel] = sound;
	//}

	//if (sounds[pSoundChannel].getStatus() == sf::Sound::Status::Playing && pInterrupt || sounds[pSoundChannel].getStatus() == sf::Sound::Status::Stopped)
	//{
	//	sounds[pSoundChannel].setBuffer(soundBuffers[pSoundBufferName]);
	//	sounds[pSoundChannel].setLoop(pLoop);
	//	sounds[pSoundChannel].setVolume(pVolume);
	//	sounds[pSoundChannel].play();
	//}
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
			std::cout << "unable to find " << filePath << ".wav" << std::endl;
			continue;
		}

		soundBuffers[filePath] = soundBuffer;
		if(filePath == "")
	}
}


SoundManager::~SoundManager()
{
}
