#pragma once 

// DirectSoundの各種ヘッダーのインクルードが必要なため
// すでにまとめてあるヘッダーをインクルードする
#include "directsound.h"

class SoundBuffer
{
public:
	IDirectSoundBuffer8*	pSecondaryBuffer;	// セカンダリバッファ

	// コンストラクタ
	SoundBuffer();
	// デストラクタ
	~SoundBuffer();

	bool Create(IDirectSound8*	pDirectSound8, WAVEFORMATEX& WaveFormat, byte* WaveData, int DataSize);

	void Play(bool isLoop);
	void Stop();
};