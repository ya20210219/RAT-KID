#pragma once

#include <xaudio2.h>
#include "GameObject.h"

#define SE_WIRE_SHOT ("asset\\audio\\Wire_Use.wav")
#define SE_GET_COIN ("asset\\audio\\Coin.wav")
#define SE_EXPLOSION ("asset\\audio\\Explosion.wav")
#define SE_DRONE ("asset\\audio\\Drone.wav")

class Audio : public GameObject
{
private:
	static IXAudio2*				m_Xaudio;
	static IXAudio2MasteringVoice*	m_MasteringVoice;

	IXAudio2SourceVoice*	m_SourceVoice;
	BYTE*					m_SoundData;

	int						m_Length;
	int						m_PlayLength;


public:

	static void InitMaster();
	static void UninitMaster();

	void Init() {};
	void UnInit();
	void Update() {};
	void Draw() {};

	void Load(const char *FileName);
	void Play(bool Loop, float volume);
	void SetVolume(float volume);
};

