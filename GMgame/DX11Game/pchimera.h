//=============================================================================
//
// プレイヤー処理 [pchimera.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#pragma once

#include "main.h"

//マクロ定義
#define PCHIMERA_MAX	(1)

enum
{
	CAMERA_VIEW_FIXED,		//固定
	CAMERA_VIEW_BIRD,			//バードビュー
	CAMERA_VIEW_BEHIND,		//ビハインドビュー
	CAMERA_VIEW_FPS,			//FPSビュー
	CAMERA_VIEW_CAMERA_DEBUG,	//CAMERA_DEBUG用
	CAMERA_VIEW_DIAGONAL,
	CAMERA_VIEW_MAX,		//
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitPChimera(void);
void UninitPChimera(void);
void UpdatePChimera(void);
void DrawPChimera(void);

void Sflag();