//=============================================================================
//
// ゲージ処理 [gauge.h]
//
//=============================================================================
#pragma once

#include "main.h"

// マクロ定義
#define EGAUGE_MAX	(100)		//100%

// プロトタイプ宣言
HRESULT InitEGauge(void);
void UninitEGauge(void);
void UpdateEGauge(void);
// 座標は左端を指定
void DrawEGauge( XMFLOAT2 pos, float percent );