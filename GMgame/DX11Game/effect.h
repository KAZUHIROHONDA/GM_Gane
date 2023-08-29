//=============================================================================
//
// �G�t�F�N�V�A�̃G�t�F�N�g���� [effect.h]
//
//=============================================================================
#pragma once

#include "main.h"

// �}�N����`
enum
{
	E_EFFECT_LASER,
	E_EFFECT_HOKORI,
	E_EFFECT_AREA,

	E_EFFECT_MAX
};

// �v���g�^�C�v�錾
HRESULT InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);

void SetEffect(int no, XMFLOAT3 pos);