//=============================================================================
//
// タイトルシーン [sceanTitle.h]
// Author : KATOU TAKUYA
//
//=============================================================================
#pragma once

#include "main.h"
#include "playercs.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSceneTitle();
void	UninitSceneTitle();
void	UpdateSceneTitle();
void	DrawSceneTitle();

Player* GetPlayer();
Player* GetEnemy();



