//=============================================================================
//
// エフェクシアのエフェクト処理 [effect.h]
//
//=============================================================================
#pragma once

#include "main.h"

// マクロ定義
enum
{
	E_EFFECT_LASER,
	E_EFFECT_HOKORI,
	E_EFFECT_AREA,

	E_EFFECT_MAX
};

// プロトタイプ宣言
HRESULT InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);

void SetEffect(int no, XMFLOAT3 pos);