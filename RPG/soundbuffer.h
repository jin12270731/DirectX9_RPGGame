#pragma once 

// DirectSound�̊e��w�b�_�[�̃C���N���[�h���K�v�Ȃ���
// ���łɂ܂Ƃ߂Ă���w�b�_�[���C���N���[�h����
#include "directsound.h"

class SoundBuffer
{
public:
	IDirectSoundBuffer8*	pSecondaryBuffer;	// �Z�J���_���o�b�t�@

	// �R���X�g���N�^
	SoundBuffer();
	// �f�X�g���N�^
	~SoundBuffer();

	bool Create(IDirectSound8*	pDirectSound8, WAVEFORMATEX& WaveFormat, byte* WaveData, int DataSize);

	void Play(bool isLoop);
	void Stop();
};