#pragma once

#include <string>
#include <map>
using std::string, std::map;

// NOTE: This is where to add more configs
enum class Config
{
	CHIP_8,
	CHIP_48
};

const map<string, Config> CONFIG_CLI_MAP{
	{"chip-8", Config::CHIP_8},
	{"chip-48", Config::CHIP_48}
};
