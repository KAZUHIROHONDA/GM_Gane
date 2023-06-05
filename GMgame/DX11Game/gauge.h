//=============================================================================
//
// ゲージ処理 [gauge.h]
//
//=============================================================================
#pragma once

#include "main.h"

// マクロ定義
#define GAUGE_MAX	(100)		//100%

// プロトタイプ宣言
HRESULT InitGauge(void);
void UninitGauge(void);
void UpdateGauge(void);
// 座標は左端を指定
void DrawGauge( XMFLOAT2 pos, float percent );