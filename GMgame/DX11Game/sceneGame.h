//=============================================================================
//
// �Q�[���V�[�� [sceanGame.h]
// Author : TAKUYA KATOU
//
//=============================================================================
#pragma once

#include "main.h"
#include "phasecs.h"
#include "playercs.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSceneGame();
void	UninitSceneGame();
void	UpdateSceneGame();
void	DrawSceneGame();

void Clearflag();

Phase* GetPhase();

