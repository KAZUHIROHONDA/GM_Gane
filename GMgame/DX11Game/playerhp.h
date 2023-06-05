//=============================================================================
//
// スコア画像表示処理 [playerhp.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#pragma once

#include "main.h"

//*****************************************************************************
// 定数宣言
//*****************************************************************************
#define PLAYERHP_MAX		(1)	// スコアの最大数



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT	InitPlayerhp();
void	UninitPlayerhp();
void	UpdatePlayerhp();
void	DrawPlayerhp();

//void	AddPlayerhp(int num);	// スコア加算


