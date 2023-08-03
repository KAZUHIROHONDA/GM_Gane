//=============================================================================
//
// プレイヤー処理 [partsmenu.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#pragma once

#include "main.h"
#include "playercs.h"

// ポーズメニュー
typedef enum {
	PARTS_MENU_KABU = 0,	// コンティニュー
	PARTS_MENU_WANI,			// リトライ
	PARTS_MENU_MAX
} PARTS_MENU;

// プロトタイプ宣言
HRESULT InitParts(void);
void UninitParts(void);
void UpdateParts(void);
void DrawParts(void);

void SetPartsMenu(void);
PARTS_MENU GetPartsMenu(void);
void ResetPartsMenu(void);
void ResetPartsMenu1(void);

Player* PartsGet();