#pragma once
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers.
#endif
#include <string>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include "Content\Core\EventHandler.h"
#include "Content\Core\EventNoArgs.h"
#include "glm.hpp"
#ifdef API_DIRECTX
#include <windows.h>
#endif
class Config 
{
public:
	static std::string MGE_MODEL_PATH;// = "mge/models/";
	static std::string MGE_TEXTURE_PATH; //= "mge/textures/";
#ifdef API_OPENGL
	static std::string MGE_SHADER_PATH;// = "mge/shaders/";
#elif defined(API_DIRECTX)
	static std::wstring MGE_SHADER_PATH;// = "mge/shaders/";
#endif // API_OPENGL
	static std::string MGE_FONT_PATH;// = "mge/fonts/";
	static std::string MGE_LEVEL_PATH;// = "mge/levels/";
	static std::string MGE_SOUNDS_PATH;// = "mge/sounds/";
	static std::string MGE_SETTINGS_PATH;// = "mge/settings/";
	static std::string MGE_SPRITES_PATH;// = "mge/sprites/";
	static sf::Vector2f HUD_SCALE_FACTOR;
	static sf::Vector2i SCREEN_RESOLUTION;
	static bool FULL_SCREEN;
	static bool POST_FX;
	static EventNoArgs onConfigUpdated;

	static std::string tryGetValue(std::string key, std::string defaultValue);

	static void updateValue(std::string key, std::string& defaultValue);
	static std::string getConfigValue(std::string key, std::string defaultValue);
	static std::string getConfigValue(std::string key, char defaultValue[]);

	static void updateValue(std::string key, float& defaultValue);
	static float getConfigValue(std::string key, float defaultValue);

	static void updateValue(std::string key, bool& defaultValue);
	static bool getConfigValue(std::string key, bool defaultValue);

	static void updateValue(std::string key, double& defaultValue);
	static double getConfigValue(std::string key, double defaultValue);

	static void updateValue(std::string key, int& defaultValue);
	static int getConfigValue(std::string key, int defaultValue);

	static void updateValue(std::string key, glm::vec3& defaultValue);
	static glm::vec3 getConfigValue(std::string key, glm::vec3 defaultValue);

	static void updateValue(std::string key, glm::vec4& defaultValue);
	static glm::vec4 getConfigValue(std::string key, glm::vec4 defaultValue);


private:
	static void updateFromConfig();
	static std::unordered_map<std::string, std::string> _loadedConfigVaraibles;

	static bool _configUpdated;


	static class _init
	{
	public:
		_init() {
			updateFromConfig();
			HUD_SCALE_FACTOR = sf::Vector2f(SCREEN_RESOLUTION.x / 1920.0f, SCREEN_RESOLUTION.y / 1080.0f);
			EventHandler::bindKeyDownEvent(sf::Keyboard::F5, this, &_init::onRefreshPressedEvent);
		}
		void onRefreshPressedEvent(sf::Event::KeyEvent& event) { updateFromConfig(); }
	} _initializer;
};