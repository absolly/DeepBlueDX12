#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include "Content\Core\EventHandler.h"
#include "glm.hpp"

class Config 
{
public:
	static std::string MGE_MODEL_PATH;// = "mge/models/";
	static std::string MGE_TEXTURE_PATH; //= "mge/textures/";
	static std::string MGE_SHADER_PATH;// = "mge/shaders/";
	static std::string MGE_FONT_PATH;// = "mge/fonts/";
	static std::string MGE_LEVEL_PATH;// = "mge/levels/";
	static std::string MGE_SETTINGS_PATH;// = "mge/settings/";
	static Event<bool> onConfigUpdated;

	static std::string tryGetValue(std::string key, std::string defaultValue);
	static void updateValue(std::string key, std::string& defaultValue);
	static void updateValue(std::string key, float& defaultValue);
	static void updateValue(std::string key, bool& defaultValue);
	static void updateValue(std::string key, double& defaultValue);
	static void updateValue(std::string key, int& defaultValue);
	static void updateValue(std::string key, glm::vec3 & defaultValue);
	static void updateValue(std::string key, glm::vec4 & defaultValue);


private:
	static void updateFromConfig();
	static std::unordered_map<std::string, std::string> _loadedConfigVaraibles;

	static bool _configUpdated;


	static class _init
	{
	public:
		_init() {
			updateFromConfig();
			EventHandler::bindKeyDownEvent(sf::Keyboard::F5, this, &_init::onRefreshPressedEvent);
		}
		void onRefreshPressedEvent(sf::Event::KeyEvent& event) { updateFromConfig(); }
	} _initializer;
};