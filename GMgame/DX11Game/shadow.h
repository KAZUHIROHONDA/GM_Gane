//=============================================================================
//
// 地面処理 [shadow.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#pragma once

#include "main.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitShadow(void);
void UninitShadow(void);
void UpdateShadow(void);
void DrawShadow(void);

int CreateShadow(XMFLOAT3 pos,float radius);		//影を作成する
void MoveShadow(int nShadow, XMFLOAT3 pos);			//影を移動させる
void ReleaseShadow(int nShadow);						//影を解放する
