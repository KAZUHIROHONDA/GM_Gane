//=============================================================================
//
// プレイヤー処理 [Mpartsmenu.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#pragma once

#include "main.h"
#include "playercs.h"

// ポーズメニュー
typedef enum {
	MPARTS_MENU_KABU = 0,	// コンティニュー
	MPARTS_MENU_WANI,			// リトライ
	MPARTS_MENU_MAX
} MPARTS_MENU;

// プロトタイプ宣言
HRESULT InitMParts(void);
void UninitMParts(void);
void UpdateMParts(void);
void DrawMParts(void);

void SetMPartsMenu(void);
MPARTS_MENU GetMPartsMenu(void);
void ResetMPartsMenu(void);
void ResetMPartsMenu1(void);

Player* MPartsGet();