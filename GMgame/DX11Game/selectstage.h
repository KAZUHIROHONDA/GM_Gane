//=============================================================================
//
// ポーズ処理 [selectstage.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#pragma once

#include "main.h"

// ポーズメニュー
typedef enum {
	SELECT_STAGE_1 = 0,	
	SELECT_STAGE_2,		
	SELECT_STAGE_3,	
	SELECT_STAGE_4,
	SELECT_STAGE_5,
	SELECT_STAGE_6,
	SELECT_STAGE_7,
	SELECT_STAGE_8,
	SELECT_STAGE_9,
	SELECT_MAX
} SELECT_STAGE;

// プロトタイプ宣言
HRESULT InitSelectStage(void);
void UninitSelectStage(void);
void UpdateSelectStage(void);
void DrawSelectStage(void);

void SetSelectStageMenu( void );
SELECT_STAGE GetSelectStageMenu( void );
void ResetSelectStageMenu( void );
