//=============================================================================
//
// �^�C�g���V�[�� [sceanTitle.h]
// Author : KATOU TAKUYA
//
//=============================================================================
#pragma once

#include "main.h"
#include "playercs.h"
#include "model.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSceneTitle();
void	UninitSceneTitle();
void	UpdateSceneTitle();
void	DrawSceneTitle();

Player* GetPlayer();
Player* GetEnemy();

Model* GetPlayerModel();


