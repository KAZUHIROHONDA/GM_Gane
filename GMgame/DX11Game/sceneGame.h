//=============================================================================
//
// ゲームシーン [sceanGame.h]
// Author : TAKUYA KATOU
//
//=============================================================================
#pragma once

#include "main.h"
#include "phasecs.h"
#include "playercs.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSceneGame();
void	UninitSceneGame();
void	UpdateSceneGame();
void	DrawSceneGame();

void Clearflag();

Phase* GetPhase();

