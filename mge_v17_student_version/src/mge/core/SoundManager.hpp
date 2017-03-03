#pragma once
#include <SFML/Audio.hpp>
#include <map>
#include <string>
#include <vector>

class SoundManager
{
public:
	SoundManager();
	bool GetChannelState(std::string pChannelName);
	~SoundManager();
	void PlaySound(std::string pSoundBufferName, std::string pSoundChannel, bool pLoop, bool pInterrupt, bool repeatedSong, int pVolume = 100);

	static SoundManager* getInstance();
private:
	static SoundManager* _instance;

	sf::Music music;
	void SetupFiles();
	std::vector<std::string> * fileNames;
	std::map<std::string, sf::SoundBuffer> soundBuffers;
	std::map<std::string, sf::Sound> sounds;
	void SetupSounds();
	void PlayMusic(std::string pSoundName, float volume, bool loop);
};

