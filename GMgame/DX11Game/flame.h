//=============================================================================
//
// ������ [flame.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#pragma once

#include "main.h"

// �}�N����`
#define MAX_FLAME		(300)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitFlame(void);
void UninitFlame(void);
void UpdateFlame(void);
void DrawFlame(void);

int SetFlame(XMFLOAT3 pos, float size,int no);

