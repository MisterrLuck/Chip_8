#pragma once

#include <string>
#include <map>
using std::string, std::map;

// NOTE: This is where to add more configs
enum class Config
{
	COSMIC,
	SUPER_CHIP
};

const map<string, Config> CONFIG_CLI_MAP{
	{"cosmic", Config::COSMIC},
	{"super_chip", Config::SUPER_CHIP}
};
