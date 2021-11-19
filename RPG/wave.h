#pragma once

// DirectSoundの各種ヘッダーのインクルードが必要なため
// すでにまとめてあるヘッダーをインクルードする
#include "directsound.h"

class Wave {
public:
	WAVEFORMATEX		WaveFormat;	// Waveフォーマット
	byte*				WaveData;	// 音の生データ
	int					DataSize;	// データサイズ

	// コンストラクタ
	Wave();
	// デストラクタ
	~Wave();

	// Wavファイル読み込み
	bool Load(const char* FileName);
};
