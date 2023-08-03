//=============================================================================
//
// タイトルシーン [sceanChimera.h]
// Author : KATOU TAKUYA
//
//=============================================================================
#pragma once

#include "main.h"
#include "chimeracs.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSceneChimera();
void	UninitSceneChimera();
void	UpdateSceneChimera();
void	DrawSceneChimera();

Chimera* Get();

