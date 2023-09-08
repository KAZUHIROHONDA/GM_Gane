//=============================================================================
//
// サウンド処理 [sound.h]
// Author : AKIRA TANAKA
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

#include <xaudio2.h>
// Windows7の場合は上記を削除して以下に置き換え.
//#include <C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Include\xaudio2.h>

//*****************************************************************************
// サウンドファイル
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_BGM000 = 0,		// BGM0
	SOUND_LABEL_BGM001,			// BGM1
	SOUND_LABEL_BGM002,
	SOUND_LABEL_SE_PARTS1,		//パーツ選択音4
	SOUND_LABEL_SE_PARTS2,		//パーツ選択音5
	SOUND_LABEL_SE_PARTS3,		//パーツ選択音6
	SOUND_LABEL_SE_PARTS4,		//パーツ選択音7
	SOUND_LABEL_SE_PARTS5,		//パーツ選択音8
	SOUND_LABEL_SE_PARTS6,		//パーツ選択音9
	SOUND_LABEL_SE_START,
	SOUND_LABEL_SE_SELECTS,
	SOUND_LABEL_SE_POSE,
	SOUND_LABEL_BGM_1,			//タイトル
	SOUND_LABEL_BGM_2,			//セレクト
	SOUND_LABEL_BGM_3,			
	SOUND_LABEL_BGM_4,			
	SOUND_LABEL_BGM_5,			//キメラ17
	SOUND_LABEL_SE_SET,
	SOUND_LABEL_SE_GO,
	SOUND_LABEL_SE_CANCEL,
	SOUND_LABEL_SE_PUNCH,
	SOUND_LABEL_SE_SENPUU1,
	SOUND_LABEL_SE_SENPUU2,
	SOUND_LABEL_SE_KORO,
	SOUND_LABEL_SE_BUN,
	SOUND_LABEL_SE_TAI,

	SOUND_LABEL_MAX,			// 登録数の自動設定
} SOUND_LABEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);

HRESULT PlaySound(SOUND_LABEL label);	// 再生

void StopSound(SOUND_LABEL label);		// 1つ停止
void StopSound(void);					// 全部停止
void PauseSound( void );				// 一時停止
void ResumeSound( void );				// 再開

void SetVolume(float fVol);				// 音量調整

#endif
