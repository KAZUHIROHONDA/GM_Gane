//=============================================================================
//
// �Q�[�W���� [gauge.h]
//
//=============================================================================
#pragma once

#include "main.h"

// �}�N����`
#define GAUGE_MAX	(100)		//100%

// �v���g�^�C�v�錾
HRESULT InitGauge(void);
void UninitGauge(void);
void UpdateGauge(void);
// ���W�͍��[���w��
void DrawGauge( XMFLOAT2 pos, float percent );