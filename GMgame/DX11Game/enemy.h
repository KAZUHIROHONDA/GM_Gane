//=============================================================================
//
// 敵処理 [enemy.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#pragma once

#include "main.h"

//マクロ定義
#define ENEMY_MAX	(100)


//*****************************************************************************
// プロトタイプ宣言
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
