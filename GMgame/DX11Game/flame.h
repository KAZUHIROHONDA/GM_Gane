//=============================================================================
//
// 炎処理 [flame.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#pragma once

#include "main.h"

// マクロ定義
#define MAX_FLAME		(300)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitFlame(void);
void UninitFlame(void);
void UpdateFlame(void);
void DrawFlame(void);

int SetFlame(XMFLOAT3 pos, float size,int no);

