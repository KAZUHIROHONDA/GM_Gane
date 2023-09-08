//=============================================================================
//
// �T�E���h���� [sound.h]
// Author : AKIRA TANAKA
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

#include <xaudio2.h>
// Windows7�̏ꍇ�͏�L���폜���Ĉȉ��ɒu������.
//#include <C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Include\xaudio2.h>

//*****************************************************************************
// �T�E���h�t�@�C��
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_BGM000 = 0,		// BGM0
	SOUND_LABEL_BGM001,			// BGM1
	SOUND_LABEL_BGM002,
	SOUND_LABEL_SE_PARTS1,		//�p�[�c�I����
	SOUND_LABEL_SE_PARTS2,		//�p�[�c�I����
	SOUND_LABEL_SE_PARTS3,		//�p�[�c�I����
	SOUND_LABEL_SE_PARTS4,		//�p�[�c�I����
	SOUND_LABEL_SE_PARTS5,		//�p�[�c�I����
	SOUND_LABEL_SE_PARTS6,		//�p�[�c�I����
	SOUND_LABEL_SE_START,
	SOUND_LABEL_SE_SELECTS,
	SOUND_LABEL_SE_POSE,
	SOUND_LABEL_BGM_1,			//�^�C�g��
	SOUND_LABEL_BGM_2,			//�Z���N�g
	SOUND_LABEL_BGM_3,			
	SOUND_LABEL_BGM_4,			
	SOUND_LABEL_BGM_5,			//�L����
	SOUND_LABEL_SE_CANCEL,
	SOUND_LABEL_SE_PUNCH,

	SOUND_LABEL_MAX,			// �o�^���̎����ݒ�
} SOUND_LABEL;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);

HRESULT PlaySound(SOUND_LABEL label);	// �Đ�

void StopSound(SOUND_LABEL label);		// 1��~
void StopSound(void);					// �S����~
void PauseSound( void );				// �ꎞ��~
void ResumeSound( void );				// �ĊJ

void SetVolume(float fVol);				// ���ʒ���

#endif
