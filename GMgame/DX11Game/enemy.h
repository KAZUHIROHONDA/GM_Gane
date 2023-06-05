//=============================================================================
//
// �G���� [enemy.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#pragma once

#include "main.h"

//�}�N����`
#define ENEMY_MAX	(100)


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);

int SetEnemy(XMFLOAT3 pos,int no);

bool IsEnemy(int no);
void DestroyEnemy(int no);
XMFLOAT3 GetEnemyPos(int no);
XMFLOAT3 GetEnemySize(int no);

void DamageEnemy(int damage);

int GetEnemyHp(int no);
