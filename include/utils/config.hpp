#pragma once

// NOTE: This is where to add more configs
enum class Config
{
	COSMIC,
	SUPER_CHIP
};

inline Config flagsToConfig(bool cosmic_flag, bool super_flag)
{
	if (cosmic_flag)
		return Config::COSMIC;
	if (super_flag)
		return Config::SUPER_CHIP;

	// NOTE: COSMIC is the default
	return Config::COSMIC;
}
