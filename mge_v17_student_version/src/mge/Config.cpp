#include "Config.hpp"

#include <ctype.h>
#include <sstream>
#include <filesystem>

std::string Config::MGE_MODEL_PATH = "mge/models/";
std::string Config::MGE_TEXTURE_PATH = "mge/textures/";
std::string Config::MGE_SHADER_PATH = "mge/shaders/";
std::string Config::MGE_FONT_PATH = "mge/fonts/";
std::string Config::MGE_LEVEL_PATH = "mge/levels/";
std::string Config::MGE_SETTINGS_PATH = "mge/settings/";
Event<bool> Config::onConfigUpdated;

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
	onConfigUpdated(true);
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
	defaultValue = std::stof(tryGetValue(key, defaultValue));
}
void Config::updateValue(std::string key, float& defaultValue)
{
	defaultValue = std::stof(tryGetValue(key, std::to_string(defaultValue)));
}

void Config::updateValue(std::string key, bool& defaultValue)
{
	defaultValue = std::stoi(tryGetValue(key, std::to_string(defaultValue)));
}

void Config::updateValue(std::string key, double& defaultValue)
{
	defaultValue = std::stod(tryGetValue(key, std::to_string(defaultValue)));
}

void Config::updateValue(std::string key, int& defaultValue)
{
	defaultValue = std::stoi(tryGetValue(key, std::to_string(defaultValue)));
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