//=============================================================================
//
// �Ǐ��� [wall.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#pragma once

#include "main.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitWall(void);
void UninitWall(void);
void UpdateWall(void);
void DrawWall(void);

void SetWall(XMFLOAT3 pos, float rotY);
void ClearWall();
