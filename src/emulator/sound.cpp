#include "sound.hpp"
#include <math.h>

SoundManager::SoundManager()
{
	InitAudioDevice();
	SetAudioStreamBufferSizeDefault(BUFFER_SIZE);

	// Init raw audio stream (sample rate: 44100, sample size: 32bit-float, channels: 1-mono)
	beep = LoadAudioStream(SAMPLE_RATE, 32, 1);
    SetAudioStreamPan(beep, 0);

	playing = false;

	updateStream();
}

SoundManager::~SoundManager()
{
	UnloadAudioStream(beep);
	CloseAudioDevice();
}

void SoundManager::playSound()
{
	if (!playing)
		PlayAudioStream(beep);
	playing = true;

	updateStream();
}

void SoundManager::stopSound()
{
	if (playing)
		StopAudioStream(beep);
	playing = false;
}

// I don't really understand this but it's very important
void SoundManager::updateStream()
{
	int sineFrequency = 440;
    int sineIndex = 0;

	if (IsAudioStreamProcessed(beep))
	{
		for (int i = 0; i < BUFFER_SIZE; i++)
		{
			int wavelength = SAMPLE_RATE/sineFrequency;
			buffer[i] = sinf(2*PI*sineIndex/wavelength);
			sineIndex++;

			if (sineIndex >= wavelength)
				sineIndex = 0;
		}

		UpdateAudioStream(beep, buffer, BUFFER_SIZE);
	}
}
