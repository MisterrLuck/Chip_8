#pragma once
#include "raylib.h"

// WARNING: I dont know what to set this
#define BUFFER_SIZE 4096
#define SAMPLE_RATE 44100

class SoundManager
{
public:
	SoundManager();
	~SoundManager();

	void playSound();
	void stopSound();
	void updateStream();

	bool playing = false;

	float buffer[BUFFER_SIZE] = {};
	AudioStream beep;
};

