#include "Config.hpp"

#include <ctype.h>
#include <sstream>
#include <filesystem>

std::string Config::MGE_MODEL_PATH = "mge/models/";
std::string Config::MGE_TEXTURE_PATH = "mge/textures/";
#ifdef API_OPENGL
std::string Config::MGE_SHADER_PATH = "mge/shaders/glsl/";
#elif defined(API_DIRECTX)
std::wstring Config::MGE_SHADER_PATH = L"mge/shaders/hlsl/";
#endif // API_OPENGL
std::string Config::MGE_FONT_PATH = "mge/fonts/";
std::string Config::MGE_LEVEL_PATH = "mge/levels/";
std::string Config::MGE_SETTINGS_PATH = "mge/settings/";
std::string Config::MGE_SPRITES_PATH = "mge/sprites/";
std::string Config::MGE_SOUNDS_PATH = "mge/sounds/";
sf::Vector2f Config::HUD_SCALE_FACTOR;
bool Config::FULL_SCREEN = false;
bool Config::POST_FX = false;
sf::Vector2i Config::SCREEN_RESOLUTION = sf::Vector2i(false ? 1920 : 1600, false ? 1080 : 900);
EventNoArgs Config::onConfigUpdated;

std::unordered_map<std::string, std::string> Config::_loadedConfigVaraibles = std::unordered_map<std::string, std::string>();
Config::_init Config::_initializer;


void Config::updateFromConfig()
{
	std::cout << "------ UPDATING CONFIG ------" << std::endl;
	std::ifstream infile(MGE_SETTINGS_PATH + "Config.txt");
	if (!infile.is_open())
	{
		std::cout << "File not found" << std::endl;
	}
	std::string line;
	while (std::getline(infile, line))
	{
		std::istringstream iss(line);
		std::string key;
		std::string value;
		if (!(iss >> key) || key[0] == '[' || key[0] == '/')
		{
			std::cout << "----- " << key << " -----" << std::endl;
			continue;
		}
		getline(iss, value);
		value = value.substr(1);
		std::cout << key << " = " << value << std::endl;
		_loadedConfigVaraibles[key] = value;
	}
	infile.close();
	std::cout << "------ UPDATING CONFIG ------" << std::endl << std::endl;
	onConfigUpdated();
}

std::string Config::tryGetValue(std::string key, std::string defaultValue)
{
	if (_loadedConfigVaraibles.find(key) != _loadedConfigVaraibles.end())
		return _loadedConfigVaraibles[key];
	_loadedConfigVaraibles[key] = defaultValue;

	std::ofstream outfile(MGE_SETTINGS_PATH + "Config.txt", std::ios_base::app | std::ios_base::out);
	outfile << "\n" << key << " " << defaultValue;
	outfile.close();
	return defaultValue;
}

void Config::updateValue(std::string key, std::string& defaultValue)
{
	defaultValue = tryGetValue(key, defaultValue);
}
std::string Config::getConfigValue(std::string key, std::string defaultValue)
{
	updateValue(key, defaultValue);
	return defaultValue;
}
std::string Config::getConfigValue(std::string key, char defaultValue[])
{
	updateValue(key, std::string(defaultValue));
	return defaultValue;
}
void Config::updateValue(std::string key, float& defaultValue)
{
	defaultValue = std::stof(tryGetValue(key, std::to_string(defaultValue)));
}

float Config::getConfigValue(std::string key, float defaultValue)
{
	updateValue(key, defaultValue);
	return defaultValue;
}

void Config::updateValue(std::string key, bool& defaultValue)
{
	defaultValue = std::stoi(tryGetValue(key, std::to_string(defaultValue)));
}

bool Config::getConfigValue(std::string key, bool defaultValue)
{
	updateValue(key, defaultValue);
	return defaultValue;
}

void Config::updateValue(std::string key, double& defaultValue)
{
	defaultValue = std::stod(tryGetValue(key, std::to_string(defaultValue)));
}

double Config::getConfigValue(std::string key, double defaultValue)
{
	updateValue(key, defaultValue);
	return defaultValue;
}

void Config::updateValue(std::string key, int& defaultValue)
{
	defaultValue = std::stoi(tryGetValue(key, std::to_string(defaultValue)));
}
int Config::getConfigValue(std::string key, int defaultValue)
{
	updateValue(key, defaultValue);
	return defaultValue;
}
void Config::updateValue(std::string key, glm::vec3& defaultValue)
{
	std::string input = tryGetValue(key, glm::to_string(defaultValue));
	std::istringstream ss(input.substr(5));
	std::string token;
	std::getline(ss, token, ',');
	defaultValue.x = std::stof(token);
	std::getline(ss, token, ',');
	defaultValue.y = std::stof(token);
	std::getline(ss, token, ')');
	defaultValue.z = std::stof(token);
}
glm::vec3 Config::getConfigValue(std::string key, glm::vec3 defaultValue)
{
	updateValue(key, defaultValue);
	return defaultValue;
}
void Config::updateValue(std::string key, glm::vec4& defaultValue)
{
	std::string input = tryGetValue(key, glm::to_string(defaultValue));
	std::istringstream ss(input.substr(5));
	std::string token;
	std::getline(ss, token, ',');
	defaultValue.x = std::stof(token);
	std::getline(ss, token, ',');
	defaultValue.y = std::stof(token);
	std::getline(ss, token, ',');
	defaultValue.z = std::stof(token);
	std::getline(ss, token, ')');
	defaultValue.w = std::stof(token);
	
}

glm::vec4 Config::getConfigValue(std::string key, glm::vec4 defaultValue)
{
	updateValue(key, defaultValue);
	return defaultValue;
}
