//=============================================================================
//
// プレイヤー処理 [Upartsmenu.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#pragma once

#include "main.h"
#include "playercs.h"

// ポーズメニュー
typedef enum {
	UPARTS_MENU_KABU = 0,	// コンティニュー
	UPARTS_MENU_WANI,			// リトライ
	UPARTS_MENU_PANDA,
	UPARTS_MENU_TORI,
	UPARTS_MENU_BUTA,
	UPARTS_MENU_USI,
	UPARTS_MENU_MAX
} UPARTS_MENU;

// プロトタイプ宣言
HRESULT InitUParts(void);
void UninitUParts(void);
void UpdateUParts(void);
void DrawUParts(void);

void SetUPartsMenu(void);
UPARTS_MENU GetUPartsMenu(void);
void ResetUPartsMenu(void);
void ResetUPartsMenu1(void);

Player* UPartsGet();