//=============================================================================
//
// �Q�[�W���� [gauge.h]
//
//=============================================================================
#pragma once

#include "main.h"

// �}�N����`
#define EGAUGE_MAX	(100)		//100%

// �v���g�^�C�v�錾
HRESULT InitEGauge(void);
void UninitEGauge(void);
void UpdateEGauge(void);
// ���W�͍��[���w��
void DrawEGauge( XMFLOAT2 pos, float percent );