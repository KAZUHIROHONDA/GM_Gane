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
	PARTS_MENU_WANI,	
	PARTS_MENU_PANDA,
	PARTS_MENU_TORI,
	PARTS_MENU_BUTA,
	PARTS_MENU_USI,// リトライ
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

Player* PartsGet();
int CharaNo();