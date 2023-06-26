//=============================================================================
//
// フェード処理 [fade.h]
//
//=============================================================================
#pragma once

#include "main.h"

// マクロ定義
enum E_FADE 
{
	E_FADE_NONE,	// フェードしていない
	E_FADE_IN,		// フェードイン中
	E_FADE_OUT,		// フェードアウト中
	E_FADE_MAX
};

// プロトタイプ宣言
HRESULT InitFade(void);
void UninitFade(void);
void UpdateFade(void);
void DrawFade(void);

void StartFade( E_TYPE_SCENE stateNext, int nFrame = 30 );	// フェード開始
E_FADE GetFade();		// フェード中？
void WaitTimer(int second);
