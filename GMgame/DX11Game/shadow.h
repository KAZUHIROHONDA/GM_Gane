//=============================================================================
//
// �n�ʏ��� [shadow.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#pragma once

#include "main.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitShadow(void);
void UninitShadow(void);
void UpdateShadow(void);
void DrawShadow(void);

int CreateShadow(XMFLOAT3 pos,float radius);		//�e���쐬����
void MoveShadow(int nShadow, XMFLOAT3 pos);			//�e���ړ�������
void ReleaseShadow(int nShadow);						//�e���������
